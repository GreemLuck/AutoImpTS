//
// Created by lucie on 22.03.2023.
//

#ifndef REDUCED_BENCH_SCENARIOCONTEXT_H
#define REDUCED_BENCH_SCENARIOCONTEXT_H

#include <armadillo>
#include <regex>

#include "Scenario.h"

namespace Scenarios {

    enum Type{
        MCAR,
        MISSBLOCK
    };

    struct scenario_settings {
        Type type;
        std::string variables;
        std::string outpath;
        std::vector<std::string> input_paths;
    };

    Type valueOf(const std::string& type);

    std::vector<int> rangeIncluded(int from, int to, int step);

    class ScenarioContext {

    private:
        std::unique_ptr<Scenario> strategy_;

    public:
        explicit ScenarioContext(std::unique_ptr<Scenario> &&scenario = {})
            : strategy_(std::move(scenario))
        {}

        void runScenario(const arma::mat &original, scenario_settings &scenarioSettings);
    };

} // Scenarios

#endif //REDUCED_BENCH_SCENARIOCONTEXT_H
