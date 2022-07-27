//
// Created by Zakhar on 16/03/2017.
//

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
int64_t Recovery_CD(arma::mat &mat, std::vector<double> &params)
{
    // Parameters order: maxiter, eps, truncation
    std::vector<double> default_params = {100, 1E-6, 3};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;
    CDMissingValueRecovery rmv(mat, params[0], params[1]);
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    rmv.setReduction(params[2]);
    rmv.disableCaching = false;
    rmv.useNormalization = false;
    
    begin = std::chrono::steady_clock::now();
    rmv.autoDetectMissingBlocks();
    rmv.performRecovery(params[2] == mat.n_cols);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (CDRec): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_TKCM(arma::mat &mat, std::vector<double> &params)
{
    // Parameters order: k, d
    std::vector<double> default_params = {1, 1};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;
    Algorithms::TKCM tkcm(mat, params[0], params[1]);
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

int64_t Recovery_ST_MVL(arma::mat &mat, std::vector<double> &params)
{
    // Parameters order: alpha, beta, w
    std::vector<double> default_params = {2.0, 0.85, 7};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;
    // TODO: implement correctly latlong argument by adding it before calling benchmark
    const std::string latlong = "latlong_placeholder.txt";

    ST_MVL stmvl(mat, latlong, params[0], params[1], params[2]);

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


int64_t Recovery_SPIRIT(arma::mat &mat, std::vector<double> &params)
{
    // Parameters order: k0, w, lambda
    std::vector<double> default_params = {3, 6, 1};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;
    
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    SPIRIT::doSpirit(mat, params[0], params[1], params[2]);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SPIRIT): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_GROUSE(arma::mat &mat, std::vector<double> &params)
{
    // Params order: maxrank
    std::vector<double> default_params = {3};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    // Recovery

    mat = mat.t();

    begin = std::chrono::steady_clock::now();
    GROUSE::doGROUSE(mat, params[0]);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (GROUSE): " << result << std::endl;
    
    mat = mat.t();
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_NNMF(arma::mat &mat, std::vector<double> &params)
{
    // Params order: maxiter, truncation, tolerance
    std::vector<double> default_params = {100, 3, 1E-6};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;
    
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    NMFMissingValueRecovery::doNMFRecovery(mat, params[1], params[2], params[0]);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (TeNMF): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_DynaMMo(arma::mat &mat, std::vector<double> &params)
{
    // Params order: maxiter, truncation
    std::vector<double> default_params = {100, 3};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    
    mat = mat.t();
    
    begin = std::chrono::steady_clock::now();
    DynaMMo::doDynaMMo(mat, params[1], params[0], true);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (DynaMMo): " << result << std::endl;
    
    mat = mat.t();
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_SVT(arma::mat &mat, std::vector<double> &params)
{
    // Params order: maxiter, tauscale, tolerance
    std::vector<double> default_params = {100, 0.2, 1E-4};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;
    
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // double tauScale = std::stod(xtra);

    // Recovery
    begin = std::chrono::steady_clock::now();
    SVT::doSVT(mat, params[1], params[2], params[0]);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SVT): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_ROSL(arma::mat &mat, std::vector<double> &params)
{
    // Params order: rankE, tolerance, maxiter
    std::vector<double> default_params = {3, 1E-6, 500};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    ROSL::ROSL_Recovery(mat, params[0], params[1], params[2]);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (ROSL): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_IterativeSVD(arma::mat &mat, std::vector<double> &params)
{
    // Params order: maxiter, tolerance, truncation
    std::vector<double> default_params = {100, 1E-5, 3};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    IterativeSVD::recoveryIterativeSVD(mat, params[2], params[1], params[0]);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SVDImpute): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery_SoftImpute(arma::mat &mat, std::vector<double>  &params)
{
    //Params order: maxiter, truncation, tolerance
    std::vector<double> default_params = {100, 3, 1E-5};

    set_params(params, default_params, default_params.size());

    // Local
    int64_t result;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    // Recovery
    begin = std::chrono::steady_clock::now();
    SoftImpute::doSoftImpute(mat, params[1], params[2], params[0]);
    end = std::chrono::steady_clock::now();
    
    result = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::cout << "Time (SoftImpute): " << result << std::endl;
    
    verifyRecovery(mat);
    return result;
}

int64_t Recovery(arma::mat &mat, const std::string &algorithm, std::vector<double> &params){
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
