//
// Created by lucie on 08.03.2023.
//

#include "Scenarios/MissingBlock.h"

namespace Scenarios {

    arma::mat MissingBlock::createMissing(arma::mat original, int tcase, std::vector<int> params) {
        if(!params.empty()){
            col = params[0];
            start = params[1];
        }
        arma::mat missing = original;
        int startIndex = missing.n_rows * start / 100;
        int blockSize = missing.n_rows * tcase / 100;
        for(int i=0; i<blockSize; i++){
            missing(startIndex-1 + i, col) = NAN;
        }
        return missing;
    }
}