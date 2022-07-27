//
// Created by lucie on 7/15/2022.
//

#include "parser.h"
#include "optionparser.h"
#include <cmath>

using namespace std;

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
                {HELP,    0, "", "help", option::Arg::None, "  --help  \tPrint usage and exit."},
                {ALG, 0, "", "alg", option::Arg::Optional, "  --alg \tSet algorithm."},
                {PARAMS, 0, "", "set-params", option::Arg::Optional, "  --set-params \tSet the algorithm parameters"},
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

    // algo setup
    if (options[ALG])
        set.algorithm = options[ALG].arg;

    // param setup
    vector<double> params;
    if (options[PARAMS]) {
        std::string s = options[PARAMS].arg;
        params = tokenize(s);
    }
    set.params = params;

    // tick setup
    if (options[TICK])
        set.tick = std::stoull(options[TICK].arg);

    return 2;
}
