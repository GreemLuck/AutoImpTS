import sqlite3
import matplotlib.pyplot as plt
import numpy as np
import BayesOpt
import ts_algorithms
import plotSH
import random_search
import random

alg_name = 'rosl'
dataset = "chlorine"
default_value = {"truncation":3,
                 "max_iter": 100,
                 "tolerance": 1e-6}

bounds_sh = {"truncation":(1,10,1)}
sample_size = 9
do_sh = True

bounds_bayes = {"truncation":(1,10), "max_iter":(50, 400), "tolerance":(0,10)}
exploration = 5
exploitation = 10

random.seed(2)

def getdata(runs):
    rows = runs.split("\n")
    rows = [x.split(",") for x in rows]
    ticks, rmses, runtimes = list(zip(*rows[:-1]))
    ticks = [int(x) for x in ticks]
    rmses = [float(x) for x in rmses]
    runtimes = [int(x) for x in runtimes]
    return [ticks, rmses, runtimes]

def plot_data(axes, ticks, rmses, runtimes, params, name='NoName'):
    ax_rmse, ax_runtime = axes
    str_params = '\n'.join([key + ": " + str(value) for key, value in params.items()])
    ax_rmse.plot(ticks, rmses, "o-", label=f"{name}\n" + str_params)
    ax_runtime.plot(ticks, runtimes, "o-", label=f"{name}")

def main(alg_name='cd', dataset='airq'):
    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(15, 8), dpi=200)
    alg_func, _ = ts_algorithms.get_algorithm(alg_name)

    # Control (default values)
    _, _, runs, params = alg_func(dataset=dataset, verbose=True)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1,  ax2), ticks, rmses, runtimes, params, name="Default")

    # Successive Halving (if available)
    if do_sh:
        result = plotSH.sh_execute(algorithm=alg_name, dataset=dataset, sample_size=sample_size,
                                   distribution=bounds_sh,
                                   resource_name='max_iter',
                                   resource_max=100, resource_min=5, keep_losers=False)
        _, _, runs, params = alg_func(**result, dataset=dataset, verbose=True)
        ticks, rmses, runtimes = getdata(runs)
        plot_data((ax1,  ax2), ticks, rmses, runtimes, params, name="SuccessiveH")

    # BayesOpt
    result = BayesOpt.main(alg_name, dataset=dataset, exploration=5, exploitation=7, bounds=bounds_bayes)
    _, _, runs, params = alg_func(**result["params"], dataset=dataset, verbose=True)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1,  ax2), ticks, rmses, runtimes, params, name="Bayes")

    # Random Search
    result = random_search.rs_execute(algorithm=alg_name, dataset=dataset, sample_size=sample_size,
                           distribution=bounds_sh)
    _, _, runs, params = alg_func(**result, dataset=dataset, verbose=True)
    print(runs)
    print(params)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1,  ax2), ticks, rmses, runtimes, params, name="Random Search")

    ax1.set_title("rmse")
    ax1.set_xlabel("missing block size (permil)")
    ax1.set_ylabel("rmse")
    ax1.set_xticks(ticks)
    ax2.set_title("runtime")
    ax2.set_xlabel("missing block size (permil)")
    ax2.set_ylabel("runtime (μs)")
    fig.suptitle(f"Algorithm: {alg_name}\n"
                 f"Dataset: {dataset}\n"
                 f"Comparing the best hyperparameter set of each autoparam algorithms")
    ax1.legend()
    ax2.legend()
    fig.tight_layout()
    fig.savefig(f"Graphs/compare/{alg_name}_compare_{dataset}")
    print(f"Saved in Graphs/compare/{alg_name}_compare_{dataset}")

def bayes_example():
    result = BayesOpt.main("cd")
    alg, _ = ts_algorithms.get_algorithm("cd")
    rmse, runtime, runs, *_ = alg(**result["params"], verbose=True)
    print(rmse)
    print(runtime)
    print(runs)

def sh_example():
    result = plotSH.sh_execute(algorithm='cd', dataset='airq', sample_size=9,
                               distribution={'truncation': (1,10,1)},
                               resource_name='max_iter',
                               resource_max=100, resource_min=5, keep_losers=False)
    alg, _ = ts_algorithms.get_algorithm("cd")
    rmse, runtime, runs, *_ = alg(**result, verbose=True)
    print(rmse)
    print(runtime)
    print(runs)

if __name__ == "__main__":
    main(alg_name=alg_name, dataset=dataset)