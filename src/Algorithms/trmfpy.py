import sys, os
import numpy as np
from time import perf_counter
import sqlite3

sys.path.append("exp-trmf-nips16/python/trmf")
import trmf

__location__ = os.path.realpath(
    os.path.join(os.getcwd(), os.path.dirname(__file__)))

__root__ = os.path. abspath(os.path.join(__location__, os.pardir))

LAG_SET = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]


def main(dataset: str = 'airq',
         ticks: int = 100,
         k: int = 5,
         max_iter: int = 40,
         lambdaI: float = 0.75,
         lambdaAR: float = 0.75,
         lambdaLag: float = 0.75,
         lag_set=LAG_SET):

    print(f"Starting TRMF on {dataset}, tick={ticks} ... ")

    ticks = int(ticks)
    k = int(k)
    lambdaI = float(lambdaI)
    lambdaAR = float(lambdaAR)
    lambdaLag = float(lambdaLag)

    Y = np.loadtxt(get_dataset_path(dataset), dtype=float)

    Ymiss = Y.copy()
    # Y = Y[:-(7 * 24), :]
    missing_block(Ymiss, ticks)

    threads = 40
    missing = False
    verbose = True

    start = perf_counter()

    m0 = trmf.Model.initialize(Ymiss, lag_set, k, seed=0)
    trmf.train(Ymiss, m0,
               lambdaI=lambdaI,
               lambdaAR=lambdaAR,
               lambdaLag=lambdaLag,
               max_iter=max_iter, missing=missing, verbose=verbose, threads=threads)

    end = perf_counter()
    runtime = end - start

    Ynew = m0.W.dot(m0.H.T)
    rmse = np.sqrt(np.mean(Ynew-Y)**2)

    # metrics = trmf.rolling_validate(Y, lag_set, k, window_size, nr_windows, lambdaI, lambdaAR, lambdaLag,
    #                                 max_iter=max_iter, threshold=threshold, transform=transform, threads=threads,
    #                                 seed=seed, missing=missing, verbose=verbose)
    print("TRMF executed.")
    sql_insert(dataset, k, max_iter, lambdaI, lambdaAR, lambdaLag, runtime, rmse)
    return rmse, runtime


def missing_block(Y, ticks):
    # Preparing the matrix
    n_rows = Y.shape[0]
    starting_index = int(round(n_rows * 0.05))
    block_size = int(round((n_rows * ticks) / 1000))
    reff_block = Y[starting_index:(block_size + starting_index), 0]
    replace = np.empty(block_size)
    replace[:] = 0
    Y[starting_index:(block_size + starting_index), 0] = replace
    return Y


def dataset2folder(dataset):
    if dataset == "airq":
        return "air_quality"
    if dataset == "drift10":
        return "drift"
    return dataset


def get_dataset_path(dataset):
    datapath = os.path.join(__root__, "Datasets", "real_world")
    filename = f"{dataset}_normal.txt"
    filepath = os.path.join(datapath, dataset2folder(dataset), filename)
    return filepath


def sql_insert(dataset: str,
               k: int,
               max_iter: int,
               lambdaI: float,
               lambdaAR: float,
               lambdaLag: float,
               runtime,
               rmse):
    db_path = os.path.join(__root__, 'Results')
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    qry = "INSERT INTO TRMF (Dataset, K, Max_iter, LambdaI, LambdaAR, LambdaLag, Runtime, Rmse, Label)" \
          f"VALUES ('{dataset}', {k}, {max_iter}, {lambdaI}, {lambdaAR}, {lambdaLag}, {runtime}, {rmse}, '')"
    cursor.execute(qry)
    conn.commit()
    conn.close()


if __name__ == "__main__":
    r, t = main(**dict(arg.split('=') for arg in sys.argv[2:]))
    print(f"RMSE : {r}")
    print(f"Runtime : {t}")
