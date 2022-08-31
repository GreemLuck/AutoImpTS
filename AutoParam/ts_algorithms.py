import os
import sys
from pathlib import Path
import sqlite3
import time

ROOT_FOLDER = str(Path(__file__).parent.parent.absolute())
EXE = "reduced_bench"

sys.path.append(os.path.join(ROOT_FOLDER, "Algorithms"))
import trmfpy

CDREC_BOUNDS = {"truncation": (1, 10), "tolerance": (0, 1), "max_iter": (10, 1000)}
DYNAMMO_BOUNDS = {"truncation": (1, 10), "max_iter": (10, 200)}
TKCM_BOUNDS = {"truncation": (1, 10), "d": (0, 10)}
ST_MVL_BOUNDS = {"aplha": (0, 1), "gamma": (0, 1), "win_size": (0, 100)}
SPIRIT_BOUNDS = {"truncation": (1,10), "win_size": (0, 100), "lambda": (0,1)}
GROUSE_BOUNDS = {"truncation": (1, 10)}
NNMF_BOUNDS = {"tolerance": (0, 1), "truncation": (1, 10), "max_iter": (10, 1000)}
SVT_BOUNDS = {"tolerance": (0, 1), "tauscale": (0, 1), "max_iter": (10, 1000)}
ROSL_BOUDNS = {"tolerance": (0, 1), "truncation": (0, 1), "max_iter": (10, 1000)}
ITERSVD_BOUNDS = {"tolerance": (0, 1), "truncation": (0, 1), "max_iter": (10, 1000)}
SOFTIMP_BOUNDS = {"tolerance": (0, 1), "truncation": (0, 1), "max_iter": (10, 1000)}
TRMF_BOUNDS = {"tolerance": (0, 1), "lambdaI": (0, 1), "lambdaLag": (0, 1), "lambdaAR": (0, 1), "max_iter": (10, 100)}


def dynammo(truncation, max_iter, tick=100, dataset='airq', verbose=False, label = "dynammo-bayes"):
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
    rmse = cursor.execute("SELECT Rmse, Runtime FROM Dynnamo "
                          "WHERE Truncation=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          , (truncation, max_iter, label,)).fetchone()

    return rmse[0], rmse[1]


def tkcm(truncation, d, tick=100, dataset='airq', verbose=False, label = "tkcm-bayes"):
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
                          , (truncation, d, label,)).fetchone()

    return rmse[0], rmse[1]


def stmvl(alpha, gamma, win_size, tick=100, dataset='airq', verbose=False, label = "stmvl-bayes"):
    alg = "st-mvl"

    alpha = f"{alpha:.15f}"
    gamma = f"{gamma:.15f}"
    win_size = int(win_size)

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
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
                          , (alpha, gamma, win_size, label,)).fetchone()

    return rmse[0], rmse[1]


def spirit(truncation, win_size, lbda, tick=100, dataset='airq', verbose=False, label = "spirit-bayes"):
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
                          , (truncation, win_size, lbda, label,)).fetchone()

    return rmse[0], rmse[1]


def grouse(truncation, tick=100, dataset='airq', verbose=False, label = "grouse-bayes"):
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
                          , (truncation, label,)).fetchone()

    return rmse[0], rmse[1]


def nnmf(truncation, tolerance, max_iter, tick=100, dataset='airq', verbose=False, label = "nnmf-bayes"):
    alg = "nnmf"

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

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
    rmse = cursor.execute("SELECT Rmse, Runtime FROM NNMF "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          , (truncation, tolerance, max_iter, label,)).fetchone()

    return rmse[0], rmse[1]


def svt(tolerance, tauscale, max_iter, tick=100, dataset='airq', verbose=False, label = "svt-bayes"):
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
                          , (tolerance, tauscale, max_iter, label,)).fetchone()

    return rmse[0], rmse[1]


def rosl(truncation, tolerance, max_iter, tick=100, dataset='airq', verbose=False, label = "rosl-bayes"):
    alg = "rosl"

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

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
    rmse = cursor.execute("SELECT Rmse, Runtime FROM ROSL "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          , (truncation, tolerance, max_iter, label,)).fetchone()

    return rmse[0], rmse[1]


def itersvd(truncation, tolerance, max_iter, tick=100, dataset='airq', verbose=False, label = "itersvd-bayes"):
    alg = "itersvd"

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

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
    rmse = cursor.execute("SELECT Rmse, Runtime FROM IterSVD "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          , (truncation, tolerance, max_iter, label,)).fetchone()

    return rmse[0], rmse[1]


def softimp(truncation, tolerance, max_iter, tick=100, dataset='airq', verbose=False, label = "softimp-bayes"):
    alg = "softimpute"

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

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
    rmse = cursor.execute("SELECT Rmse, Runtime FROM SoftImpute "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          , (truncation, tolerance, max_iter, label,)).fetchone()

    return rmse[0], rmse[1]


def cdrec(truncation, tolerance, max_iter, tick=100, dataset='airq', verbose=False, label = "cdrec-bayes"):
    alg = "cd"

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

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
    rmse = cursor.execute("SELECT Rmse, Runtime FROM CDREC "
                          "WHERE Truncation=? "
                          "AND Tolerance=? "
                          "AND Max_iter=? "
                          "AND Label=?"
                          , (truncation, tolerance, max_iter, label,)).fetchone()

    return rmse[0], rmse[1]


def trmf(tolerance, max_iter, lambdaI, lambdaAR, lambdaLag, tick=100, dataset='airq', verbose=False, label = "trmf-bayes"):
    alg = "trmf"

    max_iter = int(max_iter)

    t_start = time.perf_counter()
    result = trmfpy.main(dataset, tick,
                         threshold=tolerance,
                         max_iter=max_iter,
                         lambdaI = lambdaI,
                         lambdaAR = lambdaAR,
                         lambdaLag = lambdaLag)
    t_stop = time.perf_counter()
    runtime = (t_stop - t_start) * 1e6

    print(f"tolerance: {tolerance}, max_iter: {max_iter}, lambdaI: {lambdaI}, lambdaAR: {lambdaAR}, lambdaLag: {lambdaLag}")

    conn = sqlite3.connect(ROOT_FOLDER + "/Results")
    cursor = conn.cursor()
    cursor.execute("INSERT INTO TRMF(Dataset, Ticks, Tolerance, Max_iter, LambdaI, "
                   "LambdaAR, LambdaLag, Runtime, Rmse, Label) "
                   "VALUES (?, ? ,? ,? ,?, ?, ?, ?, ?, ?)"
                   , (dataset, tick, tolerance, max_iter, lambdaI, lambdaAR, lambdaLag, runtime, result, label))
    print(f"RMSE : {result}")
    print(f"Runtime : {runtime}")
    return result, runtime