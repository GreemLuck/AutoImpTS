//
// Created by lucie on 11/22/2022.
//

#include "Performance/MisalignRedux.h"

namespace MisalignRedux {

    arma::vec linear_interpolation(double start, double end, int size){
        arma::vec rv = arma::linspace(0, size-1, size);
        for(auto& value : rv){
            value = (start*(size-1 - value) + end*(value)) / size;
        }
        return rv;
    }
} // MisalignRedux