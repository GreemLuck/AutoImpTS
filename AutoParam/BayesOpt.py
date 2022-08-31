import os
import sys
from pathlib import Path
import sqlite3
import time

import numpy as np
import matplotlib.pyplot as plt
from bayes_opt import BayesianOptimization
from bayes_opt.logger import JSONLogger
from bayes_opt.event import Events

ROOT_FOLDER = str(Path(__file__).parent.parent.absolute())
EXE = "reduced_bench"

sys.path.append(os.path.join(ROOT_FOLDER, "AutoParam"))
import ts_algorithms

RUNTIME_RATIO = 0
TICKS = 100
DATASET = "airq"
LABEL = "bayseOpt"
VERBOSE = False


def dynammo(truncation, max_iter):
    rmse, runtime = ts_algorithms.dynammo(truncation, max_iter,
                                          tick=TICKS,
                                          verbose=VERBOSE,
                                          dataset=DATASET,
                                          label=LABEL)
    return -rmse


def tkcm(truncation, d):
    rmse, runtime = ts_algorithms.tkcm(truncation, d,
                                       tick=TICKS,
                                       verbose=VERBOSE,
                                       dataset=DATASET,
                                       label=LABEL)
    return -rmse


def stmvl(alpha, gamma, win_size):
    rmse, runtime = ts_algorithms.stmvl(alpha, gamma, win_size,
                                        tick=TICKS,
                                        verbose=VERBOSE,
                                        dataset=DATASET,
                                        label=LABEL)
    return -rmse


def spirit(truncation, win_size, lbda):
    rmse, runtime = ts_algorithms.spirit(truncation, win_size, lbda,
                                         tick=TICKS,
                                         verbose=VERBOSE,
                                         dataset=DATASET,
                                         label=LABEL)
    return -rmse


def grouse(truncation):
    rmse, runtime = ts_algorithms.grouse(truncation,
                                         tick=TICKS, verbose=VERBOSE, dataset=DATASET, label=LABEL)
    return -rmse


def nnmf(truncation, tolerance, max_iter):
    rmse, runtime = ts_algorithms.nnmf(truncation, tolerance, max_iter,
                                       tick=TICKS, verbose=VERBOSE, dataset=DATASET, label=LABEL)
    return -rmse


def svt(tolerance, tauscale, max_iter):
    rmse, runtime = ts_algorithms.svt(tolerance, tauscale, max_iter,
                                      tick=TICKS, verbose=VERBOSE, dataset=DATASET, label=LABEL)
    return -rmse


def rosl(truncation, tolerance, max_iter):
    rmse, runtime = ts_algorithms.rosl(truncation, tolerance, max_iter,
                                       tick=TICKS, verbose=VERBOSE, dataset=DATASET, label=LABEL)
    return -rmse


def itersvd(truncation, tolerance, max_iter):
    rmse, runtime = ts_algorithms.itersvd(truncation, tolerance, max_iter,
                                          tick=TICKS, verbose=VERBOSE, dataset=DATASET, label=LABEL)
    return -rmse


def softimp(truncation, tolerance, max_iter):
    rmse, runtime = ts_algorithms.softimp(truncation, tolerance, max_iter,
                                          tick=TICKS, verbose=VERBOSE, dataset=DATASET, label=LABEL)
    return -rmse


def cdrec(truncation, tolerance, max_iter):
    rmse, runtime = ts_algorithms.cdrec(truncation, tolerance, max_iter,
                                        tick=TICKS, verbose=VERBOSE, dataset=DATASET, label=LABEL)
    return -rmse


def trmf(tolerance, max_iter, lambdaI, lambdaAR, lambdaLag):
    rmse, runtime = ts_algorithms.trmf(tolerance, max_iter, lambdaI, lambdaAR, lambdaLag,
                                       tick=TICKS, verbose=VERBOSE,
                                       dataset=DATASET, label=LABEL)
    return -rmse


def plot_grouse(bo):
    x = np.linspace(1, 10, 10000)

    mean, sigma = bo._gp.predict(x.reshape(-1, 1), return_std=True)

    plt.figure(figsize=(16, 9))
    plt.plot(x, mean)
    plt.fill_between(x, mean + sigma, mean - sigma, alpha=0.1)

    plt.scatter(bo.space.params, bo.space.target, c="red", s=50, zorder=10)
    plt.savefig(os.path.join(ROOT_FOLDER, "Graphs", "grouse_bayesOpt"))


algorithms = {
    "cdrec": (cdrec, ts_algorithms.CDREC_BOUNDS),
    "cd": (cdrec, ts_algorithms.CDREC_BOUNDS),
    "dynammo": (dynammo, ts_algorithms.DYNAMMO_BOUNDS),
    "dynnamo": (dynammo, ts_algorithms.DYNAMMO_BOUNDS),
    "tkcm": (tkcm, ts_algorithms.TKCM_BOUNDS),
    "stmvl": (stmvl, ts_algorithms.ST_MVL_BOUNDS),
    "st-mvl": (stmvl, ts_algorithms.ST_MVL_BOUNDS),
    "spirit": (spirit, ts_algorithms.SPIRIT_BOUNDS),
    "grouse": (grouse, ts_algorithms.GROUSE_BOUNDS),
    "nnmf": (nnmf, ts_algorithms.NNMF_BOUNDS),
    "svt": (svt, ts_algorithms.SVT_BOUNDS),
    "rosl": (rosl, ts_algorithms.ROSL_BOUDNS),
    "itersvd": (itersvd, ts_algorithms.ITERSVD_BOUNDS),
    "iter-svd": (itersvd, ts_algorithms.ITERSVD_BOUNDS),
    "svd": (itersvd, ts_algorithms.ITERSVD_BOUNDS),
    "softimpute": (softimp, ts_algorithms.SOFTIMP_BOUNDS),
    "softimp": (softimp, ts_algorithms.SOFTIMP_BOUNDS),
    "trmf": (trmf, ts_algorithms.TRMF_BOUNDS)
}


def main(alg, dataset="airq", tick=100, exploration=2, exploitation=5, verbose=False):
    global TICKS
    global DATASET
    global VERBOSE
    TICKS = int(tick)
    DATASET = dataset
    VERBOSE = verbose

    optimizer = BayesianOptimization(
        f=algorithms[alg][0],
        pbounds=algorithms[alg][1],
        random_state=2,
    )

    # logger = JSONLogger(path=os.path.join(ROOT_FOLDER, "BayesLogs", alg + "_" + dataset))
    # optimizer.subscribe(Events.OPTIMIZATION_STEP, logger)

    optimizer.maximize(
        init_points=int(exploration),
        n_iter=int(exploitation)
    )

    if alg == "grouse":
        plot_grouse(optimizer)
    return optimizer.max


if __name__ == "__main__":
    max = main(sys.argv[1], **dict(arg.split('=') for arg in sys.argv[2:]))
    print(max)
