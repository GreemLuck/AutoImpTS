# AutoImpTS:  Benchmark of Imputation Techniques in Time Series and Autoparameterization Techniques

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
   git clone git@github.com:GreemLuck/AutoImpTS.git
    ```
2. Run the setup script
    ``` bash
   cd AutoImpTS
   ./setup.sh
    ```
3. Build the project
    ``` bash
    cmake -B build
    cmake --build build
    ```

## Execution

### Benchmark

Once built you can use the benchmark.
``` bash
python3 autoimpts.py [arguments]
```

You can run `python autoimpts.py --help` to get more help

#### Run imputation scenarios

You can run an imputation like in the original ImputeBench by specifying the kind of imputation algorithm you want to run, the dataset you want to use and the missing data scenario you want to use.

``` bash
python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb
```

#### Parallelization

You can chose to enable parallelization by setting the ```multi-thread``` flag.

``` bash
python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --multi-thread
```

#### Scenario Configuration

You can configure the scenario variables using the ```scenv``` argument. The arguments takes a list of variables separated by comas.

| Scenario | Required Variables | Optional Variables |
| -------- | ------------------ | ------------------ |
| missb    | from, to, step     | starting row, time series index |
| mcar     | from, to, step     | missing block size, number of missing block |

##### Examples 

1. Run the cdrec algorithm on the airq dataset using the missing block scenario. Generate a single missing block matrix with 30% missing data in the 1st time series starting at 10% of the time series.
    ``` bash
    python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --scenv 30,30,30
    ```
2. Run the cdrec algorithm on the airq dataset using the missing block scenario. Generate multiple missing block matrices starting from 20% to 40% missing data increasing by 10% each steps in the 1st time series starting at 10% of the time series.
    ``` bash
    python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --scenv 20,40,10
    ```
3. Run the cdrec algorithm on the airq dataset using the missing block scenario. Generate multiple missing block matrices starting from 10% to 40% missing data increasing by 5% each steps in the 3rd time series starting at 10% of the time series.
    ``` bash
    python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --scenv 10,40,5,50,2
    ```
 4. Run the cdrec algorithm on the airq dataset using the mcar scenario. Generate multiple missing block matrices starting from 10% to 40% missing data increasing by 5% each steps with a size of 15 data points and a total number of 20 missing blocks.
    ``` bash
    python3 autoimpts.py --algorithm cdrec --dataset airq --scenario mcar --scenv 10,40,10,15,20
    ```

#### Autoparameterization

You can chose a automatic parameterization technique through the ```technique``` argument

``` bash
python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --technqiue rsearch
```


### Results
All results will be added to `_data/out` folder. 
___

## Contributors

___

## Citation
