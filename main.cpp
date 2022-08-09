#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <filesystem>
#include <algorithm>

#include "Performance/Benchmark.h"
#include "AutoParam/SuccessiveHalving.h"
#include "parser.h"
#include "sqlite3.h"

#include <iostream>

void sql_insert(settings&);
void SuccessiveHalving();

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

using namespace std;
using namespace arma;
using namespace Algorithms;

int main(int argc, char **argv) {
    settings set;
    parse(argc, argv, set);

    if(set.autoH)
        SuccessiveHalving(set);
    else
        Performance::Start_Benchmark(set);

    map<string, double>::iterator it = set.params.begin();
    cout.precision(15);
    cout.flush();
    cout << "Dataset : " << set.dataset << endl <<
         "Algorithm : " << set.algorithm << endl;
    while(it != set.params.end()){
        cout << it->first << " : " << it->second << endl;
    }
    cout << "Ticks : " << set.tick << endl <<
         "Runtime : " << set.runtime << endl <<
         "RMSE : " << set.rmse << endl;

    sql_insert(set);

    return 0;
}

string GetTableName(string &algorithm){
    if(algorithm == "cd"){
        return "CDREC";
    } else if(algorithm == "tkcm"){
        return "TKCM";
    } else if(algorithm == "st-mvl") {
        algorithm = "ST_MVL";
    } else if(algorithm == "spirit"){
        algorithm = "Spirit";
    } else if(algorithm == "grouse"){
        return "Grouse";
    } else if(algorithm == "nnmf"){
        return "NNMF";
    } else if(algorithm == "dynnamo"){
        return "Dynnamo";
    } else if(algorithm == "svt"){
        return "SVT";
    } else if(algorithm == "rosl"){
        return "ROSL";
    } if(algorithm == "itersvd") {
        return "IterSVD";
    } else if(algorithm == "softimpute"){
        return "SoftImpute";
    }
}

string vec2str(std::vector<double> vec){
    ostringstream oss;
    if(!vec.empty()){
        // Convert all element and add ","
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<double>(oss, ","));
    }
    return oss.str();
}

void sql_insert(settings &set){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string paramsValues, paramsNames, genericHeader, resultHeader, table;

    // Format Parameters keys and values
    map<string, double>::iterator it = set.params.begin();
    ostringstream streamKeys;
    ostringstream streamValues;
    while(it != set.params.end()){
        streamKeys << it->first << ",";
        streamValues << it->second << ",";
    }
    paramsNames = streamKeys.str();
    paramsValues = streamValues.str();
    genericHeader = "Dataset,Ticks,";
    resultHeader = "Runtime,Rmse";

    // Get the table name
    table = GetTableName(set.algorithm);

    // Open database
    rc = sqlite3_open("Results", &db);

    if(rc){
        cerr << "Can't open the database: " << sqlite3_errmsg(db) << endl;
        return;
    } else {
        cout << "Opened database successfully" << endl;
    }

    std::ostringstream stringStream;
    stringStream.precision(15);
    stringStream << "INSERT INTO " << table << "(" << genericHeader << paramsNames << resultHeader << ") " <<
                    "VALUES('" << set.dataset << "'," << set.tick << "," << paramsValues << set.runtime <<
                    "," << set.rmse << ");";
    const string& tmp = stringStream.str();
    const char *sql = tmp.c_str();

    cout << sql << endl;

    rc = sqlite3_exec(db ,sql, callback, 0, &zErrMsg);
    sqlite3_close(db);

    if(rc != SQLITE_OK){
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    } else {
        cout << "Inserted successfully" << endl;
    }
}