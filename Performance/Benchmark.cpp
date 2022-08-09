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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 3;

    if(params.find("Threshold") != params.end())
        threshold = params["Threshold"];
    else
        threshold = 1e-6;

    if(params.find("Max_Iter") != params.end())
        maxIter = params["Max_Iter"];
    else
        maxIter = 100;

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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 1;

    if(params.find("D") != params.end())
        d = params["D"];
    else
        d = 1;

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

    if(params.find("Alpha") != params.end())
        alpha = params["Alpha"];
    else
        alpha = 2.0;

    if(params.find("Beta") != params.end())
        beta = params["Beta"];
    else
        beta = 0.85;

    if(params.find("Win_Size") != params.end())
        winSize = params["Win_Size"];
    else
        winSize = 7;

    // Local
    int64_t result;
    // TODO: implement correctly latlong argument by adding it before calling benchmark
    const std::string latlong = "latlong_placeholder.txt";

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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 3;

    if(params.find("Win_Size") != params.end())
        winSize = params["Win_Size"];
    else
        winSize = 6;

    if(params.find("Lambda") != params.end())
        lambda = params["Lambda"];
    else
        lambda = 1;

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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 3;

    if(params.find("Threshold") != params.end())
        threshold = params["Threshold"];
    else
        threshold = 1e-6;

    if(params.find("Max_Iter") != params.end())
        maxIter = params["Max_Iter"];
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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 3;

    if(params.find("Max_Iter") != params.end())
        maxIter = params["Max_Iter"];
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

    if(params.find("Max_Iter") != params.end())
        maxIter = params["Max_Iter"];
    else
        maxIter = 100;

    if(params.find("Threshold") != params.end())
        threshold = params["Threshold"];
    else
        threshold = 1e-4;

    if(params.find("Tauscale") != params.end())
        tauscale = params["Tauscale"];
    else
        tauscale = 0.2;

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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 3;

    if(params.find("Threshold") != params.end())
        threshold = params["Threshold"];
    else
        threshold = 1e-7;

    if(params.find("Max_Iter") != params.end())
        maxIter = params["Max_Iter"];
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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 3;

    if(params.find("Threshold") != params.end())
        threshold = params["Threshold"];
    else
        threshold = 1e-6;

    if(params.find("Max_Iter") != params.end())
        maxIter = params["Max_Iter"];
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

    if(params.find("Truncation") != params.end())
        truncation = params["Truncation"];
    else
        truncation = 3;

    if(params.find("Threshold") != params.end())
        threshold = params["Threshold"];
    else
        threshold = 1e-6;

    if(params.find("Max_Iter") != params.end())
        maxIter = params["Max_Iter"];
    else
        maxIter = 100;

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

void Recovery_TRMF(std::string &pathData, settings &set){
    // Local
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // Testing Python interpreter
    Py_Initialize();

    // Add trmf module to path
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *pathModule = PyObject_GetAttrString(sys, "path");
    PyList_Append(pathModule, PyUnicode_FromString("Algorithms/"));
    PyObject * Module = PyImport_ImportModule("trmfpy");

    // Prepare args
    PyObject * Arg1 = PyUnicode_FromString(pathData.c_str());
    PyObject * Arg2 = PyLong_FromLong(set.tick);
    PyObject * Args = PyTuple_Pack(2, Arg1, Arg2);

    if(!PyTuple_Check(Args))
        std::cerr << "Error with python arguments" << std::endl;

    // Get and run main function
    PyObject * Dict = PyModule_GetDict(Module);
    PyObject * Func = PyDict_GetItemString(Dict, "main");

    begin = std::chrono::steady_clock::now();
    PyObject * Result = PyObject_CallObject(Func, Args);
    end = std::chrono::steady_clock::now();

    if(PyNumber_Check(Result))
        set.rmse = PyFloat_AsDouble(Result);
    else
        PyErr_Print();

    Py_DECREF(Module);
    Py_DECREF(Dict);
    Py_DECREF(Func);
    Py_DECREF(Args);

    Py_Finalize();

    set.runtime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time(TRMF) : " << set.runtime << std::endl;
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

    std::string dataFolder = "Datasets/real_world/";
    currentPath = std::filesystem::current_path();

    std::string dataFdName = DName2Folder(set.dataset);
    std::string fileName = set.dataset + "_normal.txt";
    std::string filePath = currentPath + "/" + dataFolder + dataFdName + "/" + fileName;

    if(set.algorithm == "trmf"){
        Recovery_TRMF(filePath, set);
        return;
    }

    // Creating the dataset matrix
    file.open(filePath);

    if (file.is_open()) {
        mat.load(file, arma::raw_ascii);
    } else {
        std::cerr << "can't open the fucking file" << std::endl;
        exit(EXIT_FAILURE);
    }
    file.close();

    // Replace real data with nan
    u_int64_t column = 0;
    u_int64_t startingIndex = (u_int64_t) (mat.n_rows * 0.05);
    u_int64_t blockSize = mat.n_rows * set.tick / 1000;
    MissingBlock missing(column, startingIndex, blockSize, mat);

    arma::vec reff = missing.extractBlock();
    arma::vec missingVector(blockSize);
    missingVector.fill(NAN);
    missing.imputeBlock(missingVector);

    // Recover the matrix and get metrics
    set.runtime = Performance::Recovery(mat, set.algorithm, set.params);

    arma::vec recovered = missing.extractBlock();
    set.rmse = getRMSE_Vec(reff, recovered, missing.blockSize);
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

} // namespace Performance
