//
// Created by lucie on 7/15/2022.
//

#include "parser.h"
#include "optionparser.h"
#include <cmath>
#include <vector>

using namespace std;
int valueOfScenarioType(std::string);

vector<double> tokenize(string s, string del = ",")
{
    vector<double> result;
    int start = 0;
    int end = s.find(del);
    string token;
    while (end != -1) {
        token = s.substr(start, end - start);
        if(token == "-")
            result.push_back(nan(""));
        else
            result.push_back(stod(token));
        start = end + del.size();
        end = s.find(del, start);
    }
    token = s.substr(start, end - start);
    result.push_back(stod(token));
    return result;
}

string parseScenarioVariables(option::Option options[], int optionsCount, const string& argKey) {
    string concatenatedArgs;
    for (int i = 0; i < optionsCount; ++i) {
        string s, str;
        if(options[i].name != nullptr)
            s = options[i].name;
        else
            continue;
        stringstream ss(s);
        getline(ss, str, '=');
        if (str == argKey) {
            if (!concatenatedArgs.empty()) concatenatedArgs += ",";
            concatenatedArgs += string(options[i].arg).substr(string(options[i].arg).find('=') + 1);
        }
    }
    return concatenatedArgs ;
}

const option::Descriptor usage[] =
        {
                {UNKNOWN, 0, "", "",     option::Arg::None,
                 "USAGE: example [options]\n\nOptions:"},
                {HELP,    0, "", "help", option::Arg::None,
                 "  --help \tPrint usage and exit."},
                {MULTI_T, 0, "m", "multi-thread", option::Arg::Optional,
                 "  --multi-thread \tAllow multi threading."},
                {MISALIGNED, 0, "", "misaligned", option::Arg::Optional,
                 "  --misaligned \tSet the misalignment reconstruction."},
                {ALG, 0, "", "alg", option::Arg::Optional,
                 "  --alg \tSet algorithm."},
                {PARAMS, 0, "", "set-params", option::Arg::Optional,
                 "  --set-params \tSet the algorithm parameters"},
                {SCENARIO_TYPE, 0, "s", "scenario", option::Arg::Optional,
                 "  --scenario, -s \tSet the type of scenario (MCAR,MISSINGBLOCK)."},
                {SCENARIO_VARIABLES, 0, "", "scenv", option::Arg::Optional,
                 "  --scenv \tSet the scenario variables as {v1,v2,v3,...} "
                 "\t\t - mcar : from, to, steps, block size, # of blocks"
                 "\t\t - missp : from, to, steps, affected column, starting row"},
                {SCENARIO_OUTPATH, 0, "", "sceno", option::Arg::Optional,
                 "  --sceno \tSaves the scenario resulting matrices in the given path"},
                {TRUNCATION, 0, "", "set-truncation", option::Arg::Optional,
                 " --set-truncation \tSet the truncation parameter"},
                {MAX_ITER, 0, "", "set-max-iter", option::Arg::Optional,
                 " --set-max-iter \tSet the max iteration parameter"},
                {TOLERANCE, 0, "", "set-tolerance", option::Arg::Optional,
                 " --set-tolerance \tSet the threshold parameter"},
                {D, 0, "", "set-d", option::Arg::Optional,
                 " --set-d \tSet the d parameter"},
                {ALPHA, 0, "", "set-alpha", option::Arg::Optional,
                 " --set-alpha \tSet the alpha parameter"},
                {GAMMA, 0, "", "set-gamma", option::Arg::Optional,
                 " --set-gamma \tSet the gamma parameter"},
                {WIN_SIZE, 0, "", "set-win-size", option::Arg::Optional,
                 " --set-win-size \tSet the window size parameter"},
                {LAMBDA, 0, "", "set-lambda", option::Arg::Optional,
                 " --set-lambda \tSet the lambda parameter"},
                {TAUSCALE, 0, "", "set-tauscale", option::Arg::Optional,
                 " --set-tauscale \tSet the tauscale parameter"},
                {LAMBDA_I, 0, "", "set-lambdaI", option::Arg::Optional,
                 " --set-lambdaI \tSet the lambdaI parameter"},
                {LAMBDA_AR, 0, "", "set-lambdaAR", option::Arg::Optional,
                 " --set-lambdaAR \tSet the lambdaAR parameter"},
                {LAMBDA_LAG, 0, "", "set-lambdaLag", option::Arg::Optional,
                 " --set-lambdaLag \tSet the lambdaLag parameter"},
                {DATASET, 0, "d", "dataset", option::Arg::Optional,
                 "  --dataset -d \tSet the dataset to work on."},
                {LABEL, 0, "l", "label", option::Arg::Optional,
                 "  --label -d \tSet a label to recognize the run in the database."},
                {UNKNOWN, 0, "", "",     option::Arg::None,
                 "\nExamples:\n"
                 "  example --unknown -- --this_is_no_option\n"
                 "  example -unk --plus -ppp file1 file2\n"},
                {0,0,0,0,0,0}
        };

int parse(int argc, char **argv, settings &set, Scenarios::scenario_settings &scenarioSettings){
    argc -= (argc > 0); argv += (argc > 0); // skip program name argv[0] if present
    option::Stats stats(usage, argc, argv);
    option::Option options[stats.options_max], buffer[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);

    if (parse.error())
        return 1;
    if (options[HELP]) {
        option::printUsage(std::cout, usage);
        return 0;
    }

    // Catching incorrect options
    for (option::Option *opt = options[UNKNOWN]; opt; opt = opt->next())
        std::cout << "Unknown option: " << opt->name << "\n";
    for (int i = 0; i < parse.nonOptionsCount(); ++i)
        std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";

    set.rmse =0;
    set.runtime = 0;

    // dataset setup
    if (options[DATASET])
        set.dataset = options[DATASET].arg;
    else
        set.dataset = "airq";

    // algo setup
    if (options[ALG])
        set.algorithm = options[ALG].arg;
    else
        set.algorithm = "cd";

    // Automatisation option
    if (options[MULTI_T])
        set.multi_t = true;
    else
        set.multi_t = false;

    // label
    if (options[LABEL])
        set.label = options[LABEL].arg;

    if(options[MISALIGNED])
        set.misaligned = true;
    else
        set.misaligned = false;

    if(options[SCENARIO_TYPE])
        scenarioSettings.type = Scenarios::valueOf(options[SCENARIO_TYPE].arg);
    if(options[SCENARIO_VARIABLES]) 
        scenarioSettings.variables = parseScenarioVariables(buffer, stats.buffer_max, "--scenv");
    if(options[SCENARIO_OUTPATH]) {
        scenarioSettings.outpath = options[SCENARIO_OUTPATH].arg;
        if(scenarioSettings.outpath.back() != '/'){
            scenarioSettings.outpath += '/';
        }
    }
    else
        scenarioSettings.outpath = "_data/in/";

    if(options[TRUNCATION])
        set.params["Truncation"] = std::stod(options[TRUNCATION].arg);
    if(options[MAX_ITER])
        set.params["Max_Iter"] = std::stod(options[MAX_ITER].arg);
    if(options[TOLERANCE])
        set.params["Tolerance"] = std::stod(options[TOLERANCE].arg);
    if(options[D])
        set.params["D"] = std::stod(options[D].arg);
    if(options[ALPHA])
        set.params["Alpha"] = std::stod(options[ALPHA].arg);
    if(options[GAMMA])
        set.params["Gamma"] = std::stod(options[GAMMA].arg);
    if(options[WIN_SIZE])
        set.params["Win_Size"] = std::stod(options[WIN_SIZE].arg);
    if(options[LAMBDA])
        set.params["Lambda"] = std::stod(options[LAMBDA].arg);
    if(options[TAUSCALE])
        set.params["Tauscale"] = std::stod(options[TAUSCALE].arg);
    if(options[LAMBDA_I])
        set.params["LambdaI"] = std::stod(options[LAMBDA_I].arg);
    if(options[LAMBDA_AR])
        set.params["LambdaAR"] = std::stod(options[LAMBDA_AR].arg);
    if(options[LAMBDA_LAG])
        set.params["LambdaLag"] = std::stod(options[LAMBDA_LAG].arg);

    return 2;
}