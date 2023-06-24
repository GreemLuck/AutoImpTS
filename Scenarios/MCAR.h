//
// Created by lucie on 10.03.2023.
//

#ifndef REDUCED_BENCH_MCAR_H
#define REDUCED_BENCH_MCAR_H

#include "Scenario.h"

#include <stdexcept>
#include <armadillo>

namespace Scenarios {

    class Mcar : public Scenario {
    public:
//        Mcar(int missingBlockSize, int nMissingBlocks)
//        :missingBlockSize(missingBlockSize), nMissingBlocks(nMissingBlocks)
//        {
//            subMissingBlockSize = missingBlockSize / nMissingBlocks;
//            if(subMissingBlockSize <= 0){
//                throw std::invalid_argument("The number of missing blocks is too high");
//            }
//        }

        arma::mat createMissing(arma::mat original, int tcase, std::vector<int> params);
        void setSeed(unsigned int seed);

    private:
        double tcase = 10;
        int missingBlockSize = 10;
        int subMissingBlockSize;
        int nMissingBlocks = 1;

        int startLock = 20;
    };

} // Scenarios

#endif //REDUCED_BENCH_MCAR_H
