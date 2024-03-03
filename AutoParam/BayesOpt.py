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
SCENARIO = "MISSINGBLOCK"
DATASET = "airq"
LABEL = "bayesOpt"
VERBOSE = False


def dynammo(truncation, max_iter):
    rmse, *_ = ts_algorithms.dynammo(truncation, max_iter,
                                          tick=TICKS,
                                          verbose=VERBOSE,
                                          dataset=DATASET, 
                                          scenario=SCENARIO,
                                          parallel=MULTITHREAD,
                                          label=LABEL)
    return -rmse


def tkcm(truncation, d):
    rmse, *_ = ts_algorithms.tkcm(truncation, d,
                                       tick=TICKS,
                                       verbose=VERBOSE,
                                       dataset=DATASET, 
                                       scenario=SCENARIO,
                                       parallel=MULTITHREAD,
                                       label=LABEL)
    return -rmse


def stmvl(alpha, gamma, win_size):
    rmse, *_ = ts_algorithms.stmvl(alpha, gamma, win_size,
                                        verbose=VERBOSE,
                                        dataset=DATASET, 
                                        scenario=SCENARIO,
                                        parallel=MULTITHREAD,
                                        label=LABEL)
    return -rmse


def spirit(truncation, win_size, lbda):
    rmse, *_ = ts_algorithms.spirit(truncation, win_size, lbda,
                                         tick=TICKS,
                                         verbose=VERBOSE,
                                         dataset=DATASET, 
                                         scenario=SCENARIO,
                                         parallel=MULTITHREAD,
                                         label=LABEL)
    if rmse >= float("inf"):
        rmse = float("inf")
    return -rmse


def grouse(truncation):
    rmse, *_ = ts_algorithms.grouse(truncation,
                                         tick=TICKS, 
                                         verbose=VERBOSE, 
                                         dataset=DATASET, 
                                         scenario=SCENARIO,
                                         parallel=MULTITHREAD, 
                                         label=LABEL)
    return -rmse


def nnmf(truncation, tolerance, max_iter):
    tolerance = 1.*10**-int(tolerance)
    rmse, *_ = ts_algorithms.nnmf(truncation, tolerance, max_iter,
                                       tick=TICKS,
                                       verbose=VERBOSE, 
                                       dataset=DATASET, 
                                       scenario=SCENARIO,
                                       parallel=MULTITHREAD, 
                                       label=LABEL)
    return -rmse


def svt(tolerance, tauscale, max_iter):
    tolerance = 1.*10**-int(tolerance)
    rmse, *_ = ts_algorithms.svt(tolerance, tauscale, max_iter,
                                      verbose=VERBOSE, 
                                      dataset=DATASET, 
                                      scenario=SCENARIO,
                                      parallel=MULTITHREAD, 
                                      label=LABEL)
    return -rmse


def rosl(truncation, tolerance, max_iter):
    rmse, *_ = ts_algorithms.rosl(truncation, tolerance, max_iter,
                                       tick=TICKS, 
                                       verbose=VERBOSE, 
                                       dataset=DATASET, 
                                       scenario=SCENARIO,
                                       parallel=MULTITHREAD, 
                                       label=LABEL)
    return -rmse


def itersvd(truncation, tolerance, max_iter):
    rmse, *_ = ts_algorithms.itersvd(truncation, tolerance, max_iter,
                                          tick=TICKS, 
                                          verbose=VERBOSE, 
                                          dataset=DATASET, 
                                          scenario=SCENARIO,
                                          parallel=MULTITHREAD, 
                                          label=LABEL)
    return -rmse


def softimp(truncation, tolerance, max_iter):
    rmse, *_ = ts_algorithms.softimp(truncation, tolerance, max_iter,
                                          tick=TICKS, 
                                          verbose=VERBOSE, 
                                          dataset=DATASET, 
                                          scenario=SCENARIO,
                                          parallel=MULTITHREAD, 
                                          label=LABEL)
    return -rmse


def cdrec(truncation, tolerance=5, max_iter=100):
    rmse, *_ = ts_algorithms.cdrec(truncation, tolerance, max_iter,
                                        tick=TICKS, 
                                        verbose=VERBOSE, 
                                        dataset=DATASET, 
                                        scenario=SCENARIO,
                                        parallel=MULTITHREAD, 
                                        label=LABEL)
    return -rmse

def minimise_runtime(tolerance, max_iter):
    rmse, runtime, *_ = ALG_FUNC(FIX_TRUNCATION, tolerance, max_iter,
                             verbose=VERBOSE, 
                             dataset=DATASET, 
                             scenario=SCENARIO,
                             parallel=MULTITHREAD)
    return -rmse

# def trmf(lambdaI, lambdaAR, lambdaLag):
#     rmse, *_ = ts_algorithms.trmf(lambdaI=lambdaI,lambdaAR=lambdaAR, lambdaLag=lambdaLag, verbose=VERBOSE,
#                                        dataset=DATASET, scenario=SCENARIO, parallel=MULTITHREAD, label=LABEL)
#     return -rmse


def plot_grouse(bo, alg, dataset):
    param_name = bo.space.keys[0]
    bmin, bmax = bo.space.bounds[0]

    x = np.linspace(bmin, bmax, 10000)

    mean, sigma = bo._gp.predict(x.reshape(-1, 1), return_std=True)

    plt.figure(figsize=(16, 9))
    plt.plot(x, mean)
    plt.fill_between(x, mean + sigma, mean - sigma, alpha=0.1)
    plt.scatter(bo.space.params, bo.space.target, c="red", s=50, zorder=10)

    plt.title(f"Bayesian Optimization of {alg}'s {param_name} on {dataset}")
    plt.xlabel(f"{param_name}")
    plt.ylabel(f"rmse (reversed)")

    plt.savefig(os.path.join(ROOT_FOLDER, "Graphs", f"bayesOpt_{alg}_{param_name}"))


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
    "softimp": (softimp, ts_algorithms.SOFTIMP_BOUNDS)
}


def main(alg_name, dataset="airq", bounds=None, tick=100, exploration=2, exploitation=5, verbose=False, alg_func=None,
         process_runtime=False, max_runtime=0, fix_truncation=3, scenario="MISSINGBLOCK", multithread=True):
    global TICKS
    global DATASET
    global VERBOSE
    global ALG_FUNC
    global MAX_RUNTIME
    global FIX_TRUNCATION
    global SCENARIO
    global MULTITHREAD

    TICKS = int(tick)
    DATASET = dataset
    SCENARIO = scenario
    MULTITHREAD = multithread
    VERBOSE = verbose
    ALG_FUNC = alg_func
    MAX_RUNTIME = max_runtime
    FIX_TRUNCATION = fix_truncation

    if bounds==None:
        alg, bounds = algorithms[alg_name]
    else:
        alg, _ = algorithms[alg_name]

    if process_runtime:
        alg = minimise_runtime

    optimizer = BayesianOptimization(
        f=alg,
        pbounds=bounds,
    )

    # logger = JSONLogger(path=os.path.join(ROOT_FOLDER, "BayesLogs", alg + "_" + dataset))
    # optimizer.subscribe(Events.OPTIMIZATION_STEP, logger)

    optimizer.maximize(
        init_points=int(exploration),
        n_iter=int(exploitation)
    )

    if optimizer.space.dim == 1:
        plot_grouse(optimizer, alg_name, dataset)

    max_ = optimizer.max
    # if 'tolerance' in max_['params']:
    #     tol = int(max_['params']['tolerance'])
    #     max_['params']['tolerance'] = 1.*10**-tol

    return max_


if __name__ == "__main__":
    max = main(sys.argv[1], **dict(arg.split('=') for arg in sys.argv[2:]))
    print(max)
