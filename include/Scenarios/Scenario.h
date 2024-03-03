//
// Created by lucie on 08.03.2023.
//

#ifndef REDUCED_BENCH_SCENARIO_H
#define REDUCED_BENCH_SCENARIO_H

#include <armadillo>
#include <regex>

namespace Scenarios {

    class Scenario
    {
    public:
        virtual ~Scenario() = default;
        virtual arma::mat createMissing(arma::mat original, int tcase, std::vector<int> params) = 0;
    };

}
#endif //REDUCED_BENCH_SCENARIO_H
