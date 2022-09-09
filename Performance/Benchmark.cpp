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

    if(params.find(Parameters::CD::TRUNCATION) == params.end())
        params[Parameters::CD::TRUNCATION] = 3;
    truncation = params[Parameters::CD::TRUNCATION];

    if(params.find(Parameters::CD::TOLERANCE) == params.end())
        params[Parameters::CD::TOLERANCE] = 1e-6;
    threshold = params[Parameters::CD::TOLERANCE];

    if(params.find(Parameters::CD::MAX_ITER) == params.end())
        params[Parameters::CD::MAX_ITER] = 100;
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

    if(params.find(Parameters::TKCM::TRUNCATION) == params.end())
        params[Parameters::TKCM::TRUNCATION] = 1;
    truncation = params[Parameters::TKCM::TRUNCATION];

    if(params.find(Parameters::TKCM::D) == params.end())
        d = params[Parameters::TKCM::D] = 1;
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

    if(params.find(Parameters::ST_MVL::ALPHA) == params.end())
        params[Parameters::ST_MVL::ALPHA] = 2.0;
    alpha = params[Parameters::ST_MVL::ALPHA];

    if(params.find(Parameters::ST_MVL::GAMMA) == params.end())
        params[Parameters::ST_MVL::GAMMA] = 0.85;
    beta = params[Parameters::ST_MVL::GAMMA];

    if(params.find(Parameters::ST_MVL::WIN_SIZE) == params.end())
        params[Parameters::ST_MVL::WIN_SIZE] = 7;
    winSize = params[Parameters::ST_MVL::WIN_SIZE];

    // Local
    int64_t result;
    const std::string latlong = "latlong_placeholder.txt";
    generateLatLongFile(latlong, mat);
    ST_MVL stmvl(mat, latlong, alpha, beta, winSize);

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // Recovery

    begin = std::chrono::steady_clock::now();
    stmvl.Run(true);
    end = std::chrono::steady_clock::now();

    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (STMVL): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}


int64_t Recovery_SPIRIT(arma::mat &mat, std::map<std::string, double> &params)
{
    double truncation, winSize, lambda;

    if(params.find(Parameters::SPIRIT::TRUNCATION) == params.end())
        truncation = params[Parameters::SPIRIT::TRUNCATION] = 3;
    else
        truncation = params[Parameters::SPIRIT::TRUNCATION];

    if(params.find(Parameters::SPIRIT::WIN_SIZE) == params.end())
        winSize = params[Parameters::SPIRIT::WIN_SIZE] = 6;
    else
        winSize = params[Parameters::SPIRIT::WIN_SIZE];

    if(params.find(Parameters::SPIRIT::LAMBDA) == params.end())
        lambda = params[Parameters::SPIRIT::LAMBDA] = 1;
    else
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

    if(params.find(Parameters::GROUSE::TRUNCATION) != params.end())
        truncation = params[Parameters::GROUSE::TRUNCATION];
    else
        truncation = 3;

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

        if(params.find(Parameters::NNMF::TRUNCATION) != params.end())
            truncation = params[Parameters::NNMF::TRUNCATION];
        else
            truncation = 3;

        if(params.find(Parameters::NNMF::TOLERANCE) != params.end())
            threshold = params[Parameters::NNMF::TOLERANCE];
        else
            threshold = 1e-6;

        if(params.find(Parameters::NNMF::MAX_ITER) != params.end())
            maxIter = params[Parameters::NNMF::MAX_ITER];
        else
            maxIter = 100;

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

    if(params.find(Parameters::DYNAMMO::TRUNCATION) != params.end())
        truncation = params[Parameters::DYNAMMO::TRUNCATION];
    else
        truncation = 3;

    if(params.find(Parameters::DYNAMMO::MAX_ITER) != params.end())
        maxIter = params[Parameters::DYNAMMO::MAX_ITER];
    else
        maxIter = 100;

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

    if(params.find(Parameters::SVT::MAX_ITER) != params.end())
        maxIter = params[Parameters::SVT::MAX_ITER];
    else{
        maxIter = 100;
        params[Parameters::SVT::MAX_ITER] = maxIter;
    }


    if(params.find(Parameters::SVT::TOLERANCE) != params.end())
        threshold = params[Parameters::SVT::TOLERANCE];
    else {
        threshold = 1e-4;
        params[Parameters::SVT::TOLERANCE] = threshold;
    }

    if(params.find(Parameters::SVT::TAUSCALE) != params.end())
        tauscale = params[Parameters::SVT::TAUSCALE];
    else {
        tauscale = 0.2;
        params[Parameters::SVT::TAUSCALE] = tauscale;
    }

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

    if(params.find(Parameters::ROSL::TRUNCATION) != params.end())
        truncation = params[Parameters::ROSL::TRUNCATION];
    else
        truncation = 3;

    if(params.find(Parameters::ROSL::TOLERANCE) != params.end())
        threshold = params[Parameters::ROSL::TOLERANCE];
    else
        threshold = 1e-7;

    if(params.find(Parameters::ROSL::MAX_ITER) != params.end())
        maxIter = params[Parameters::ROSL::MAX_ITER];
    else
        maxIter = 500;

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

    if(params.find(Parameters::SVD::TRUNCATION) != params.end())
        truncation = params[Parameters::SVD::TRUNCATION];
    else
        truncation = 3;

    if(params.find(Parameters::SVD::TOLERANCE) != params.end())
        threshold = params[Parameters::SVD::TOLERANCE];
    else
        threshold = 1e-6;

    if(params.find(Parameters::SVD::MAX_ITER) != params.end())
        maxIter = params[Parameters::SVD::MAX_ITER];
    else
        maxIter = 100;
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

    maxIter = params.find(Parameters::SOFTIMPUTE::MAX_ITER) != params.end() ?
            params[Parameters::SOFTIMPUTE::MAX_ITER] : 100;
    truncation = params.find(Parameters::SOFTIMPUTE::TRUNCATION) != params.end() ?
              params[Parameters::SOFTIMPUTE::TRUNCATION] : 3;
    threshold = params.find(Parameters::SOFTIMPUTE::TOLERANCE) != params.end() ?
                 params[Parameters::SOFTIMPUTE::TOLERANCE] : 1e-6;

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

    params[Parameters::SOFTIMPUTE::MAX_ITER] = maxIter;
    params[Parameters::SOFTIMPUTE::TRUNCATION] = truncation;
    params[Parameters::SOFTIMPUTE::TOLERANCE] = threshold;

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
        return Recovery_CD(mat, params);
    }
    else if (algorithm == "tkcm")
    {
        return Recovery_TKCM(mat, params);
    }
    else if (algorithm == "st-mvl")
    {
        return Recovery_ST_MVL(mat, params);
    }
    else if (algorithm == "spirit")
    {
        return Recovery_SPIRIT(mat, params);
    }
    else if (algorithm == "grouse")
    {
        return Recovery_GROUSE(mat, params);
    }
    else if (algorithm == "nnmf")
    {
        return Recovery_NNMF(mat, params);
    }
    else if (algorithm == "dynammo")
    {
        return Recovery_DynaMMo(mat, params);
    }
    else if (algorithm == "svt")
    {
        return Recovery_SVT(mat, params);
    }
    else if (algorithm == "rosl")
    {
        return Recovery_ROSL(mat, params);
    }
    else if (algorithm == "itersvd")
    {
        return Recovery_IterativeSVD(mat, params);
    }
    else if (algorithm == "softimpute")
    {
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

void Start_Benchmark(settings &set){
    arma::mat mat;
    std::ifstream file;

    // TODO: Remove this hardcoded list
    size_t nticks = 8;
    u_int64_t ticks[] = {100, 200, 300, 400, 500, 600, 700, 800};
    u_int64_t runtimeSum = 0;
    double rmseSum = 0;

    std::string dataFolder = "Datasets/real_world/";
    currentPath = std::filesystem::current_path();

    std::string dataFdName = DName2Folder(set.dataset);
    std::string fileName = set.dataset + "_normal.txt";
    std::string filePath = currentPath + "/" + dataFolder + dataFdName + "/" + fileName;

    if(set.algorithm == "trmf"){
        for(auto t: ticks) {
            // Local
            std::chrono::steady_clock::time_point begin;
            std::chrono::steady_clock::time_point end;

            begin = std::chrono::steady_clock::now();
            rmseSum += Recovery_TRMF2(set.dataset, set, t);
            end = std::chrono::steady_clock::now();

            runtimeSum += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            std::cout << "Time(TRMF) : " << set.runtime << std::endl;
        }
        set.rmse = rmseSum / (double) nticks;
        set.runtime = runtimeSum / nticks;
        return;
    }

    // Creating the dataset matrix
    file.open(filePath);

    if (file.is_open()) {
        mat.load(file, arma::raw_ascii);
    } else {
        std::cerr << "can't open the dataset file" << std::endl;
        exit(EXIT_FAILURE);
    }
    file.close();

    // Replace real data with nan
    for(auto t : ticks){
        arma::mat matcopy = mat;
        u_int64_t column = 0;
        u_int64_t startingIndex = (u_int64_t) (matcopy.n_rows * 0.05);
        u_int64_t blockSize = mat.n_rows * t / 1000;
        MissingBlock missing(column, startingIndex, blockSize, matcopy);

        arma::vec reff = missing.extractBlock();
        arma::vec missingVector(blockSize);
        missingVector.fill(NAN);
        missing.imputeBlock(missingVector);

        // Recover the matrix and get metrics
        runtimeSum += Performance::Recovery(matcopy, set.algorithm, set.params);

        arma::vec recovered = missing.extractBlock();
        rmseSum += getRMSE_Vec(reff, recovered, missing.blockSize);
    }
    set.rmse = rmseSum / (double) nticks;
    set.runtime = runtimeSum / nticks;
}

double getRMSE_Vec(arma::vec &ref, arma::vec &forecast, int64_t blockSize){
    arma::vec difference = forecast - ref;

    double square_sum = 0;
    for(double value: difference){
        square_sum += pow(value, 2);
    }
    double mse = square_sum / (double)blockSize;
    double rmse = sqrt(mse);
    return rmse;
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
}

} // namespace Performance
