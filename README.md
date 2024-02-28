# AutoBench:  Benchmark of Imputation Techniques in Time Series and Autoparameterization Techniques

This project presents an alternative implementation of <a href="https://github.com/eXascaleInfolab/bench-vldb20">ImputeBench</a>, a benchmark suite for time series imputation algorithms. Our version builds upon the original by retaining the basic functionalities while introducing enhanced features aimed at providing more flexibility and efficiency. These enhancements include:
- Enhanced customization of imputation algorithm hyperparameters, allowing for more tailored algorithm performance.
- Support for parallel computation, significantly reducing computation times for large datasets.
- More options for customizing missing data scenarios, offering users the ability to test algorithms under a wider range of conditions.
- Autoparameterization of the imputation algorithms, simplifying the process of finding optimal settings.
- Improved postprocessing through block realignment, ensuring better data coherence after imputation.

___

[**Prerequisites**](#prerequisites) | [**Build**](#build) | [**Execution**](#execution) | [**Contributors**](#contributors) | [**Citation**](#citation)

___

## Prerequisites

- Ubuntu 20 or Ubuntu 22 (including Ubuntu derivatives, e.g., Xubuntu) or the same distribution under WSL.
- Python 3
- Clone this repository.

___

## Build

We provide a `setup.sh` script that automates the installation of both C++ and Python packages required for this 
project. Here's how to use it:

### Steps
1. Clone this repository to your local machine
    ``` bash
   git clone https://github.com/GreemLuck/autobench.git
    ```
2. Run the setup script
    ``` bash
   cd reduced-bench
   chmod +x setup.sh
   ./setup.sh
    ```
3. Build the project
    ``` bash
    mkdir build
    cd build
    cmake ..
    make
    ```

## Execution

### Benchmark

Once built you can use the benchmark go back to the root of the project to run the program
``` bash
cd .. # If you are still in /build
./build/reduced_bench [arguments]
```

You can run `./build/reduced_bench --help` to get more help

#### Arguments

 | --alg  | --dataset  |  --scenario 
 | -------- | -------- | -------- |
 | cdrec    | airq        | MCAR         |
 | dynammo  | bafu        | MISSINGBLOCK |
 | grouse   | chlorine    |              |
 | rosl     | climate     |              |
 | softimp  | drift10     |              |
 | svdimp   | electricity |              |
 | svt      | meteo       |              |
 | stmvl    | temp        |              |
 | spirit   |             |              |
 | tenmf    |             |              |
 | tkcm     |             |              |

### Autoparam Algorithms

To run the autoparameterization algorithm
``` bash
python3 AutoParam/start_imputation_benchmarks.py [arguments]
```
You can run `python3 AutoParam/start_imputation_benchmark.py --help` to get more help

### Arguments

 | --algorithm  | --dataset  |  --scenario | --technique
 | -------- | -------- | -------- | -------- |
 | cd       | airq        | MCAR         | rsearch          |
 | dynammo  | bafu        | MISSINGBLOCK | bayes            |
 | grouse   | chlorine    |              | succ_halving     |
 | rosl     | climate     |              | swarm_particle   |
 | softimp  | drift10     |              | none             |
 | svdimp   | electricity |              |                  |
 | svt      | meteo       |              |                  |
 | stmvl    | temp        |              |                  |
 | spirit   |             |              |                  |
 | tenmf    |             |              |                  |
 | tkcm     |             |              |                  |


### Results
All results will be added to `_data` folder. 

### Execution examples

If you want to run the benchmark with on airq using cdrec and a missing block scenario you can run
``` bash
./build/reduced_bench --alg=cd --dataset=airq --scenario=MISSINGBLOCK
```
___

## Contributors

___

## Citation
