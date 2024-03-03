//
// Created by lucie on 11/22/2022.
//

#ifndef REDUCED_BENCH_MISALIGNREDUX_H
#define REDUCED_BENCH_MISALIGNREDUX_H

#include <mlpack/core.hpp>
#include <armadillo>

namespace MisalignRedux {
    arma::vec linear_interpolation(double start, double end, int size);
} // MisalignRedux

#endif //REDUCED_BENCH_MISALIGNREDUX_H
