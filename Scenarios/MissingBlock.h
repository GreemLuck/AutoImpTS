//
// Created by lucie on 08.03.2023.
//

#ifndef REDUCED_BENCH_MISSINGBLOCK_H
#define REDUCED_BENCH_MISSINGBLOCK_H

#include <armadillo>

#include "Scenario.h"

namespace Scenarios{
    class MissingBlock : public Scenario {
    public:
//        MissingBlock(int affected_col=0, int start=5)
//                : col(affected_col), start(start) {
//        }

        arma::mat createMissing(arma::mat original, int tcase, std::vector<int> params);

    private:
        int col = 0;
        int start = 20;
    };

}

#endif //REDUCED_BENCH_MISSINGBLOCK_H
