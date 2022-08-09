import sys, os
import scipy as sp
import numpy as np
sys.path.append("exp-trmf-nips16/python/trmf")
import trmf

def main(path, ticks=100, lambdaI=0.7, lambdaAR=125, lambdaLag=4, max_iter=40, threshold=1e-6):
    print("Starting TRMF ... ")
    Y = sp.loadtxt(path)
    # Y = Y[:-(7 * 24), :]
    missing_block(Y, ticks)

    lag_set = sp.array(list(range(1, 25)) + list(range(7 * 24, 8 * 24)), dtype=sp.uint32)
    #lag_set = sp.array(list(range(1, 7 * 24 + 1)), dtype=sp.uint32)
    k = 60
    #lambdaI = 0.7
    #lambdaAR = 125
    #lambdaLag = 4
    window_size = 24
    nr_windows = 7
    #max_iter = 40
    threads=40
    seed=0
    missing = False
    transform = True
    #threshold=0.5
    verbose=False

    metrics = trmf.rolling_validate(Y, lag_set, k, window_size, nr_windows, lambdaI, lambdaAR, lambdaLag, max_iter=max_iter, threshold=threshold, transform=transform, threads=threads, seed=seed, missing=missing, verbose=verbose)
    print(metrics.rmse)
    print("TRMF executed.")
    return metrics.rmse

def missing_block(Y, ticks):
    # Preparing the matrix
    n_rows = Y.shape[0]
    starting_index = round(n_rows * 0.05)
    block_size = round((n_rows * ticks) / 1000)
    reff_block = Y[starting_index:(block_size+starting_index), 0]
    replace = np.empty(block_size)
    replace[:] = 0
    Y[starting_index:(block_size+starting_index), 0] = replace

    return Y

if __name__ == "__main__":
    main('Datasets/real_world/air_quality/airq_normal.txt', 100)

