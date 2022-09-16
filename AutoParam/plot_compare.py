import sqlite3
import matplotlib.pyplot as plt
import numpy as np

dataset = "airq"
params_bench = {"truncation":3,
                "max_iter":100,
                "tolerance":1e-6}
params_sh = {"truncation":1,
             "max_iter":100,
             "tolerance":1e-6}
params_bayes = {"truncation":1,
                "max_iter":275,
                "tolerance":1e-8}

def getdata(params):
    db_path = "Results"
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    qry = f"SELECT Runs FROM NNMF " \
          f"WHERE Truncation={params['truncation']} " \
          f"AND Max_Iter={params['max_iter']} " \
          f"AND Tolerance={params['tolerance']} " \
          f"AND Dataset='{dataset}'"
    cursor.execute(qry)
    print(qry)
    rows, = cursor.fetchone()

    rows = rows.split("\n")
    rows = [x.split(",") for x in rows]
    ticks, rmses, runtimes = list(zip(*rows[:-1]))
    ticks = [int(x) for x in ticks]
    rmses = [float(x) for x in rmses]
    runtimes = [int(x) for x in runtimes]
    return [ticks, rmses, runtimes]

def setax(ax, x, y, setlabel=False, params=None, name=''):
    if setlabel:
        label = f"{name}\ntruncation: {params['truncation']}"
        ax.plot(x, y, "o-", label=label)
    else:
        label = ''

def main():
    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(12, 6))
    ticks, rmses_bench, runtimes_bench = getdata(params_bench)
    _, rmses_sh, runtimes_sh = getdata(params_sh)
    _, rmses_bayes, runtimes_bayes = getdata(params_bayes)

    ax1.plot(ticks, rmses_bench, "o-", label=f"bench\n"
                                             f"truncation: {params_bench['truncation']}\n"
                                             f"max_iter: {params_bench['max_iter']}\n"
                                             f"tolerance: {params_bench['tolerance']}")
    ax2.plot(ticks, runtimes_bench, "o-", label=f"bench")

    ax1.plot(ticks, rmses_sh, "o-", label=f"sh\n"
                                          f"truncation: {params_sh['truncation']}\n"
                                          f"max_iter: {params_sh['max_iter']}\n"
                                          f"tolerance: {params_sh['tolerance']}")
    ax2.plot(ticks, runtimes_sh, "o-", label=f"sh")

    ax1.plot(ticks, rmses_bayes, "o-", label=f"bayes\n"
                                             f"truncation: {params_bayes['truncation']}\n"
                                             f"max_iter: {params_bayes['max_iter']}\n"
                                             f"tolerance: {params_bayes['tolerance']}")
    ax2.plot(ticks, runtimes_bayes, "o-", label=f"bayes")

    ax1.set_title("rmse")
    ax1.set_xlabel("missing block size (permil)")
    ax1.set_ylabel("rmse")
    ax1.set_xticks(ticks)
    ax2.set_title("runtime")
    ax2.set_xlabel("missing block size (permil)")
    ax2.set_ylabel("runtime (μs)")
    fig.suptitle("NNMF comparison between benchmark parameters and autoparametrization algorithms parameters\n"
                 f"({dataset})")
    ax1.legend()
    ax2.legend()
    fig.tight_layout()
    plt.savefig(f"Graphs/compare/nnmf_compare_{dataset}")

if __name__ == "__main__":
    main()
