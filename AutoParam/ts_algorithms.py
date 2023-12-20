import os
from pathlib import Path
import numpy as np

ROOT_FOLDER = str(Path(__file__).parent.parent.absolute())
EXE = "build/reduced_bench"
DB_FILE_NAME = 'results.db'

# sys.path.append(os.path.join(ROOT_FOLDER, "Algorithms"))
# import trmfpy

CDREC_BOUNDS = {"truncation": (1, 10), "tolerance": (1, 10), "max_iter": (10, 1000)}
CDREC_BOUNDS_STEP = {"truncation":(1,10,1), "tolerance": (1,10,1), "max_iter": (1,1000,1)}

DYNAMMO_BOUNDS = {"truncation": (1, 10), "max_iter": (10, 200)}
DYNAMMO_BOUNDS_STEP = {"truncation": (1, 10, 1), "max_iter": (10, 200, 1)}

TKCM_BOUNDS = {"truncation": (1, 10), "d": (2, 10)}
TKCM_BOUNDS_STEP = {"truncation": (1, 10, 1), "d": (2, 10, 1)}

ST_MVL_BOUNDS = {"alpha": (0.00000001, 5), "gamma": (0.000001, 1), "win_size": (1, 100)}
ST_MVL_BOUNDS_STEP = {"alpha": (0.01, 5, 0.01), "gamma": (0.01, 1, 0.1), "win_size": (1, 100, 1)}

SPIRIT_BOUNDS = {"truncation": (1,10), "win_size": (1, 100), "lbda": (0.000001,1)}
SPIRIT_BOUNDS_STEP = {"truncation": (1,10, 1), "win_size": (1, 100, 1), "lbda": (0.1,1, 0.1)}

GROUSE_BOUNDS = {"truncation": (1, 10)}
GROUSE_BOUNDS_STEP = {"truncation": (1, 10, 1)}

NNMF_BOUNDS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
NNMF_BOUNDS_STEP = {"tolerance": (1, 10, 1), "truncation": (1, 10, 1), "max_iter": (10, 1000, 1)}

SVT_BOUNDS = {"tolerance": (1, 10), "tauscale": (0, 1), "max_iter": (10, 300)}
SVT_BOUNDS_STEP = {"tolerance": (1, 10, 1), "tauscale": (0, 1, 0.01), "max_iter": (10, 300, 1)}

ROSL_BOUDNS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
ROSL_BOUDNS_STEP = {"tolerance": (1, 10, 1), "truncation": (1, 10, 1), "max_iter": (10, 1000, 1)}

ITERSVD_BOUNDS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
ITERSVD_BOUNDS_STEP = {"tolerance": (1, 10, 1), "truncation": (1, 10, 1), "max_iter": (10, 1000, 1)}

SOFTIMP_BOUNDS = {"tolerance": (1, 10), "truncation": (1, 10), "max_iter": (10, 1000)}
SOFTIMP_BOUNDS_STEP = {"tolerance": (1, 10, 1), "truncation": (1, 10, 1), "max_iter": (10, 1000, 1)}
# TRMF_BOUNDS = {"lambdaI": (0, 1), "lambdaLag": (0, 1), "lambdaAR": (0, 1)}


def get_algorithm(alg):
    algorithms = {
        "cdrec": (cdrec, CDREC_BOUNDS, CDREC_BOUNDS_STEP),
        "cd": (cdrec, CDREC_BOUNDS, CDREC_BOUNDS_STEP),
        "dynammo": (dynammo, DYNAMMO_BOUNDS, DYNAMMO_BOUNDS_STEP),
        "dynnamo": (dynammo, DYNAMMO_BOUNDS, DYNAMMO_BOUNDS_STEP),
        "tkcm": (tkcm, TKCM_BOUNDS, TKCM_BOUNDS_STEP),
        "stmvl": (stmvl, ST_MVL_BOUNDS, ST_MVL_BOUNDS_STEP),
        "st-mvl": (stmvl, ST_MVL_BOUNDS, ST_MVL_BOUNDS_STEP),
        "spirit": (spirit, SPIRIT_BOUNDS, SPIRIT_BOUNDS_STEP),
        "grouse": (grouse, GROUSE_BOUNDS, GROUSE_BOUNDS_STEP),
        "nnmf": (nnmf, NNMF_BOUNDS, NNMF_BOUNDS_STEP),
        "svt": (svt, SVT_BOUNDS, SVT_BOUNDS_STEP),
        "rosl": (rosl, ROSL_BOUDNS, ROSL_BOUDNS_STEP),
        "itersvd": (itersvd, ITERSVD_BOUNDS, ITERSVD_BOUNDS_STEP),
        "iter-svd": (itersvd, ITERSVD_BOUNDS, ITERSVD_BOUNDS_STEP),
        "svd": (itersvd, ITERSVD_BOUNDS, ITERSVD_BOUNDS_STEP),
        "softimpute": (softimp, SOFTIMP_BOUNDS, SOFTIMP_BOUNDS_STEP),
        "softimp": (softimp, SOFTIMP_BOUNDS, SOFTIMP_BOUNDS_STEP)
    }
    return algorithms[alg]


def dynammo(truncation=3, max_iter=100, tick=100, dataset='airq', verbose=False, label = "dynammo-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "dynammo"

    truncation = int(truncation)
    max_iter = int(max_iter)

    params = {"truncation": truncation, "max_iter": max_iter}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"

    output = os.system(cmd)

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def tkcm(truncation=2, d=5, tick=100, dataset='airq', verbose=False, label="tkcm-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "tkcm"

    truncation = int(truncation)
    d = int(d)

    params = {"truncation": truncation, "d": d}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-d={d} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def stmvl(alpha=2, gamma=0.4, win_size=7, dataset='airq', misaligned=False, verbose=False, label = "stmvl-bayes", parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "st-mvl"

    alpha_temp = alpha
    gamma_temp = gamma

    alpha = f"{alpha:.5f}"
    gamma = f"{gamma:.5f}"
    win_size = int(win_size)

    params = {"alpha": alpha_temp, "gamma": gamma_temp, "win_size": win_size}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-alpha={alpha} " \
          f"--set-gamma={gamma} " \
          f"--set-win-size={win_size} " \
          f"--label={label}"

    if(parallel):
        cmd += " -m"
    if misaligned:
        cmd += " --misaligned"
    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def spirit(truncation=3, win_size=6, lbda=1, tick=100, dataset='airq', verbose=False, label = "spirit-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "spirit"

    lbda_temp = lbda

    truncation = int(truncation)
    lbda = f"{lbda:.15f}"
    win_size = int(win_size)

    params = {"truncation": truncation, "lbda": lbda_temp, "win_size": win_size}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-lambda={lbda} " \
          f"--set-win-size={win_size} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"
    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def grouse(truncation=3, tick=100, dataset='airq', verbose=False, label = "grouse-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "grouse"

    truncation = int(truncation)

    params = {"truncation": truncation}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"
    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def nnmf(truncation=3, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "nnmf-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
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
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"
    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def svt(tolerance=6, tauscale=0.2, max_iter=100, tick=100, dataset='airq', misaligned=False, verbose=False, label = "svt-bayes", parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "svt"

    tolerance = 1.*10**-int(tolerance)

    tol_tmp = tolerance
    tau_tmp = tauscale

    tolerance = f"{tolerance:.15f}"
    tauscale = f"{tauscale:.15f}"
    max_iter = int(max_iter)

    params = {"max_iter": max_iter, "tauscale": tau_tmp, "tolerance": tol_tmp}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-tolerance={tolerance} " \
          f"--set-tauscale={tauscale} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if parallel:
        cmd += " -m"
    if misaligned:
        cmd += " --misaligned"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")


    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def rosl(truncation=4, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "rosl-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "rosl"

    tolerance = 1.*10**-int(tolerance)
    tol_tmp = tolerance

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

    params = {'truncation': truncation, 'max_iter': max_iter, 'tolerance': tol_tmp}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"
    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def itersvd(truncation=3, tolerance=6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "itersvd-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "itersvd"

    tolerance = 1.*10**-int(tolerance)
    tol_tmp = tolerance

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)
    params = {'truncation': truncation, 'max_iter': max_iter, 'tolerance': tol_tmp}


    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"
    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def softimp(truncation=3, tolerance=1e-6, max_iter=100, tick=100, dataset='airq', verbose=False, label = "softimp-bayes", misaligned=False, parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "softimpute"

    tolerance = 1.*10**-int(tolerance)
    tol_tmp = tolerance

    truncation = int(truncation)
    tolerance = f"{tolerance:.15f}"
    max_iter = int(max_iter)

    params = {'truncation': truncation, 'max_iter': max_iter, 'tolerance': tol_tmp}


    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label} " \
          f"--misaligned={misaligned}"

    if(parallel):
        cmd += " -m"
    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params


def cdrec(truncation=3, tolerance=6, max_iter=100, tick=100, dataset='airq', misaligned=False, verbose=False, label = "cdrec-bayes", parallel=True, scenario="MISSINGBLOCK", scenv=""):
    alg = "cd"

    tolerance = 1.*10**-int(tolerance)

    truncation = int(truncation)
    tolerance_cmd = f"{tolerance:.15f}"
    max_iter = int(max_iter)

    params = {'truncation': truncation, 'tolerance': tolerance, 'max_iter': max_iter}

    cmd = f"{ROOT_FOLDER}/{EXE} " \
          f"--alg={alg} " \
          f"--tick={tick} " \
          f"--dataset={dataset} " \
          f"--scenario={scenario} " \
          f"--scenv={scenv} " \
          f"--set-truncation={truncation} " \
          f"--set-tolerance={tolerance_cmd} " \
          f"--set-max-iter={max_iter} " \
          f"--label={label}"

    if(parallel):
        cmd += " -m"
    if misaligned:
        cmd += " --misaligned"

    if verbose:
        os.system(cmd)
    else:
        os.system(cmd + "> /dev/null 2>&1")

    ids, rmse, runtime = read_results(ROOT_FOLDER + "/_data/out/results.txt")

    return rmse, runtime, params

def read_results(filename):
    # Read the data from the text file
    data = np.genfromtxt(filename, delimiter=',')

    # Check if the data is 1D (i.e., only one row in the file)
    if data.ndim == 1:
        # Reshape the data to be a 2D array with one row
        data = data.reshape(1, -1)

    # Separate the data into individual arrays
    ids = data[:, 0].astype(int)      # Convert the id to int
    rmses = data[:, 1]                # RMSE values are already float
    runtimes = data[:, 2]             # Runtime values are already float

    # Calculate the average of rmse and runtime
    avg_rmse = np.mean(rmses)
    avg_runtime = np.mean(runtimes)


    return ids, avg_rmse, avg_runtime



# def trmf(tolerance=1e-3, max_iter=40, lambdaI=0.7, lambdaAR=0.7, lambdaLag=0.7, k=10, dataset='airq', verbose=False,label="trmf-bayes"):
#     alg = "trmf"
#     TICKS = [100, 200, 300, 400, 500, 600, 700, 800]
#     rmses = []
#     runtimes = []
#
#     max_iter = int(max_iter)
#
#     for t in TICKS:
#         t_start = time.perf_counter()
#         result = trmfpy.main(dataset, ticks=t,
#                              threshold=tolerance,
#                              max_iter=max_iter,
#                              lambdaI = lambdaI,
#                              lambdaAR = lambdaAR,
#                              lambdaLag = lambdaLag)
#         t_stop = time.perf_counter()
#         runtime = (t_stop - t_start) * 1e6
#         rmses.append(result)
#         runtimes.append(runtime)
#
#     mean_rmse = mean(rmses)
#     mean_runtime = mean(runtimes)
#
#     print(f"RMSE : {mean_rmse}")
#     print(f"RUNTIME : {mean_runtime}")
#
#     return mean_rmse, mean_runtime