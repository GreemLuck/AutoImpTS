//
// Created by lucie on 10.03.2023.
//

#include "Scenarios/MCAR.h"
#include "Scenarios/ScenarioContext.h"

#include <utility>

namespace Scenarios {
    arma::mat Mcar::createMissing(arma::mat original, int tcase, std::vector<int> params){
        arma::mat missing = std::move(original);

        if(!params.empty()){
            missingBlockSize = params[0];
            nMissingBlocks = params[1];
        }

        subMissingBlockSize = missingBlockSize / nMissingBlocks;
        if(subMissingBlockSize <= 0){
            throw std::invalid_argument("The number of missing blocks is too high");
        }

        // Calculate the number of columns to loop over ( tcase of total columns)
        int n_cols = missing.n_cols * tcase / 100;
        int start = missing.n_rows * startLock / 100;
        int n_segments = (missing.n_rows - start) / subMissingBlockSize;
        std::vector<int> segmentIndexes = rangeIncluded(0, n_segments - 1, 1);

        for(int i = 0; i < n_cols; i++){
            // If the number of missing Block is too great, just remove the entire row - start
            if(nMissingBlocks >= segmentIndexes.size()){
                for(int j=0; j<missing.n_rows; j++){
                    missing(start-1 + j, i) = NAN;
                }
            }

            // Segment the row in n part of subMissingBlockSize size
            // Pick one segement to remove at random.
            std::vector<int> segIdxCopy = segmentIndexes;
            for(int k = 0; k < nMissingBlocks; k++){
                const int r = std::rand() % segIdxCopy.size();
                int idx = segIdxCopy[r];
                segIdxCopy.erase(segIdxCopy.begin() + r);
                for (int j = 0; j < subMissingBlockSize; j++) {
                    missing(start + idx * subMissingBlockSize + j, i) = NAN;
                }
            }
        }
        return missing;
    }

    void Mcar::setSeed(unsigned int seed) {
        std::srand(seed);
    }
} // Scenarios