import sys, os
import scipy as sp
import numpy as np

sys.path.append("exp-trmf-nips16/python/trmf")
import trmf

__location__ = os.path.realpath(
    os.path.join(os.getcwd(), os.path.dirname(__file__)))

__root__ = os.path. abspath(os.path.join(__location__, os.pardir))


def main(dataset, ticks=100, max_iter=40, threshold=1e-6, lambdaI=0.7, lambdaAR=125, lambdaLag=4):
    print("Starting TRMF ... ")
    Y = sp.loadtxt(get_dataset_path(dataset))
    # Y = Y[:-(7 * 24), :]
    missing_block(Y, ticks)

    lag_set = sp.array(list(range(1, 25)) + list(range(7 * 24, 8 * 24)), dtype=sp.uint32)
    # lag_set = sp.array(list(range(1, 7 * 24 + 1)), dtype=sp.uint32)
    k = 60
    # lambdaI = 0.7
    # lambdaAR = 125
    # lambdaLag = 4
    window_size = 24
    nr_windows = 7
    # max_iter = 40
    threads = 40
    seed = 0
    missing = False
    transform = True
    # threshold=0.5
    verbose = True

    metrics = trmf.rolling_validate(Y, lag_set, k, window_size, nr_windows, lambdaI, lambdaAR, lambdaLag,
                                    max_iter=max_iter, threshold=threshold, transform=transform, threads=threads,
                                    seed=seed, missing=missing, verbose=verbose)
    print("TRMF executed.")
    return metrics.rmse


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


if __name__ == "__main__":
    main('airq', 100)