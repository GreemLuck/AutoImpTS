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
   git clone https://github.com/GreemLuck/AutoImpute.git
    ```
2. Run the setup script
    ``` bash
   cd AutoTSImpute
   ./setup.sh
    ```
3. Build the project
    ``` bash
    cmake -B build
    cmake --build build
    ```

## Execution

Once built you can use the benchmark.
``` bash
python3 autoimpts.py [arguments]
```

You can run `./build/autoimpts.py --help` to get more help

### Customize imputation scenarios

You can run an imputation like in the original ImputeBench by specifying the kind of imputation algorithm you want to run, the dataset you want to use and the missing data scenario you want to use.

``` bash
python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb
```

### Enable Parallelization

You can enable parallelization by setting the ```multi-thread``` flag.

``` bash
python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --multi-thread
```

### Scenario Configuration

You can configure the scenario variables using the ```scenv``` argument.

``` bash
python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --scenv 30,30,30
```

### Autoparameterization

You can choose an automatic parameterization technique through the ```technique``` argument

``` bash
python3 autoimpts.py --algorithm cdrec --dataset airq --scenario missb --technqiue rsearch
```


### Results
All results will be added to `_data/out` folder. 
___

## Contributors
Gremaud Lucien (lucien.gremaud@gmail.com) and Mourad Khayati (mkhayati@exascale.info)
___

## Citation
