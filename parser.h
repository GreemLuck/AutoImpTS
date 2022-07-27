//
// Created by lucie on 7/15/2022.
//

#ifndef REDUCED_BENCH_PARSER_H
#define REDUCED_BENCH_PARSER_H

#include <iostream>
#include <vector>

struct settings {
    std::string dataset;
    std::string algorithm;
    std::vector<double> params;
    u_int64_t tick;
    u_int64_t runtime;
    double rmse;
};

enum optionIndex { UNKNOWN, HELP, ALG, PARAMS, DATASET, TICK };

int parse(int argc, char **argv, settings &set);

#endif //REDUCED_BENCH_PARSER_H
