//
// Created by lucie on 22.03.2023.
//

#include "Scenarios/ScenarioContext.h"

namespace Scenarios {

    Type valueOf(const std::string& type){
        if(type == "MCAR")
            return MCAR;
        if(type == "MISSINGBLOCK")
            return MISSBLOCK;
        throw std::invalid_argument("Scenario does not exist. Accepted scenarios: MCAR, MISSINGBLOCK");
    }

    std::vector<int> rangeIncluded(int from, int to, int step) {
        std::vector<int> result;
        for (int i = from; i <= to; i += step) {
            result.push_back(i);
        }
        return result;
    }

    void ScenarioContext::runScenario(const arma::mat &original, scenario_settings &scenarioSettings){
        std::vector<int> params;
        std::vector<int> tcases;
        std::vector<int> scenarioVariables;

        if (!scenarioSettings.variables.empty()) {
            std::istringstream ss(scenarioSettings.variables);
            std::string s;
            while (getline(ss, s, ',')) {
                scenarioVariables.push_back(std::stoi(s));
            }

            if (scenarioVariables.size() < 3) {
                throw std::invalid_argument(
                        "scenv is expecting at least 3 parameters, received " + std::to_string(params.size()));
            }
            tcases = rangeIncluded(scenarioVariables[0], scenarioVariables[1], scenarioVariables[2]);
            params = std::vector<int>(scenarioVariables.begin() + 3, scenarioVariables.end());
        } else {
            tcases = rangeIncluded(10, 80, 10);
            scenarioSettings.variables = "10,80,10";
        }

        for (auto t: tcases) {
            arma::mat missing = strategy_->createMissing(original, t, params);
            std::string filename = "data_" + std::to_string(t) + ".txt";
            missing.save(scenarioSettings.outpath + filename, arma::raw_ascii);
            scenarioSettings.input_paths.push_back(scenarioSettings.outpath + filename);
        }
    }
} // Scenarios