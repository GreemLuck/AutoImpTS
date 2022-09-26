import os
import sys
from pathlib import Path
import sqlite3
import time
from numpy import mean

ROOT_FOLDER = str(Path(__file__).parent.parent.absolute())
EXE = "reduced_bench"

sys.path.append(os.path.join(ROOT_FOLDER, "Algorithms"))
import trmfpy

CDREC_BOUNDS = {"truncation": (1, 10), "tolerance": (1, 10), "max_iter": (10, 1000)}
# CDREC_BOUNDS = {"truncation": (1, 10)}
DYNAMMO_BOUNDS = {"truncation": (1, 10), "max_iter": (10, 200)}
TKCM_BOUNDS = {"truncation": (1, 10), "d": (0, 10)}
ST_MVL_BOUNDS = {"alpha": (0, 5), "gamma": (0, 1), "win_size": (1, 100)}
SPIRIT_BOUNDS = {"truncation": (1,10), "win_size": (0, 100), "lambda": (0,1)}
GROUSE_BOUNDS = {"truncation": (1, 10)}
NNMF_BOUNDS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
SVT_BOUNDS = {"tolerance": (1, 10), "tauscale": (0, 1), "max_iter": (10, 1000)}
ROSL_BOUDNS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
ITERSVD_BOUNDS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
SOFTIMP_BOUNDS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
TRMF_BOUNDS = {"lambdaI": (0, 1), "lambdaLag": (0, 1), "lambdaAR": (0, 1)}


def get_algorithm(alg):
    algorithms = {
        "cdrec": (cdrec, CDREC_BOUNDS),
        "cd": (cdrec, CDREC_BOUNDS),
        "dynammo": (dynammo, DYNAMMO_BOUNDS),
        "dynnamo": (dynammo, DYNAMMO_BOUNDS),
        "tkcm": (tkcm, TKCM_BOUNDS),
        "stmvl": (stmvl, ST_MVL_BOUNDS),
        "st-mvl": (stmvl, ST_MVL_BOUNDS),
        "spirit": (spirit, SPIRIT_BOUNDS),
        "grouse": (grouse, GROUSE_BOUNDS),
        "nnmf": (nnmf, NNMF_BOUNDS),
        "svt": (svt, SVT_BOUNDS),
        "rosl": (rosl, ROSL_BOUDNS),
        "itersvd": (itersvd, ITERSVD_BOUNDS),
        "iter-svd": (itersvd, ITERSVD_BOUNDS),
        "svd": (itersvd, ITERSVD_BOUNDS),
        "softimpute": (softimp, SOFTIMP_BOUNDS),
        "softimp": (softimp, SOFTIMP_BOUNDS),
        "trmf": (trmf, TRMF_BOUNDS)
    }
    return algorithms[alg]


def dynammo(truncation=3, max_iter=100, tick=100, dataset='airq', verbose=False, label = "dynammo-bayes"):
    alg = "dynammo"

    truncation = int(truncation)
    max_iter = int(max_iter)

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    output = os.system(cmd)

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime, Runs FROM Dynammo "
                          "WHERE Truncation=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, max_iter, label, dataset)).fetchone()

    return rmse[0], rmse[1], rmse[2]


def tkcm(truncation=2, d=5, tick=100, dataset='airq', verbose=False, label = "tkcm-bayes"):
    alg = "tkcm"

    truncation = int(truncation)
    d = int(d)

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-d={d} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime FROM TKCM "
                          "WHERE Truncation=? "
                          "AND d=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, d, label, dataset)).fetchone()

    return rmse[0], rmse[1]


def stmvl(alpha=0.2, gamma=0.4, win_size=10, dataset='airq', verbose=False, label = "stmvl-bayes"):
    alg = "st-mvl"

    alpha = f"{alpha:.15f}"
    gamma = f"{gamma:.15f}"
    win_size = int(win_size)

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--dataset={dataset} " \
          f"--set-alpha={alpha} " \
          f"--set-gamma={gamma} " \
          f"--set-win-size={win_size} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime FROM ST_MVL "
                          "WHERE Alpha=? "
                          "AND Gamma=? "
                          "AND Win_Size=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (alpha, gamma, win_size, label, dataset)).fetchone()

    return rmse[0], rmse[1]


def spirit(truncation=3, win_size=10, lbda=0.3, tick=100, dataset='airq', verbose=False, label = "spirit-bayes"):
    alg = "spirit"

    truncation = int(truncation)
    lbda = f"{lbda:.15f}"
    win_size = int(win_size)

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={lbda} " \
          f"--set-win-size={win_size} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime FROM Spirit "
                          "WHERE Truncation=? "
                          "AND Win_Size=? "
                          "AND Lambda=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, win_size, lbda, label, dataset)).fetchone()

    return rmse[0], rmse[1]


def grouse(truncation=3, tick=100, dataset='airq', verbose=False, label = "grouse-bayes"):
    alg = "grouse"

    truncation = int(truncation)


    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime FROM Grouse "
                          "WHERE Truncation=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, label, dataset)).fetchone()

    return rmse[0], rmse[1]


def nnmf(truncation=3, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "nnmf-bayes"):
    alg = "nnmf"

    tol_return = tolerance

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

    params = {'truncation': truncation, 'tolerance': tol_return, 'max_iter': max_iter}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime, Runs FROM NNMF "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, tolerance, max_iter, label, dataset)).fetchone()

    rmse, runtime, runs, *_ = rmse

    return rmse, runtime, runs, params


def svt(tolerance=1e-6, tauscale=0.2, max_iter=100, tick=100, dataset='airq', verbose=False, label = "svt-bayes"):
    alg = "svt"

    tolerance = f"{tolerance:.15f}"
    tauscale = f"{tauscale:.15f}"
    max_iter = int(max_iter)

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-tolerance={tolerance} " \
          f"--set-tauscale={tauscale} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime FROM SVT "
                          "WHERE Tolerance=? "
                          "AND Tauscale=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (tolerance, tauscale, max_iter, label, dataset)).fetchone()

    return rmse[0], rmse[1]


def rosl(truncation=4, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "rosl-bayes"):
    alg = "rosl"

    tol_tmp = tolerance

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

    params = {'truncation': truncation, 'max_iter': max_iter, 'tolerance': tol_tmp}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime, Runs FROM ROSL "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, tolerance, max_iter, label, dataset)).fetchone()

    rmse, runtime, runs, *_ = rmse
    return rmse, runtime, runs, params


def itersvd(truncation=3, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "itersvd-bayes"):
    alg = "itersvd"

    tol_tmp = tolerance

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)
    params = {'truncation': truncation, 'max_iter': max_iter, 'tolerance': tol_tmp}


    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime, Runs FROM IterSVD "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, tolerance, max_iter, label, dataset)).fetchone()

    rmse, runtime, runs, *_ = rmse
    return rmse, runtime, runs, params


def softimp(truncation=3, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "softimp-bayes"):
    alg = "softimpute"

    tol_tmp = tolerance

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

    params = {'truncation': truncation, 'max_iter': max_iter, 'tolerance': tol_tmp}


    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime, Runs FROM SoftImpute "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, tolerance, max_iter, label, dataset)).fetchone()

    rmse, runtime, runs, *_ = rmse
    return rmse, runtime, runs, params


def cdrec(truncation=3, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "cdrec-bayes"):
    alg = "cd"

    truncation = int(truncation)
    tolerance_cmd = f"{tolerance:.15f}"
    max_iter = int(max_iter)

    params = {'truncation': truncation, 'tolerance': tolerance, 'max_iter': max_iter}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance_cmd} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    rmse = cursor.execute("SELECT Rmse, Runtime, Runs FROM CDREC "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          "AND Dataset=?"
                          , (truncation, tolerance_cmd, max_iter, label, dataset)).fetchone()
    rmse, runtime, runs, *_ = rmse

    return rmse, runtime, runs, params


def trmf(tolerance=1e-3, max_iter=40, lambdaI=0.7, lambdaAR=0.7, lambdaLag=0.7, k=10, dataset='airq', verbose=False,label="trmf-bayes"):
    alg = "trmf"
    TICKS = [100, 200, 300, 400, 500, 600, 700, 800]
    rmses = []
    runtimes = []

    max_iter = int(max_iter)

    for t in TICKS:
        t_start = time.perf_counter()
        result = trmfpy.main(dataset, ticks=t,
                             threshold=tolerance,
                             max_iter=max_iter,
                             lambdaI = lambdaI,
                             lambdaAR = lambdaAR,
                             lambdaLag = lambdaLag)
        t_stop = time.perf_counter()
        runtime = (t_stop - t_start) * 1e6
        rmses.append(result)
        runtimes.append(runtime)

    mean_rmse = mean(rmses)
    mean_runtime = mean(runtimes)

    print(f"RMSE : {mean_rmse}")
    print(f"RUNTIME : {mean_runtime}")

    return mean_rmse, mean_runtime