//
// Created by lucie on 7/15/2022.
//

#ifndef REDUCED_BENCH_PARSER_H
#define REDUCED_BENCH_PARSER_H

#include <iostream>
#include <map>

struct settings {
    std::string autoH;
    std::string dataset;
    std::string algorithm;
    std::map<std::string, double> params;
    u_int64_t runtime;
    double rmse;
    std::string label;
};

struct cd_settings : settings {
    u_int64_t truncation;
    u_int64_t max_iter;
    double treshold;
};

namespace Parameters{
    namespace CD {
        const std::string TRUNCATION = "Truncation";
        const std::string MAX_ITER = "Max_Iter";
        const std::string TOLERANCE = "Tolerance";
    }

    namespace TKCM {
        const std::string TRUNCATION = "Truncation";
        const std::string D = "D";
    }

    namespace ST_MVL{
        const std::string ALPHA = "Alpha";
        const std::string GAMMA = "Gamma";
        const std::string WIN_SIZE = "Win_Size";
    }

    namespace SPIRIT {
        const std::string TRUNCATION = "Truncation";
        const std::string WIN_SIZE = "Win_Size";
        const std::string LAMBDA = "Lambda";
    }

    namespace GROUSE {
        const std::string TRUNCATION = "Truncation";
    }

    namespace NNMF {
        const std::string TRUNCATION = "Truncation";
        const std::string TOLERANCE = "Tolerance";
        const std::string MAX_ITER = "Max_Iter";
    }

    namespace DYNAMMO {
        const std::string TRUNCATION = "Truncation";
        const std::string MAX_ITER = "Max_Iter";
    }

    namespace SVT {
        const std::string MAX_ITER = "Max_Iter";
        const std::string TOLERANCE = "Tolerance";
        const std::string TAUSCALE = "Tauscale";
    }

    namespace ROSL {
        const std::string TRUNCATION = "Truncation";
        const std::string TOLERANCE = "Tolerance";
        const std::string MAX_ITER = "Max_Iter";
    }

    namespace SVD {
        const std::string TRUNCATION = "Truncation";
        const std::string TOLERANCE = "Tolerance";
        const std::string MAX_ITER = "Max_Iter";
    }

    namespace SOFTIMPUTE {
        const std::string TRUNCATION = "Truncation";
        const std::string TOLERANCE = "Tolerance";
        const std::string MAX_ITER = "Max_Iter";
    }

    namespace TRMF {
        const std::string K = "K";
        const std::string MAX_ITER = "Max_Iter";
        const std::string LAMBDA_I = "LambdaI";
        const std::string LAMBDA_AR = "LambdaAR";
        const std::string LAMBDA_LAG = "LambdaLag";
    }
}


enum optionIndex { UNKNOWN, HELP, AUTO, ALG, PARAMS, TRUNCATION, MAX_ITER, TOLERANCE,
        D, ALPHA, GAMMA, WIN_SIZE, LAMBDA, TAUSCALE, LAMBDA_I, LAMBDA_AR, LAMBDA_LAG,
        LABEL, DATASET };

int parse(int argc, char **argv, settings &set);

#endif //REDUCED_BENCH_PARSER_H
