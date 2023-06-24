//
// Created by Zakhar on 16/03/2017.
//

#ifndef REDUCED_BENCH_BENMARCK_H
#define REDUCED_BENCH_BENMARCK_H

#include <mlpack/core.hpp>

#include "../MathIO/MatrixReadWrite.h"
#include "../Algebra/MissingBlock.hpp"
#include "MisalignRedux.h"

#include "../parser.h"

namespace Performance
{

    int64_t
    Recovery(arma::mat &mat, uint64_t truncation,
             const std::string &algorithm, const std::string &xtra);

    int64_t Recovery(arma::mat &mat, const std::string &algorithm,
                     std::map<std::string, double> &params);

    void Start_Benchmark(settings &set, Scenarios::scenario_settings &scenario, arma::mat refference);
    double getRMSE_Vec(arma::vec &ref, arma::vec &forecast, int64_t blockSize);

    static std::string dataFolder = "Datasets/real_world/";

} // namespace Performance

#endif //REDUCED_BENCH_BENCHMARK_H

