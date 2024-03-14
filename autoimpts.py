import argparse
import os
import sys
import csv
import time
import random

sys.path.insert(1, "./AutoParam")
import ts_algorithms
import random_search
import BayesOpt
import swarm_particle
import plotSH

separation = "-----------------------------------"

header = ["Algorithm", "Dataset", "Scenario", "Scenario_Variables", "Autoparam", "Autoparam_Settings", "Average_RMSE", "Optimal_Param", "Total_Runtime", "Seed", "Multi_Thread"]

def main(args):    
    file_exists = os.path.isfile(args.savepath) and os.path.getsize(args.savepath) > 0

    row = [args.alg, args.dataset, args.scen, args.scenv, args.technique]

    alg_func, alg_bounds, alg_bounds_step = ts_algorithms.get_algorithm(args.alg)

    if args.scen:
        if args.scen == "mcar":
            args.scen = "MCAR"
        elif args.scen == "missb":
            args.scen = "MISSINGBLOCK"

    fixed_param = {"verbose": args.verbose, "scenario": args.scen, "parallel": args.multi_thread, "scenv": args.scenv}

    if args.seed:
        random.seed(args.seed)

    if args.scenv != "" and args.technique == "succ_halving":
        raise ValueError("The scenario variable cannot be altered for the Successive Halving technique.")

    print(separation)
    print(f"Algorithm: {args.alg}")
    print(f"Dataset: {args.dataset}")
    print(f"Scenario: {args.scen}")
    print(f"Technique: {args.technique}")
    print(separation)

    start_time = time.time()

    if args.technique == 'rsearch':
        row.append({"sample_size": args.sample_size})
        print(f"Runing Random Sreach with {args.alg} on {args.dataset} ...")
        params, mean_rmse = random_search.rs_execute(algorithm=args.alg, dataset=args.dataset, sample_size=args.sample_size,
                                          distribution=alg_bounds_step, fixed_param=fixed_param)
        row.append(mean_rmse)
        row.append(params)
        # mean_rmse is just floating point, result is {param1: xxx, param2: xxx}
    elif args.technique == 'bayes':
        row.append({"exploration": args.exploration, "exploitation": args.exploitation})
        print(f"Running Bayesian Optimization with {args.alg} on {args.dataset} ...")
        result = BayesOpt.main(args.alg, 
                               dataset=args.dataset, 
                               exploration=args.exploration, 
                               exploitation=args.exploitation, 
                               bounds=alg_bounds, 
                               verbose=args.verbose, 
                               scenario=args.scen,
                               multithread=args.multi_thread)
        row.append(-result["target"])
        row.append(result["params"])
        params = result["params"]
        mean_rmse = -result["target"]
        # {target: -meanrmse, params{param1: xxx, param2: xxx}}
    elif args.technique == 'swarm_particle':
        row.append({"niter": args.niter, "nparticles": args.nparticles})
        print(f"Running Particle Swarm Optimization with {args.alg} on {args.dataset} ...")
        params, mean_rmse = swarm_particle.ps_execute(algorithm=args.alg, 
                                                 dataset=args.dataset, 
                                                 distribution=alg_bounds_step,
                                                 sample_size=args.nparticles,
                                                 n_iter=args.niter,
                                                 fixed_param=fixed_param)
        row.append(mean_rmse)
        row.append(params)
        # {param1: xxx, param2: xxx}, floating point rmse
    elif args.technique == 'succ_halving':
        row.append({'sample_size': args.sample_size})
        print(f"Runing Successive Halving with {args.alg} on {args.dataset} ... ")
        rmseRuntime, params, _ = plotSH.sh_execute(algorithm=args.alg, 
                                                   dataset=args.dataset, 
                                                   sample_size=args.sample_size, 
                                                   distribution=alg_bounds_step,
                                                   verbose=args.verbose,
                                                   multi_thread=args.multi_thread,
                                                   scenario=args.scen)
        mean_rmse, runtime, params = alg_func(dataset=args.dataset, scenario=args.scen, **params)
        row.append(mean_rmse)
        row.append(params)
    elif args.technique == 'none':
        row.append({})
        print(f"Running {args.alg} on {args.dataset} with default values ...")
        mean_rmse, runtime, params = alg_func(dataset=args.dataset, scenario=args.scen, verbose=args.verbose, parallel=args.multi_thread, scenv=args.scenv)
        row.append(mean_rmse)
        row.append(params)

    end_time = time.time()
    row.append(end_time - start_time)

    if args.seed:
        row.append(args.seed)
    else:
        row.append("-")

    row.append(args.multi_thread)

    with open(args.savepath, 'a', newline='', encoding='utf-8') as csvfile:
        csvwriter = csv.writer(csvfile)

        if not file_exists:
            csvwriter.writerow(header)

        csvwriter.writerow(row)

    print(separation)
    print(f"Optimal Parameters: {params}")
    print(f"Average RMSE: {mean_rmse}")

        

if __name__ == "__main__":
    build_dir = "./build"
    executable_name = "reduced_bench"
    full_path = os.path.join(build_dir, executable_name)

    print("Verifying benchmark binaries ... ")
    if os.path.exists(full_path) and os.path.isfile(full_path):
        print("OK")
    else:
        print(f"The executable {executable_name} does not exist in {build_dir}. Build the program first by following the README.md instructions")
        sys.exit(1)

    parser = argparse.ArgumentParser(description='Run benchmarks for time series imputation.')

    parser.add_argument(
        "--alg",
        choices=['cdrec','svt','nnmf', 'stmvl', 'grouse', 'rosl', 'softimpute', 'dynammo', 'tkcm'],
        required=True,
        help="Algorithm to use for imputation."
    )

    parser.add_argument(
        "-d",
        "--dataset",
        choices=['airq', 'drift10', 'gas', 'climate', 'temp', 'baffu', 'chlorine', 'electricity', 'meteo'],
        required=True,
        help="Dataset to use."
    )

    parser.add_argument(
        "--scen",
        choices=['mcar', 'missb'],
        required=True,
        help='Missing data scenario to use.'
    )

    parser.add_argument(
        "--technique",
        choices=['rsearch', 'bayes', 'swarm_particle', 'succ_halving'],
        default='none',
        help="Autoparametrization technique to use. Default is none."
    )

    parser.add_argument(
        "--sample_size",
        type=int,
        default=15,
        help="Sample size for random search or successive halving. Default is 15."
    )

    parser.add_argument(
        "--exploration",
        type=int,
        default=5,
        help="Exploration factor for Bayesian Optimization. Default is 2.0."
    )

    parser.add_argument(
        "--exploitation",
        type=int,
        default=10,
        help="Exploitation factor for Bayesian Optimization. Default is 3.0."
    )

    parser.add_argument(
        "--nparticles",
        type=int,
        default=5,
        help="Number of particles for Particle Swarm Optimization. Default is 5."
    )

    parser.add_argument(
        "--niter",
        type=int,
        default=5,
        help="Number of iterations for Particle Swarm Optimization. Default is 5."
    )

    parser.add_argument(
        "--savepath",
        type=str,
        default="_data/data_autoparam.csv",
        help="Save path for where the data will end. (CSV)"
    )

    parser.add_argument(
        "--seed",
        type=int,
        help="Set the seed for randomness."
    )

    parser.add_argument(
        "--multi-thread",
        action='store_true',
        help="Allow multi-threading for the algorithms. Default is False."
    )

    parser.add_argument(
        "--verbose",
        action='store_true',
        help="Print the output of the algorithm."
    )

    parser.add_argument(
        "--scenv",
        type=str,
        default="",
        help="Scenario variable in brackets. Example: --scenv='10,80,10'"
    )

    args = parser.parse_args()
    main(args)
