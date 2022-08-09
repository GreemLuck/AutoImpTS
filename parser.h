//
// Created by lucie on 7/15/2022.
//

#ifndef REDUCED_BENCH_PARSER_H
#define REDUCED_BENCH_PARSER_H

#include <iostream>
#include <map>

struct settings {
    bool autoH;
    std::string dataset;
    std::string algorithm;
    std::map<std::string, double> params;
    u_int64_t tick;
    u_int64_t runtime;
    double rmse;
};

struct cd_settings : settings {
    u_int64_t truncation;
    u_int64_t max_iter;
    double treshold;
};

enum optionIndex { UNKNOWN, HELP, AUTO, ALG, PARAMS, TRUNCATION, MAX_ITER, THRESHOLD,
        D, ALPHA, GAMMA, WIN_SIZE, LAMBDA, TAUSCALE, DATASET, TICK };

int parse(int argc, char **argv, settings &set);

#endif //REDUCED_BENCH_PARSER_H
