//
// Created by lucie on 8/5/2022.
//

#include <iostream>
#include <vector>

#include "SuccessiveHalving.h"
#include "../Database/SQL.h"

template <class ForwardIt, class T> void range(ForwardIt first, ForwardIt last, T value, T step = 1){
    while(first!=last){
        *first++ = value;
        value += step;
    }
}

template <class ForwardIt, class T> void mrange(ForwardIt first, ForwardIt last, T value, T step = 2){
    while(first!=last){
        *first++ = value;
        value *= step;
    }
}

bool compareByRmse(const settings &a, const settings &b){
    return a.rmse < b.rmse;
}

void TruncationBased_SH(settings &set){
    double budgetIter = 100;
    double budgetThreshold = 1e-5;
    std::vector<int> parameters(10);
    range(parameters.begin(), parameters.end(), 1, 1);

    // Settings up the sets of parameters
    std::vector<settings> round(10, set);
    for(int i = 0; i<round.size(); i++){
        round[i].params[Parameters::CD::MAX_ITER] = budgetIter;
        if(set.algorithm != "dynammo")
            round[i].params[Parameters::CD::TOLERANCE] = budgetThreshold;
        round[i].params[Parameters::CD::TRUNCATION] = parameters[i];
    }

    while(round.size()>1){
        for(auto &s: round){
            Performance::Start_Benchmark(s);
            Database::sql_insert(s);
        }
        std::sort(round.begin(), round.end(), compareByRmse);
        round.resize(round.size()/2);
        if(round.size()<1)
            break;
        for(auto &s: round) {
            if(set.algorithm != "dynammo")
                s.params[Parameters::CD::TOLERANCE] /= 10;
            s.params[Parameters::CD::MAX_ITER] *= 2;
        }
    }
    // Copy the settings to main set
    set.params = round.at(0).params;
    set.runtime = round.at(0).runtime;
    set.rmse = round.at(0).rmse;
}

void SVT_SH(settings &set){
    double budgetIter = 100;
    double budgetThreshold = 1e-5;
    double size = 50;
    std::vector<double> parameters(size);
    range(parameters.begin(), parameters.end(), 0.02, 0.02);

    // Settings up the sets of parameters
    std::vector<settings> round(size, set);
    for(int i = 0; i<round.size(); i++){
        round[i].params[Parameters::SVT::MAX_ITER] = budgetIter;
        round[i].params[Parameters::SVT::TOLERANCE] = budgetThreshold;
        round[i].params[Parameters::SVT::TAUSCALE] = parameters[i];
    }

    while(round.size()>1){
        for(auto &s: round){
            Performance::Start_Benchmark(s);
            Database::sql_insert(s);
        }
        std::sort(round.begin(), round.end(), compareByRmse);
        round.resize(round.size()/2);
        if(round.size()<1)
            break;
        for(auto &s: round) {
            s.params[Parameters::CD::TOLERANCE] /= 10;
            s.params[Parameters::CD::MAX_ITER] *= 2;
        }
    }
    // Copy the settings to main set
    set.params = round.at(0).params;
    set.runtime = round.at(0).runtime;
    set.rmse = round.at(0).rmse;
}

void TRMF_SH(settings &set){
    double budgetIter = 100;
    double budgetThreshold = 1e-5;
    std::vector<double> parameters(10);
    range(parameters.begin(), parameters.end(), 0.1, 0.1);

    // Settings up the sets of parameters
    std::vector<settings> round(10, set);
    for(int i = 0; i<round.size(); i++){
        round[i].params[Parameters::TRMF::MAX_ITER] = budgetIter;
        round[i].params[Parameters::TRMF::TOLERANCE] = budgetThreshold;
        round[i].params[Parameters::CD::TRUNCATION] = parameters[i];
    }

    while(round.size()>1){
        for(auto &s: round){
            Performance::Start_Benchmark(s);
            Database::sql_insert(s);
        }
        std::sort(round.begin(), round.end(), compareByRmse);
        round.resize(round.size()/2);
        if(round.size()<1)
            break;
        for(auto &s: round) {
            s.params[Parameters::CD::TOLERANCE] /= 10;
            s.params[Parameters::CD::MAX_ITER] *= 2;
        }
    }
    // Copy the settings to main set
    set.params = round.at(0).params;
    set.runtime = round.at(0).runtime;
    set.rmse = round.at(0).rmse;
}

void SuccessiveHalving(settings &set){
    if(set.algorithm == "cd"
    || set.algorithm == "nnmf"
    || set.algorithm == "rosl"
    || set.algorithm == "itersvd"
    || set.algorithm == "softimpute"
    || set.algorithm == "dynammo")
        TruncationBased_SH(set);
    else if(set.algorithm == "svt")
        SVT_SH(set);
    else if(set.algorithm == "trmf")
        TRMF_SH(set);
    else
        std::cerr << set.algorithm <<  " does not support SuccessiveHalving" << std::endl;
}