//
// Created by lucie on 7/15/2022.
//

#include "parser.h"
#include "optionparser.h"
#include <cmath>
#include <vector>

using namespace std;

void parseParams(map<string, double> params, option::Option options);

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

const option::Descriptor usage[] =
        {
                {UNKNOWN, 0, "", "",     option::Arg::None, "USAGE: example [options]\n\n"
                                                            "Options:"},
                {HELP,    0, "", "help", option::Arg::None, "  --help \tPrint usage and exit."},
                {AUTO, 0, "a", "auto", option::Arg::None, "  --auto \tAutomatically set hyperparameters."},
                {ALG, 0, "", "alg", option::Arg::Optional, "  --alg \tSet algorithm."},
                {PARAMS, 0, "", "set-params", option::Arg::Optional, "  --set-params \tSet the algorithm parameters"},
                {TRUNCATION, 0, "", "set-truncation", option::Arg::Optional, " --set-truncation \tSet the truncation parameter"},
                {MAX_ITER, 0, "", "set-max-iter", option::Arg::Optional, " --set-max-iter \tSet the max iteration parameter"},
                {THRESHOLD, 0, "", "set-threshold", option::Arg::Optional, " --set-threshold \tSet the threshold parameter"},
                {D, 0, "", "set-d", option::Arg::Optional, " --set-d \tSet the d parameter"},
                {ALPHA, 0, "", "set-alpha", option::Arg::Optional, " --set-alpha \tSet the alpha parameter"},
                {GAMMA, 0, "", "set-gamma", option::Arg::Optional, " --set-gamma \tSet the gamma parameter"},
                {WIN_SIZE, 0, "", "set-win-size", option::Arg::Optional, " --set-win-size \tSet the window size parameter"},
                {LAMBDA, 0, "", "set-lambda", option::Arg::Optional, " --set-lambda \tSet the lambda parameter"},
                {TAUSCALE, 0, "", "set-tauscale", option::Arg::Optional, " --set-tauscale \tSet the tauscale parameter"},
                {DATASET, 0, "d", "dataset", option::Arg::Optional, "  --dataset -d \tSet the dataset to work on."},
                {TICK, 0, "t", "tick", option::Arg::Optional, "  --tick -t \tSet the blocksize (per mille)."},
                {UNKNOWN, 0, "", "",     option::Arg::None, "\nExamples:\n"
                                                            "  example --unknown -- --this_is_no_option\n"
                                                            "  example -unk --plus -ppp file1 file2\n"},
                {0,0,0,0,0,0}
        };

int parse(int argc, char **argv, settings &set){
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

    // tick setup
    if (options[TICK])
        set.tick = std::stoull(options[TICK].arg);
    else
        set.tick = 100;

    // Automatisation option
    if (options[AUTO])
        set.autoH = true;
    else
        set.autoH = false;

    map<string, double> params;
    parseParams(params, *options);

    return 2;
}

void parseParams(map<string, double> &params, option::Option options){
    if(options[TRUNCATION])
        params["Truncation"] = std::stod(options[TRUNCATION].arg);
    if(options[MAX_ITER])
        params["Max_Iter"] = std::stod(options[MAX_ITER].arg);
    if(options[THRESHOLD])
        params["Threshold"] = std::stod(options[THRESHOLD].arg);
    if(options[D])
        params["D"] = std::stod(options[D].arg);
    if(options[ALPHA])
        params["Alpha"] = std::stod(options[ALPHA].arg);
    if(options[GAMMA])
        params["Gamma"] = std::stod(options[GAMMA].arg);
    if(options[WIN_SIZE])
        params["Win_Size"] = std::stod(options[WIN_SIZE].arg);
    if(options[LAMBDA])
        params["Lambda"] = std::stod(options[LAMBDA].arg);
    if(options[TAUSCALE])
        params["Tauscale"] = std::stod(options[LAMBDA].arg);
}
