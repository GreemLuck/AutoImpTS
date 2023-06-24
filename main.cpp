#include <iostream>

#include "Performance/Benchmark.h"
#include "parser.h"
#include "sqlite3.h"
#include "Database/SQL.h"
#include <filesystem>

#include "Scenarios/MCAR.h"
#include "Scenarios/MissingBlock.h"

static const char *const NORMAL = "_normal.txt";
using namespace std;
using namespace arma;
using namespace Algorithms;

void printSettings(settings &set);
std::string DName2Folder(std::string dataset);
void delete_dir_content(const std::filesystem::path &path);
Scenarios::ScenarioContext createScenarioInstance(Scenarios::scenario_settings scenario);

int main(int argc, char **argv) {
    settings set;
    Scenarios::scenario_settings scenario;
    arma::mat reference;
    std::ifstream file;

    std::cout << "Starting the benchmark" << std::endl;
    parse(argc, argv, set, scenario);

    // Dataset matrix setup
    delete_dir_content("_data/in/");
    delete_dir_content("_data/out/");
    std::filesystem::create_directories(scenario.outpath);
    std::string dataFdName = DName2Folder(set.dataset);
    std::string fileName = set.dataset + NORMAL;
    std::string filePath = Performance::dataFolder + dataFdName + "/" + fileName;

    reference.load(filePath, arma::raw_ascii);

    // Prepare the scenario
    Scenarios::ScenarioContext scenarioContext = createScenarioInstance(scenario);
    scenarioContext.runScenario(reference, scenario);

    Performance::Start_Benchmark(set, scenario, reference);
//    printSettings(set);

    return 0;
}

Scenarios::ScenarioContext createScenarioInstance(Scenarios::scenario_settings scenario){
    switch(scenario.type){
        case Scenarios::MCAR:
            return Scenarios::ScenarioContext(std::make_unique<Scenarios::Mcar>());
        case Scenarios::MISSBLOCK:
            return Scenarios::ScenarioContext(std::make_unique<Scenarios::MissingBlock>());
    }
    return Scenarios::ScenarioContext(std::make_unique<Scenarios::Mcar>());
}

void delete_dir_content(const std::filesystem::path &path){
    for (const auto &entry: std::filesystem::directory_iterator(path)){
        std::filesystem::remove_all(entry);
    }
}

void printSettings(settings &set){
    map<string, double>::iterator it = set.params.begin();
    cout.precision(15);
    cout.flush();
    cout << "Dataset : " << set.dataset << endl <<
         "Algorithm : " << set.algorithm << endl;
    while(it != set.params.end()){
        cout << it->first << " : " << it->second << endl;
        it++;
    }
    cout << "Runtime : " << set.runtime << endl <<
         "RMSE : " << set.rmse << endl;
}

std::string DName2Folder(std::string dataset){
    if(dataset == "airq")
        return "air_quality";
    if(dataset == "drift10")
        return "drift";
    return dataset;
}
