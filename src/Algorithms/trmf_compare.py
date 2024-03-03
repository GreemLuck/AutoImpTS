import matplotlib.pyplot as plt

import trmfpy
import numpy as np
from time import perf_counter

ticks = [100,200,300,400,500,600,700,800]
dataset = 'chlorine'
params_bench = {"k":5,
                "max_iter":40,
                "threshold":1e-5,
                "lambdaI":0.75,
                "lambdaAR":0.75,
                "lambdaLag":0.75,
                "lag_set":range(1,11)
                }

params_new = {"k":5,
              "max_iter":40,
              "threshold":1e-5,
              "lambdaI":0.0,
              "lambdaAR":1.0,
              "lambdaLag":1.0,
              "lag_set": np.array(list(range(1, 25)) + list(range(7 * 24, 8 * 24)), dtype=np.uint32)
              }

def getdata():
    rmse_bench = []
    rmse_new = []
    runtime_bench = []
    runtime_new = []

    for t in ticks:
        t1_start = perf_counter()
        rmse_bench.append(trmfpy.main(dataset, t, **params_bench))
        t1_stop = perf_counter()
        runtime_bench.append(t1_stop - t1_start)

        t2_start = perf_counter()
        rmse_new.append(trmfpy.main(dataset, t, **params_new))
        t2_stop = perf_counter()
        runtime_new.append(t2_stop - t2_start)

    with open(f"trmf_compare_results_{dataset}_bayes.txt", "w") as f:
        for i in range(len(ticks)):
            f.write(f"{ticks[i]} {rmse_bench[i]} {runtime_bench[i]} {rmse_new[i]} {runtime_new[i]}\n")

    return rmse_bench, rmse_new, runtime_bench, runtime_new


def plot(ticks, rmse_bench, rmse_new, runtime_bench, runtime_new):
    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(12, 6))
    label_bench = f"k: {params_bench['k']}\n" \
                  f"lambdas:[{params_bench['lambdaI']}, {params_bench['lambdaAR']}, {params_bench['lambdaLag']}]"
    label_new = f"k: {params_new['k']}\n" \
                f"lambdas:[{params_new['lambdaI']}, {params_new['lambdaAR']}, {params_new['lambdaLag']}]"
    ax1.plot(ticks, rmse_bench, 'o-', label=f"bench\n{label_bench}")
    ax1.plot(ticks, rmse_new, 'o-', label=f"new\n{label_new}")
    ax1.set_title("rmse")
    ax1.set_xlabel("missing block size (permil)")
    ax1.set_ylabel("rmse")
    ax1.set_xticks(ticks)
    ax1.legend()
    ax2.plot(ticks, runtime_bench, 'o--', label=f"bench")
    ax2.plot(ticks, runtime_new, "o-.", label="new")
    ax2.set_title("runtime")
    ax2.set_xlabel("missing block size (permil)")
    ax2.set_ylabel("runtime (s)")
    ax2.set_xticks(ticks)
    ax2.legend()
    fig.suptitle("TRMF comparison between benchmark parameters and trmf package parameters"
                f"({dataset})")
    fig.text(0.02, 0.05, "test", fontsize=10)

    plt.savefig(f"trmf_compare_{dataset}_bayes")

rmse_bench, rmse_new, runtime_bench, runtime_new = getdata()

#ticks, rmse_bench, runtime_bench, rmse_new, runtime_new = np.loadtxt(f"trmf_compare_results_{dataset}.txt").T
plot(ticks, rmse_bench, rmse_new, runtime_bench, runtime_new)
