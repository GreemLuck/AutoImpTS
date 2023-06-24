import sqlite3
import matplotlib.pyplot as plt
import numpy as np
import BayesOpt
import ts_algorithms
import plotSH
import random_search
import random
import swarm_particle
import os
import shutil
from pathlib import Path


dataset = "airq"
default_value = {"win_size": 6, "truncation": 3, "lambda":100}

bounds = {"truncation":(1,10,1), "tolerance": (1,10,1), "max_iter": (1,1000,1)}

bounds_sh = {"truncation":(1, 10,1)}
sample_size = 8

bounds_bayes = {"truncation":(1,10), "tolerance": (1, 10), "max_iter":(1,1000)}
exploration = 5
exploitation = 7

n_particles = 4
n_iter = 20

swarmp_flag = True
default_flag = False
do_sh = False
rs_flag = False

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
    if default_flag:
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
    result = BayesOpt.main(alg_name, dataset=dataset, exploration=exploration, exploitation=exploitation, bounds=bounds_bayes, verbose=True)
    _, _, runs, params = alg_func(**result["params"], dataset=dataset, verbose=True)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1,  ax2), ticks, rmses, runtimes, params, name="Bayes")

    # Random Search
    if rs_flag:
        result = random_search.rs_execute(algorithm=alg_name, dataset=dataset, sample_size=sample_size,
                               distribution=bounds)
        _, _, runs, params = alg_func(**result, dataset=dataset, verbose=True)
        ticks, rmses, runtimes = getdata(runs)
        plot_data((ax1,  ax2), ticks, rmses, runtimes, params, name="Random Search")

    # Swarm Particle
    if swarmp_flag:
        result, _ = swarm_particle.ps_execute(algorithm=alg_name, dataset=dataset, distribution=bounds, sample_size=n_particles, n_iter=n_iter)
        _, _, runs, params = alg_func(**result, dataset=dataset, verbose=True)
        ticks, rmses, runtimes = getdata(runs)
        plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="SwarmP")

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
    fig.savefig(f"Graphs/compare/{alg_name}_compare_bayes_{dataset}")
    print(f"Saved in Graphs/compare/{alg_name}_compare_bayes_{dataset}")


def bayes_sp(alg_func, alg_name, dataset):
    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(15, 8), dpi=200)

    result = BayesOpt.main(alg_name, dataset=dataset, exploration=exploration, exploitation=exploitation, bounds=bounds_bayes, verbose=False)
    _, _, runs, params = alg_func(**result["params"], dataset=dataset, verbose=False)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="Bayes#1")

    truncation = params["truncation"]
    max_iter = 1000
    tolerance = 10

    _, max_runtime, *_ = alg_func(truncation=truncation, tolerance=tolerance, max_iter=max_iter, dataset=dataset, verbose=False)
    result = BayesOpt.main(alg_name, dataset=dataset, exploration=10, exploitation=20,
                           bounds={"tolerance":(1,10), "max_iter":(1,1000)}, verbose=False, alg_func=alg_func,
                           process_runtime=True, max_runtime=max_runtime, fix_truncation=truncation)
    _, _, runs, params = alg_func(truncation=truncation, **result["params"], dataset=dataset, verbose=False)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="Bayes#2")

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
    fig.savefig(f"Graphs/compare/{alg_name}_compare_bayes_{dataset}")
    print(f"Saved in Graphs/compare/{alg_name}_compare_bayes_{dataset}")


def bayes_to_swarm(alg_func, alg_name, dataset):
    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(15, 8), dpi=200)

    result = BayesOpt.main(alg_name, dataset=dataset, exploration=exploration, exploitation=exploitation, bounds=bounds_bayes, verbose=False)
    _, _, runs, params = alg_func(**result["params"], dataset=dataset, verbose=False)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="Bayes#1")

    truncationBayes = params["truncation"]

    result, _ = swarm_particle.ps_execute(algorithm=alg_name, dataset=dataset, distribution=bounds, sample_size=n_particles, n_iter=n_iter)
    _, _, runs, params = alg_func(**result, dataset=dataset, verbose=True)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="SwarmP")

    truncationSwarm = params["truncation"]

    result, _ = swarm_particle.ps_execute(algorithm=alg_name, dataset=dataset,
                                          distribution={"max_iter": (1, 1000, 1),
                                                        "tolerance": (1, 10, 1)},
                                          sample_size=n_particles, n_iter=n_iter,
                                          fixed_param={"truncation": truncationBayes})
    _, _, runs, params = alg_func(truncation=truncationBayes, **result, dataset=dataset, verbose=False)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="Bayes -> SwarmP")

    result = BayesOpt.main(alg_name, dataset=dataset, exploration=10, exploitation=20,
                           bounds={"tolerance":(1,10), "max_iter":(1,1000)}, verbose=False, alg_func=alg_func,
                           process_runtime=True, fix_truncation=truncationSwarm)
    _, _, runs, params = alg_func(truncation=truncationSwarm, **result["params"], dataset=dataset, verbose=False)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="SwarmP -> Bayes")

    ax1.set_title("rmse")
    ax1.set_xlabel("missing block size (permil)")
    ax1.set_ylabel("rmse")
    ax1.set_xticks(ticks)
    ax2.set_title("runtime")
    ax2.set_xlabel("missing block size (permil)")
    ax2.set_ylabel("runtime (μs)")
    fig.suptitle(f"Algorithm: {alg_name}\n"
                 f"Dataset: {dataset}\n"
                 f"Comparing the simple use of Bayes and SwarmP vs a combination of the two")
    ax1.legend()
    ax2.legend()
    fig.tight_layout()
    fig.savefig(f"Graphs/compare/{alg_name}_compare_bayestoswarm_{dataset}")
    print(f"Saved in Graphs/compare/{alg_name}_compare_bayestoswarm_{dataset}")

def misaligned_compare(alg_name, dataset):
    alg_func, _ = ts_algorithms.get_algorithm(alg_name)

    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(15, 8), dpi=200)

    _, _, runs, params = alg_func(misaligned=False, dataset=dataset, verbose=False)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="Normal")

    _, _, runs, params = alg_func(misaligned=True, dataset=dataset, verbose=False)
    ticks, rmses, runtimes = getdata(runs)
    plot_data((ax1, ax2), ticks, rmses, runtimes, params, name="Misalignment reduction")

    ticks = [100,200,300,400,500,600,700,800]
    for t in ticks:
        misaligned_compare_matrix(alg_name, dataset, t)

    ax1.set_title("rmse")
    ax1.set_xlabel("missing block size (permil)")
    ax1.set_ylabel("rmse")
    ax1.set_xticks(ticks)
    ax2.set_title("runtime")
    ax2.set_xlabel("missing block size (permil)")
    ax2.set_ylabel("runtime (μs)")
    fig.suptitle(f"Algorithm: {alg_name}\n"
                 f"Dataset: {dataset}\n"
                 f"Comparing with and without misalignment reduction")
    ax1.legend()
    ax2.legend()
    fig.tight_layout()
    os.makedirs(f"Graphs/misaligned/rmse_compare/{alg_name}/", exist_ok=True)
    fig.savefig(f"Graphs/misaligned/rmse_compare/{alg_name}/rmse_compare_{alg_name}_{dataset}.png")
    print(f"Saved in Graphs/misaligned/rmse_compare/{alg_name}/rmse_compare_{alg_name}_{dataset}.png")

def misaligned_compare_matrix(alg_name, dataset, ticks):
    folder_name = dataset2folder(dataset)
    reference_filepath = f"Datasets/real_world/{folder_name}/{dataset}_normal.txt"
    cdrec_misaligned_filepath = f"_data/misaligned/data_{ticks}.txt"
    cdrec_standard_filepath = f"_data/data_{ticks}.txt"

    reference = np.loadtxt(reference_filepath)
    cdrec_standard = np.loadtxt(cdrec_standard_filepath)
    cdrec_misaligned = np.loadtxt(cdrec_misaligned_filepath)

    print(f"Reference : {reference.size}")
    print(f"Standard : {cdrec_standard.size}")
    print(f"Realigned : {cdrec_misaligned.size}")

    x = np.arange(1, reference.T[0].size+1, 1)
    blocksize = int(reference.T[0].size * (ticks/1000))
    starting_index = int(reference.T[0].size * 0.05)
    ending_index = starting_index + blocksize

    rows = 1
    columns = 2

    fig = plt.figure(figsize=(13,6))
    plt.title(f"{alg_name} - {dataset}")
    fig.add_subplot(rows, columns, 1)
    plt.plot(x[starting_index-5:ending_index+5],cdrec_standard.T[0][starting_index-5:ending_index+5], linewidth=0.6, label='standard')
    plt.plot(x[starting_index-5:ending_index+5],reference.T[0][starting_index-5:ending_index+5], '--', linewidth=0.6, label='reference')
    plt.legend()

    fig.add_subplot(rows , columns, 2)
    plt.plot(x[starting_index-5:ending_index+5],cdrec_misaligned.T[0][starting_index-5:ending_index+5], linewidth=0.6, label='realigned')
    plt.plot(x[starting_index-5:ending_index+5],reference.T[0][starting_index-5:ending_index+5], '--', linewidth=0.6, label='reference')
    plt.plot([x[starting_index],x[ending_index-1]], [cdrec_standard.T[0][starting_index], cdrec_standard.T[0][ending_index-1]], linewidth=0.3, label='y')
    plt.plot([x[starting_index-1],x[ending_index]], [cdrec_standard.T[0][starting_index-1], cdrec_standard.T[0][ending_index]], linewidth=0.3, label='z')

    plt.legend()
    os.makedirs(f'Graphs/misaligned/matrix_compare/{alg_name}/{dataset}', exist_ok=True)
    plt.savefig(f'Graphs/misaligned/matrix_compare/{alg_name}/{dataset}/matrix_compare_{alg_name}_{dataset}_{ticks}.png', dpi=400)

def dataset2folder(dataset):
    if dataset == "airq":
        return "air_quality"
    if dataset == "drift10":
        return "drift"
    return dataset

def compare_real_repaired(dataset):
    folder_name = dataset2folder(dataset)
    reference_filepath = f"Datasets/real_world/{folder_name}/{dataset}_normal.txt"
    cdrec_misaligned_filepath = f"_data/misaligned/data_100.txt"

    reference = np.loadtxt(reference_filepath)
    cdrec_misaligned = np.loadtxt(cdrec_misaligned_filepath)
    x = np.arange(1, reference.T[0].size+1, 1)

    plt.figure(figsize=(8,6))
    plt.title(f"svt - {dataset}")
    plt.plot(x, reference, '--', linewidth=0.6, label='reference')
    plt.plot(x, cdrec_misaligned, label='misaligned')

    plt.legend()
    plt.savefig(f'Graphs/misaligned/matrix_compare/matrix_compare_svt_{dataset}.png', dpi=400)

def store_data(alg, dataset, folder_name, misaligned=False, auto_alg='none', scenario="MISSINGBLOCK"):
    alg_func, alg_bounds, alg_bounds_step = ts_algorithms.get_algorithm(alg)
    data_path = '_data/out/'
    store_path = f'Graphs/store_data/{folder_name}/{alg}/{dataset}/'

    if auto_alg == 'none':
        alg_func(dataset=dataset, misaligned=misaligned, scenario=scenario)
    elif auto_alg == 'random_search':
        print(f"Runing Random Sreach with {alg} on {dataset} ...")
        result = random_search.rs_execute(algorithm=alg, dataset=dataset, sample_size=sample_size,
                                          distribution=alg_bounds_step, fixed_param={"scenario": scenario})
        alg_func(**result, dataset=dataset, verbose=True, misaligned=misaligned, scenario=scenario)
    elif auto_alg == 'successive_havling':
        print(f"Running Successive Halving with {alg} on {dataset} ...")
        result = plotSH.sh_execute(algorithm=alg, dataset=dataset, sample_size=sample_size,
                                   distribution=alg_bounds_step,
                                   resource_name='max_iter',
                                   resource_max=100, resource_min=5, keep_losers=False)
        alg_func(**result, dataset=dataset, verbose=True, misaligned=misaligned)
    elif auto_alg == 'bayes':
        print(f"Running Bayesian Optimization with {alg} on {dataset} ...")
        result = BayesOpt.main(alg, dataset=dataset, exploration=exploration, exploitation=exploitation, bounds=alg_bounds, verbose=True, scenario=scenario)
        alg_func(**result["params"], dataset=dataset, verbose=True, misaligned=misaligned, scenario=scenario)
    elif auto_alg == 'swarm':
        print(f"Running Particle Swarm Optimization with {alg} on {dataset} ...")
        result, _ = swarm_particle.ps_execute(algorithm=alg, dataset=dataset, distribution=alg_bounds_step,sample_size=n_particles, n_iter=n_iter, fixed_param={"verbose": True, "scenario": scenario})
        alg_func(**result, dataset=dataset, verbose=True, misaligned=misaligned, scenario=scenario)
    else:
        print("Error with auto_param name")
        return

    Path(store_path).mkdir(parents=True, exist_ok=True)



    copy_folder_contents(data_path, store_path)
    print(f'Copied {alg} data in {folder_name} folder')

def plot_postprocess(algname, dataset):
    folder_name = dataset2folder(dataset)
    reference = np.loadtxt(f"Datasets/real_world/{folder_name}/{dataset}_normal.txt")

    save_path = f"Graphs/tables/post_processing/{algname}/"
    Path(save_path).mkdir(parents=True, exist_ok=True)

    ticks = [100, 200, 300, 400, 500, 600, 700, 800]
    default_rmse_array = []
    standard_rmse_array = []
    wider_rmse_array = []

    default_x_standard = []
    default_x_wider = []

    for t in ticks:
        default = np.loadtxt(f"Graphs/store_data/default/{algname}/{dataset}/data_{t}.txt")
        standard = np.loadtxt(f"Graphs/store_data/postprocess_standard/{algname}/{dataset}/data_{t}.txt")
        wider = np.loadtxt(f"Graphs/store_data/postprocess_wider/{algname}/{dataset}/data_{t}.txt")

        reference_block = getBlock(reference, t)
        default_block = getBlock(default, t)
        standard_block = getBlock(standard, t)
        wider_block = getBlock(wider, t)

        size = int(len(default.T[0]) * (t/1000))

        default_rmse = np.sqrt(np.sum((reference_block - default_block)**2)/size)
        standard_rmse = np.sqrt(np.sum((reference_block - standard_block)**2)/size)
        wider_rmse = np.sqrt(np.sum((reference_block - wider_block)**2)/size)

        default_rmse_array.append(default_rmse)
        standard_rmse_array.append(standard_rmse)
        wider_rmse_array.append(wider_rmse)

        default_x_standard.append((1 - (standard_rmse/default_rmse))*100)
        default_x_wider.append((1 - (wider_rmse/default_rmse))*100)

    float_formatter1 = lambda n: "{:.3f}".format(n)
    rmsefunc = np.vectorize(float_formatter1)

    percent_formatter = lambda n: "{:.1f}%".format(n)
    percentfunc = np.vectorize(percent_formatter)

    default_rmse_array = rmsefunc(default_rmse_array)
    standard_rmse_array = rmsefunc(standard_rmse_array)
    wider_rmse_array = rmsefunc(wider_rmse_array)
    default_x_wider = percentfunc(default_x_wider)
    default_x_standard = percentfunc(default_x_standard)

    fig, ax = plt.subplots()
    ax.set_title(f'{algname} on {dataset} post processing methods comparison')
    ax.axis('off')
    the_table = ax.table(cellText=np.array([default_rmse_array,
                                            standard_rmse_array, default_x_standard,
                                            wider_rmse_array, default_x_wider]).T, loc='center',
                         colLabels=["default rmse", "standard rmse", "+%", "wider rmse", "+%"],
                         rowLabels=ticks)
    the_table.set_fontsize(10)
    the_table.scale(1,1)
    fig.tight_layout()
    plt.savefig(save_path + f"post_process_{dataset}", dpi=400)
    print("Saved: " + save_path + f"post_process_{dataset}")

def getBlock(matrix, blockSize):
    first_column = matrix.T[0]
    size = int(len(first_column) * (blockSize/1000))
    start = int( len(first_column)*0.05)
    block = first_column[start:(start+size)]
    return block

def copy_folder_contents(src_folder, dest_folder):
    """
    Copies the contents of the source folder to the destination folder.

    :param src_folder: str, source folder path
    :param dest_folder: str, destination folder path
    """
    if not os.path.exists(src_folder):
        print(f"Source folder '{src_folder}' does not exist.")
        return

    if not os.path.exists(dest_folder):
        print(f"Destination folder '{dest_folder}' does not exist.")
        return

    for item in os.listdir(src_folder):
        src_path = os.path.join(src_folder, item)
        dest_path = os.path.join(dest_folder, item)

        if os.path.isfile(src_path):
            shutil.copy2(src_path, dest_path)
        elif os.path.isdir(src_path):
            shutil.copytree(src_path, dest_path)



if __name__ == "__main__":
    # misaligned_compare_matrix('cdrec', 'airq', 100)
    #misaligned_compare_matrix('chlorine')
    datasets = ["chlorine"]
    algorithms = ["cdrec", "svt", "stmvl", "spirit", "grouse", "nnmf", "rosl", "itersvd", "dynammo", "softimpute", "tkcm"]

    for a in algorithms:
        store_data(a, "chlorine", "post_processing", misaligned=True, auto_alg="none")
