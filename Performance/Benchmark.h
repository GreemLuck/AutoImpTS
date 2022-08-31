//
// Created by Zakhar on 16/03/2017.
//

#pragma once

#include <mlpack/core.hpp>

#include "../MathIO/MatrixReadWrite.h"
#include "../Algebra/MissingBlock.hpp"

#include "../parser.h"

namespace Performance
{

int64_t
Recovery(arma::mat &mat, uint64_t truncation,
         const std::string &algorithm, const std::string &xtra);

int64_t Recovery(arma::mat &mat, const std::string &algorithm,
                 std::map<std::string, double> &params);

void Start_Benchmark(settings &set);
double getRMSE_Vec(arma::vec &ref, arma::vec &forecast, int64_t blockSize);

} // namespace Performance
