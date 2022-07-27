//
// Created on 03/01/19.
//

#pragma once

#include <armadillo>

namespace Algorithms
{

#define mod(x, y) ((((x) % (y)) + (y)) % (y))

class TKCM
{
  private:
    arma::mat &matrix;
    
    uint64_t l;
    const uint64_t k = 5;
    // uint64_t k;
    // const uint64_t d = 3;
    uint64_t d;
    
    void actionTkcm(const arma::mat &ref_ts, arma::vec &ts, uint64_t &offset, const uint64_t &L);
  
  public:
    explicit TKCM(arma::mat &mx, uint64_t trunc, uint64_t d);
    
    void performRecovery();
};

} // namespace Algorithms


