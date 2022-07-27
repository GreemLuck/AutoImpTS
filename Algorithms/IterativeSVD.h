//
// Created on 21/01/19.
//

#pragma once

#include <armadillo>

namespace Algorithms
{

class IterativeSVD
{
  public:
    static void recoveryIterativeSVD(arma::mat &X, uint64_t rank, double threshold, uint64_t max_iters);
};

} // namespace Algorithms
