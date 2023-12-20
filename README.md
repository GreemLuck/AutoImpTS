# ReducedBench:  Benchmark of Imputation Techniques in Time Series and Autoparameterization Techniques

ReducedBench implements 11 SOTA recovery techniques for blocks of missing values in time series and evaluates their precision and runtime on various real-world time series datasets using different recovery scenarios. Technical details can be found on our 
PVLDB 2020 paper:  <a href = "http://www.vldb.org/pvldb/vol13/p768-khayati.pdf">Mind the Gap: An Experimental Evaluation of Imputation of Missing Values Techniques in Time Series </a>. The benchmark can be easity extended with new algorithms (C++/C), new datasets, new scenarios, and new autoparameteriation techniques.  

- The benchmark implements the following algorithms (in C++):
   - [CDRec](https://rdcu.be/b32bv): Scalable Recovery of Missing Blocks in Time Series with High and Low Cross-Correlations, KAIS'20
   - [DynaMMo](https://dl.acm.org/doi/10.1145/1557019.1557078): DynaMMo: mining and summarization of coevolving sequences with missing values, KDD'09
   - [GROUSE](http://proceedings.mlr.press/v51/zhang16b.html): Global Convergence of a Grassmannian Gradient Descent Algorithm for Subspace Estimation, PMLR'16
   - [ROSL](https://ieeexplore.ieee.org/abstract/document/6909890): Robust Orthonormal Subspace Learning: Efficient Recovery of Corrupted Low-Rank Matrices, CVPR'14
   - [SoftImpute](https://www.jmlr.org/papers/v11/mazumder10a.html): Spectral Regularization Algorithms for Learning Large Incomplete Matrices, JMLR'10
   - [SPIRIT](https://dl.acm.org/doi/10.5555/1083592.1083674): Streaming pattern discovery in multiple time-series, VLDB'05
   - [STMVL](https://www.ijcai.org/Proceedings/16/Papers/384.pdf): ST-MVL: Filling Missing Values in Geo-Sensory Time Series Data, IJCAI'16
   - [SVDImpute](https://academic.oup.com/bioinformatics/article/17/6/520/272365): Missing value estimation methods for DNA microarrays, BIOINFORMATICS'01
   - [SVT](https://epubs.siam.org/doi/10.1137/080738970?mobileUi=0): A Singular Value Thresholding Algorithm for Matrix Completion, SIAM J. OPTIM'10
   - [TeNMF](http://proceedings.mlr.press/v70/mei17a.html): Nonnegative Matrix Factorization for Time Series Recovery From a Few Temporal Aggregates, PMLR'17
   - [TKCM](https://openproceedings.org/2017/conf/edbt/paper-112.pdf): Continuous Imputation of Missing Values in Streams of Pattern-Determining Time Series, EDBT'17
- All the datasets used in this benchmark can be found [here](https://github.com/eXascaleInfolab/bench-vldb20/tree/master/Datasets).
- Both currently implemented recovery scenarios (miss_perc, mcar) can be found [here](https://github.com/eXascaleInfolab/bench-vldb20/blob/master/TestingFramework/README.md).
- **Remark**: Algorithms `tkcm`, and `spirit` cannot handle multiple incomplete time series. These algorithms will not produce results for the `mcar` scenario.


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
   git clone https://github.com/GreemLuck/reduced-bench.git
    ```
2. Navigate to the directory
    ``` bash
   cd reduced-bench
    ```
3. Make the script executable
    ``` bash
   chmod +x setup.sh
    ```
4. Run the script
    ``` bash
   ./setup.sh
    ```
5. Build the project
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
