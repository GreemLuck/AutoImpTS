#include <iostream>

#include "Performance/Benchmark.h"
#include "AutoParam/SuccessiveHalving.h"
#include "parser.h"
#include "sqlite3.h"
#include "Database/SQL.h"

using namespace std;
using namespace arma;
using namespace Algorithms;

void printSettings(settings &set);

int main(int argc, char **argv) {
    settings set;
    parse(argc, argv, set);

    if(set.autoH == "sh") {
        SuccessiveHalving(set);
        Database::sql_insert(set);
        printSettings(set);
    } else {
        if(!Database::is_in_db(set))
        {
            Performance::Start_Benchmark(set);
            Database::sql_insert(set);
        }
        if(!(set.algorithm == "trmf")) {
            Database::sql_insert(set);
            printSettings(set);
        }
    }

    return 0;
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
