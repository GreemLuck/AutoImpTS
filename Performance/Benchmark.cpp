//
// Created by Zakhar on 16/03/2017.
//

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <chrono>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

#include "Benchmark.h"
#include "../Database/SQL.h"
#include <cassert>
#include <filesystem>

#include "../Algorithms/CDMissingValueRecovery.h"
#include "../Algorithms/TKCM.h"
#include "../Algorithms/ST_MVL.h"
#include "../Algorithms/SPIRIT.h"
#include "../Algorithms/GROUSE.h"
#include "../Algorithms/NMFMissingValueRecovery.h"
#include "../Algorithms/DynaMMo.h"
#include "../Algorithms/SVT.h"
#include "../Algorithms/ROSL.h"
#include "../Algorithms/IterativeSVD.h"
#include "../Algorithms/SoftImpute.h"

std::string currentPath;

using namespace Algorithms;

namespace Performance
{
void generateLatLongFile(std::string destination, arma::mat &mat);
std::pair<double, u_int64_t> run(arma::mat &mat, std::string input, settings &set);
std::vector<std::tuple<int, int, int>> find_missing_blocks(const arma::mat& B);
void enlarge_missing_blocks(arma::mat& missing);

void verifyRecovery(arma::mat &mat)
{
    for (uint64_t j = 0; j < mat.n_cols; ++j)
    {
        for (uint64_t i = 0; i < mat.n_rows; ++i)
        {
            if (std::isnan(mat.at(i, j)))
            {
                mat.at(i, j) = std::sqrt(std::numeric_limits<double>::max() / 100000.0);
            }
        }
    }
}

void set_params(std::vector<double> &params, std::vector<double> &default_params, size_t max){
    if(params.empty())
        params = default_params;

    if(params.size() > max)
        throw std::out_of_range("Too many parameters");

    // Replace nan by default values
    for(std::size_t i = 0; i < params.size(); ++i){
        if(isnan(params[i]))
            params[i] = default_params[i];
    }
}

// CDREC
int64_t Recovery_CD(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation, threshold, maxIter;

    truncation = params[Parameters::CD::TRUNCATION];
    threshold = params[Parameters::CD::TOLERANCE];
    maxIter = params[Parameters::CD::MAX_ITER];

    // Local
    int64_t result;
    CDMissingValueRecovery rmv(mat, maxIter, threshold);
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    rmv.setReduction(truncation);
    rmv.disableCaching = false;
    rmv.useNormalization = false;
    
    begin = std::chrono::steady_clock::now();
    rmv.autoDetectMissingBlocks();
    rmv.performRecovery(truncation == mat.n_cols);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (CDRec): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_TKCM(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation, d;

    truncation = params[Parameters::TKCM::TRUNCATION];
    d = params[Parameters::TKCM::D];

    // Local
    int64_t result;
    Algorithms::TKCM tkcm(mat, truncation, d);
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // Recovery

    begin = std::chrono::steady_clock::now();
    tkcm.performRecovery();
    end = std::chrono::steady_clock::now();

    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (TKCM): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_ST_MVL(arma::mat &mat, std::map<std::string, double> &params)
{
    double alpha, beta, winSize;

    alpha = params[Parameters::ST_MVL::ALPHA];
    beta = params[Parameters::ST_MVL::GAMMA];
    winSize = params[Parameters::ST_MVL::WIN_SIZE];

    // Local
    int64_t result;
    const std::string latlong = "latlong_placeholder.txt";
    std::ifstream f(latlong.c_str());
    if(!f.good())
        generateLatLongFile(latlong, mat);
    ST_MVL stmvl(mat, latlong, alpha, beta, winSize);

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // Recovery

    begin = std::chrono::steady_clock::now();
    stmvl.Run(true);
    end = std::chrono::steady_clock::now();

    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std:: cout << "Time (STMVL): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}


int64_t Recovery_SPIRIT(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation, winSize, lambda;

    truncation = params[Parameters::SPIRIT::TRUNCATION];
    winSize = params[Parameters::SPIRIT::WIN_SIZE];
    lambda = params[Parameters::SPIRIT::LAMBDA];

    // Local
    int64_t result;
    
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    SPIRIT::doSpirit(mat, truncation, winSize, lambda);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SPIRIT): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_GROUSE(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation;

    truncation = params[Parameters::GROUSE::TRUNCATION];

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // Recovery

    mat = mat.t();

    begin = std::chrono::steady_clock::now();
    GROUSE::doGROUSE(mat, truncation);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (GROUSE): " << result << std::endl;
    
    mat = mat.t();
    
    verifyRecovery(mat);
    return result;
}

    int64_t Recovery_NNMF(arma::mat &mat, std::map<std::string, double> &params)
    {
        double truncation, threshold, maxIter;

        truncation = params[Parameters::NNMF::TRUNCATION];
        threshold = params[Parameters::NNMF::TOLERANCE];
        maxIter = params[Parameters::NNMF::MAX_ITER];

        // Local
        int64_t result;

        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;

        // Recovery
        begin = std::chrono::steady_clock::now();
        NMFMissingValueRecovery::doNMFRecovery(mat, truncation, threshold, maxIter);
        end = std::chrono::steady_clock::now();

        result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        std::cout << "Time (TeNMF): " << result << std::endl;

        verifyRecovery(mat);
        return result;
    }

int64_t Recovery_DynaMMo(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation, maxIter;

    truncation = params[Parameters::DYNAMMO::TRUNCATION];
    maxIter = params[Parameters::DYNAMMO::MAX_ITER];

    // Local
    int64_t result;
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;


    // Recovery
    mat = mat.t();

    begin = std::chrono::steady_clock::now();
    DynaMMo::doDynaMMo(mat, truncation, maxIter, true);
    end = std::chrono::steady_clock::now();

    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (DynaMMo): " << result << std::endl;

    mat = mat.t();

    verifyRecovery(mat);
    return result;
}

int64_t Recovery_SVT(arma::mat &mat, std::map<std::string, double> &params)
{
    double maxIter, threshold, tauscale;

    maxIter = params[Parameters::SVT::MAX_ITER];
    threshold = params[Parameters::SVT::TOLERANCE];
    tauscale = params[Parameters::SVT::TAUSCALE];

    // Local
    int64_t result;
    
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // double tauScale = std::stod(xtra);

    // Recovery
    begin = std::chrono::steady_clock::now();
    SVT::doSVT(mat, tauscale, threshold, maxIter);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SVT): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_ROSL(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation, threshold, maxIter;

    truncation = params[Parameters::ROSL::TRUNCATION];
    threshold = params[Parameters::ROSL::TOLERANCE];
    maxIter = params[Parameters::ROSL::MAX_ITER];

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    ROSL::ROSL_Recovery(mat, truncation,threshold,maxIter);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (ROSL): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_IterativeSVD(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation, threshold, maxIter;

    truncation = params[Parameters::SVD::TRUNCATION];
    threshold = params[Parameters::SVD::TOLERANCE];
    maxIter = params[Parameters::SVD::MAX_ITER];

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    IterativeSVD::recoveryIterativeSVD(mat, truncation, threshold, maxIter);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SVDImpute): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_SoftImpute(arma::mat &mat, std::map<std::string, double>  &params)
{
    double truncation, threshold, maxIter;

    truncation = params[Parameters::SOFTIMPUTE::TRUNCATION];
    threshold = params[Parameters::SOFTIMPUTE::TOLERANCE];
    maxIter = params[Parameters::SOFTIMPUTE::MAX_ITER];

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    SoftImpute::doSoftImpute(mat, truncation, threshold, maxIter);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SoftImpute): " << result << std::endl;
    
    verifyRecovery(mat);

    return result;
}

// depreciated
double Recovery_TRMF(std::string &dataset, settings &set, u_int64_t tick){
    double max_iter, tolerance, lambdaI, lambdaAR, lambdaLag, rmse;

    max_iter = set.params.find(Parameters::TRMF::MAX_ITER) != set.params.end()
            ? set.params[Parameters::TRMF::MAX_ITER] : 40;
/*    tolerance = set.params.find(Parameters::TRMF::TOLERANCE) != set.params.end()
            ? set.params[Parameters::TRMF::TOLERANCE] : 0.5;*/
    lambdaI = set.params.find(Parameters::TRMF::LAMBDA_I) != set.params.end()
            ? set.params[Parameters::TRMF::LAMBDA_I] : 0.7;
    lambdaAR = set.params.find(Parameters::TRMF::LAMBDA_AR) != set.params.end()
            ? set.params[Parameters::TRMF::LAMBDA_AR] : 125;
    lambdaLag = set.params.find(Parameters::TRMF::LAMBDA_LAG) != set.params.end()
            ? set.params[Parameters::TRMF::LAMBDA_LAG] : 4;



    // Python interpreter
    Py_Initialize();

    // Add trmfpy module to path
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *pathModule = PyObject_GetAttrString(sys, "path");
    PyList_Append(pathModule, PyUnicode_FromString("./Algorithms"));

    PyObject* ModuleString = PyUnicode_FromString((char*) "trmfpy");
    if (!ModuleString) {
        PyErr_Print();
        printf("Error formating python script\n");
    }

    PyObject* Module = PyImport_Import(ModuleString);
    if (!Module) {
        PyErr_Print();
        printf("Error importing python script\n");
    }

    // Prepare args
    PyObject * Dataset = PyUnicode_FromString(dataset.c_str());
    PyObject * Ticks = PyLong_FromLong(tick);
    PyObject * Max_Iter = PyLong_FromLong(max_iter);
    PyObject * Tolerance = PyFloat_FromDouble(tolerance);
    PyObject * LambdaI = PyFloat_FromDouble(lambdaI);
    PyObject * LambdaAR = PyFloat_FromDouble(lambdaAR);
    PyObject * LambdaLag = PyFloat_FromDouble(lambdaLag);
    PyObject * Args = PyTuple_Pack(7, Dataset, Ticks, Max_Iter, Tolerance, LambdaI, LambdaAR, LambdaLag);

    if(!PyTuple_Check(Args))
        std::cerr << "Error with python arguments" << std::endl;

    // Get and run main function
    PyObject * Dict = PyModule_GetDict(Module);
    PyObject * Func = PyDict_GetItemString(Dict, "main");

    PyObject * Result = PyObject_CallObject(Func, Args);
    if(!Result){
        PyErr_Print();
        std::cerr << "Error during TRMF runtime" << std::endl;
    }

    if(PyNumber_Check(Result))
        rmse = PyFloat_AsDouble(Result);
    else
        PyErr_Print();

    Py_DECREF(Module);
    Py_DECREF(Dict);
    Py_DECREF(Func);
    Py_DECREF(Args);

    Py_Finalize();

    set.params[Parameters::TRMF::MAX_ITER] = max_iter;
    // set.params[Parameters::TRMF::TOLERANCE] = tolerance;
    set.params[Parameters::TRMF::LAMBDA_I] = lambdaI;
    set.params[Parameters::TRMF::LAMBDA_AR] = lambdaAR;
    set.params[Parameters::TRMF::LAMBDA_LAG] = lambdaLag;
    return rmse;
}

double Recovery_TRMF2(std::string &dataset, settings &set, u_int64_t tick){
    double max_iter, k, lambdaI, lambdaAR, lambdaLag, rmse;

    lambdaI = set.params.find(Parameters::TRMF::LAMBDA_I) != set.params.end()
              ? set.params[Parameters::TRMF::LAMBDA_I] : 0.7;
    lambdaAR = set.params.find(Parameters::TRMF::LAMBDA_AR) != set.params.end()
               ? set.params[Parameters::TRMF::LAMBDA_AR] : 125;
    lambdaLag = set.params.find(Parameters::TRMF::LAMBDA_LAG) != set.params.end()
                ? set.params[Parameters::TRMF::LAMBDA_LAG] : 4;
    k = set.params.find(Parameters::TRMF::K) != set.params.end()
            ? set.params[Parameters::TRMF::K] : 5;

    currentPath = std::filesystem::current_path();
    std::ostringstream oss;
    oss << "python3 " << currentPath << "/Algorithms/trmfpy.py dataset=" << set.dataset <<
        " k=" << k <<
        " lambdaI=" << lambdaI <<
        " lambdaAR=" << lambdaAR <<
        " lambdaLag=" << lambdaLag <<
        " ticks=" << tick;
    std::string cmd = oss.str();
    std::cout << cmd << std::endl;
    system(cmd.c_str());
    return 0;
}


int64_t Recovery(arma::mat &mat, const std::string &algorithm, std::map<std::string, double> &params){
    if (algorithm == "cd")
    {
        if(params.find(Parameters::CD::TRUNCATION) == params.end())
            params[Parameters::CD::TRUNCATION] = 3;

        if(params.find(Parameters::CD::TOLERANCE) == params.end())
            params[Parameters::CD::TOLERANCE] = 1e-6;

        if(params.find(Parameters::CD::MAX_ITER) == params.end())
            params[Parameters::CD::MAX_ITER] = 100;

        return Recovery_CD(mat, params);
    }
    else if (algorithm == "tkcm")
    {
        if(params.find(Parameters::TKCM::TRUNCATION) == params.end())
            params[Parameters::TKCM::TRUNCATION] = 1;

        if(params.find(Parameters::TKCM::D) == params.end())
            params[Parameters::TKCM::D] = 1;

        return Recovery_TKCM(mat, params);
    }
    else if (algorithm == "st-mvl")
    {
        if(params.find(Parameters::ST_MVL::ALPHA) == params.end())
            params[Parameters::ST_MVL::ALPHA] = 2.0;

        if(params.find(Parameters::ST_MVL::GAMMA) == params.end())
            params[Parameters::ST_MVL::GAMMA] = 0.85;

        if(params.find(Parameters::ST_MVL::WIN_SIZE) == params.end())
            params[Parameters::ST_MVL::WIN_SIZE] = 7;

        return Recovery_ST_MVL(mat, params);
    }
    else if (algorithm == "spirit")
    {
        if(params.find(Parameters::SPIRIT::TRUNCATION) == params.end())
            params[Parameters::SPIRIT::TRUNCATION] = 3;

        if(params.find(Parameters::SPIRIT::WIN_SIZE) == params.end())
            params[Parameters::SPIRIT::WIN_SIZE] = 6;

        if(params.find(Parameters::SPIRIT::LAMBDA) == params.end())
            params[Parameters::SPIRIT::LAMBDA] = 1;

        return Recovery_SPIRIT(mat, params);
    }
    else if (algorithm == "grouse")
    {
        if(params.find(Parameters::GROUSE::TRUNCATION) != params.end())
            params[Parameters::GROUSE::TRUNCATION];

        return Recovery_GROUSE(mat, params);
    }
    else if (algorithm == "nnmf")
    {
        if(params.find(Parameters::NNMF::TRUNCATION) == params.end())
            params[Parameters::NNMF::TRUNCATION] = 3;

        if(params.find(Parameters::NNMF::TOLERANCE) == params.end())
            params[Parameters::NNMF::TOLERANCE] = 1e-6;

        if(params.find(Parameters::NNMF::MAX_ITER) == params.end())
            params[Parameters::NNMF::MAX_ITER] = 100;

        return Recovery_NNMF(mat, params);
    }
    else if (algorithm == "dynammo")
    {
        if(params.find(Parameters::DYNAMMO::TRUNCATION) == params.end())
            params[Parameters::DYNAMMO::TRUNCATION] = 3;

        if(params.find(Parameters::DYNAMMO::MAX_ITER) == params.end())
            params[Parameters::DYNAMMO::MAX_ITER] = 100;

        return Recovery_DynaMMo(mat, params);
    }
    else if (algorithm == "svt")
    {
        if(params.find(Parameters::SVT::MAX_ITER) == params.end())
            params[Parameters::SVT::MAX_ITER] = 100;

        if(params.find(Parameters::SVT::TOLERANCE) == params.end())
            params[Parameters::SVT::TOLERANCE] = 1e-4;

        if(params.find(Parameters::SVT::TAUSCALE) == params.end())
            params[Parameters::SVT::TAUSCALE] = 0.2;

        return Recovery_SVT(mat, params);
    }
    else if (algorithm == "rosl")
    {
        if(params.find(Parameters::ROSL::TRUNCATION) == params.end())
            params[Parameters::ROSL::TRUNCATION] = 3;

        if(params.find(Parameters::ROSL::TOLERANCE) == params.end())
            params[Parameters::ROSL::TOLERANCE] = 1e-6;

        if(params.find(Parameters::ROSL::MAX_ITER) == params.end())
            params[Parameters::ROSL::MAX_ITER] = 100;

        return Recovery_ROSL(mat, params);
    }
    else if (algorithm == "itersvd")
    {
        if(params.find(Parameters::SVD::TRUNCATION) == params.end())
            params[Parameters::SVD::TRUNCATION] = 3;

        if(params.find(Parameters::SVD::TOLERANCE) == params.end())
            params[Parameters::SVD::TOLERANCE] = 1e-6;

        if(params.find(Parameters::SVD::MAX_ITER) == params.end())
            params[Parameters::SVD::MAX_ITER] = 100;

        return Recovery_IterativeSVD(mat, params);
    }
    else if (algorithm == "softimpute")
    {
        if(params.find(Parameters::SOFTIMPUTE::MAX_ITER) == params.end())
                  params[Parameters::SOFTIMPUTE::MAX_ITER] = 100;

        if(params.find(Parameters::SOFTIMPUTE::TRUNCATION) == params.end())
                     params[Parameters::SOFTIMPUTE::TRUNCATION] = 3;

        if(params.find(Parameters::SOFTIMPUTE::TOLERANCE) == params.end())
                    params[Parameters::SOFTIMPUTE::TOLERANCE] = 1e-6;

        return Recovery_SoftImpute(mat, params);
    }
    else
    {
        std::cout << "Algorithm name '" << algorithm << "' is not valid" << std::endl;
        abort();
    }
}

std::string DName2Folder(std::string dataset){
    if(dataset == "airq")
        return "air_quality";
    if(dataset == "drift10")
        return "drift";
    return dataset;
}

void Start_Benchmark(settings &set, Scenarios::scenario_settings &scenarioSettings, arma::mat refference){
    arma::mat mat;
    std::ifstream file;

    // TODO: Remove this hardcoded list
    size_t nticks = 8;
    u_int64_t ticks[] = {100, 200, 300, 400, 500, 600, 700, 800};
    std::vector<u_int64_t> runtimes;
    std::vector<double> rmses;
    u_int64_t runtimeSum = 0;
    double rmseSum = 0;

    currentPath = std::filesystem::current_path();

    if(set.algorithm == "trmf"){
        return;
    }

    // Replace real data with nan
    int numScenarios = scenarioSettings.input_paths.size();
    unsigned int numThreads = 1;
    std::vector<std::thread> threads;

    if(set.multi_t){
        numThreads = std::thread::hardware_concurrency() > numScenarios ? numScenarios : std::thread::hardware_concurrency();
    }

    std::vector<std::pair<double, u_int64_t>> results;
    results.resize(numScenarios);

    std::cout << "Using " << numThreads << " threads" << std::endl;
    for (int t = 0; t < numThreads; t++ ) {
        int start = t * numScenarios / numThreads;
        int end = (t + 1) * numScenarios / numThreads;
        threads.emplace_back([&refference, scenarioSettings, &set, start, end, &results, t]() {
            std::cout << "Start of thread" << t << std::endl;
            for(int i = start; i < end; i++) {
                results[i] = run(refference, scenarioSettings.input_paths[i], set);
            }
            std::cout << "End of thread" << t << std::endl;
        });
    }

    for (auto& t : threads){
        t.join();
    }

    for (auto& p: results){
        rmseSum += p.first;
        rmses.push_back(p.first);
        runtimeSum += p.second;
        runtimes.push_back(p.second);
    }

    set.rmse = isinf(rmseSum / (double) nticks) ? DBL_MAX : rmseSum / (double) nticks;
    set.runtime = runtimeSum / nticks;

    std::ostringstream runsString;
    for(int i = 0; i<rmses.size(); i++){
        std::string input = scenarioSettings.input_paths[i];
        std::string number = input.substr(input.find_last_of('_') + 1);
        number = number.substr(0, number.find_last_of('.'));
        runsString << number << "," << rmses[i] << "," << runtimes[i] << "\n";
    }
    std::ofstream runs("_data/out/results.txt");
    runs << runsString.str();
    runs.close();
    std::cout << runsString.str() << std::endl;
    set.runs =  runsString.str();
}

double getRMSE_Vec(arma::vec &ref, arma::vec &forecast, int64_t blockSize){
    arma::vec difference = forecast - ref;

    double square_sum = 0;
    for(double value: difference){
        square_sum += pow(value, 2);
    }
    double mse = square_sum / (double)blockSize;
    double rmse = sqrt(mse);
    if(isinf(rmse))
        rmse = DBL_MAX;
    return rmse;
}

double getRMSE_Mat(arma::mat ref, arma::mat recovered, arma::mat missing){
    arma::umat mask = arma::find_nonfinite(missing);
    arma::mat ref_masked = ref(mask);
    arma::mat recovered_masked = recovered(mask);

    arma::mat diff = ref_masked - recovered_masked;
    return arma::norm(diff, "fro") / std::sqrt(diff.n_elem);
}

void generateLatLongFile(std::string destination, arma::mat &mat){
    int n_rows = mat.n_rows;
    int n_col = mat.n_cols;

    std::vector<std::string> sensors;

    double lat = 39.954047;
    double lng = 116.348991;

    for(int i = 0; i < n_col; i++){
        int sensorId = 1000 + i;
        sensors.push_back(std::to_string(sensorId) + ","
                        + std::to_string(lat) + ","
                        + std::to_string(lng));

        if(i % 2 == 0)
            lat += 0.01;
        else
            lng += 0.01;
    }

    std::ofstream file(destination);
    file << "sensor_id,latitude,longitude" << std::endl;
    for(auto sensor : sensors){
        file << sensor << std::endl;
    }
    file.close();
}

std::pair<double, u_int64_t>  run(arma::mat &mat, std::string input, settings &set){
    uint64_t runtime;
    double rmse;

//    if(set.misaligned) {
//        startingIndex--;
//        blockSize += 2;
//    }

    // Get the tcase from the file name
    std::string number = input.substr(input.find_last_of('_') + 1);
    number = number.substr(0, number.find_last_of('.'));
    int tcase = std::stoi(number);

    arma::mat missing;
    missing.load(input, arma::raw_ascii);
    arma::mat recovered = missing;

    if(set.misaligned){
        enlarge_missing_blocks(recovered);
        // For testing purposes
        // recovered.save("_data/enlarge_test/data_" + std::to_string(tcase)+".txt", arma::raw_ascii);
    }

    // Recover the matrix and get metrics
    runtime = Performance::Recovery(recovered, set.algorithm, set.params);


    // Dont forget to remove the first and last point of all missing blocks before the recovery to get better
    // results for misalignement
    if(set.misaligned){
        auto missing_blocks = find_missing_blocks(missing);
        for(auto missing_block: missing_blocks){
            int missing_start = std::get<0>(missing_block);
            int missing_size = std::get<1>(missing_block);
            int missing_col = std::get<2>(missing_block);
            arma::vec z = MisalignRedux::linear_interpolation(recovered(missing_start, missing_col),
                                                              recovered(missing_start + missing_size, missing_col),
                                                              missing_size);
            arma::vec y = MisalignRedux::linear_interpolation(missing(missing_start, missing_col),
                                                              missing(missing_start + missing_size, missing_col),
                                                              missing_size);
            recovered = recovered - (z - y);
        }
    }

    std::string mat_filename = "_data/out/data_"+std::to_string(tcase)+".txt";
    recovered.save(mat_filename, arma::raw_ascii);

    rmse = getRMSE_Mat(mat, recovered, missing);
    std::cout << "RMSE OF '" << input << "': " << rmse << std::endl;
    return std::make_pair(rmse, runtime);
}

// Returns list of tuple as (start, size, column), start is include but start + size is exclude.
std::vector<std::tuple<int, int, int>> find_missing_blocks(const arma::mat& B) {
    std::vector<std::tuple<int, int, int>> missing_blocks;
    for (arma::uword j = 0; j < B.n_cols; ++j) {
        int start_index = -1;
        int block_size = 0;
        for (arma::uword i = 0; i < B.n_rows; ++i) {
            if (!arma::is_finite(B(i, j))) {
                if (start_index == -1) {
                    start_index = i;
                }
                ++block_size;
            } else {
                if (start_index != -1) {
                    missing_blocks.emplace_back(start_index, block_size, j);
                    start_index = -1;
                    block_size = 0;
                }
            }
        }
        if (start_index != -1) {
            missing_blocks.emplace_back(start_index, block_size, j);
        }
    }
    return missing_blocks;
}

void enlarge_missing_blocks(arma::mat& missing){
    std::vector<std::tuple<int, int, int>> missing_blocks = find_missing_blocks(missing);
    for(auto missing_block: missing_blocks){
        int missing_start = std::get<0>(missing_block);
        int missing_size = std::get<1>(missing_block);
        int missing_end = missing_start + missing_size;
        int missing_col = std::get<2>(missing_block);

        if(missing_size < 3){ // Prevent removing small gaps
            continue;
        }

        if(missing_start != 0){
            missing(missing_start - 1,missing_col) = NAN;
        }
        if(missing_end != missing.n_rows){
            missing(missing_end, missing_col) = NAN;
        }
    }
}

} // namespace Performance
