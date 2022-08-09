//
// Created by lucie on 8/5/2022.
//

#include <iostream>
#include <vector>

#include "SuccessiveHalving.h"

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

void SuccessiveHalving(settings &set){
    std::vector<int> parameters(10);
    range(parameters.begin(), parameters.end(), 1, 1);
    double iterBudget = 50;
    double thresholdBudget = 1e-5;
    std::vector<settings> round(6, set);
    for(int i = 0; i<round.size(); i++){
        round[i].params[0] = iterBudget;
        round[i].params[1] = thresholdBudget;
        round[i].params[2] = parameters[i];
    }
    while(round.size()>1){
        for(auto &s: round)
            Performance::Start_Benchmark(s);
        std::sort(round.begin(), round.end(), compareByRmse);
        round.resize(round.size()/2);
        for(const auto &s: round)
            std::cout << "Truncation : " << s.params[2] << " // RMSE : " << s.rmse << std::endl;
        if(round.size()<1)
            break;
        for(auto &s: round){
            s.params[0] /= 10;
            s.params[1] *= 2;
        }
    }
    std::cout << "Winner, winner, chicken dinner : " << round.at(0).params[2] << std::endl;
    // copy settings to main set
    set.params = round.at(0).params;
    set.runtime = round.at(0).runtime;
    set.rmse = round.at(0).rmse;
}
