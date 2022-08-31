//
// Created by lucie on 15/08/2022.
//
#include <iostream>
#include <sstream>

#include "SQL.h"
#include "sqlite3.h"

using namespace std;
namespace Database {

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

std::string GetTableName(std::string &algorithm){
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
    } else if(algorithm == "dynammo"){
        return "Dynammo";
    } else if(algorithm == "svt"){
        return "SVT";
    } else if(algorithm == "rosl"){
        return "ROSL";
    } else if(algorithm == "itersvd"){
        return "IterSVD";
    } else if(algorithm == "softimpute"){
        return "SoftImpute";
    } else if(algorithm == "trmf"){
        return "TRMF";
    } else {
        return "";
    }
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
    streamValues.precision(15);
    while(it != set.params.end()){
        streamKeys << it->first << ",";
        streamValues << it->second << ",";
        it++;
    }
    paramsNames = streamKeys.str();
    paramsValues = streamValues.str();
    genericHeader = "Dataset,Ticks,";
    resultHeader = "Runtime,Rmse,Label";

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
    stringStream <<
                 "INSERT INTO " << table << "(" << genericHeader << paramsNames << resultHeader << ") " <<
                 "VALUES('" << set.dataset << "'," << set.tick << "," << paramsValues << set.runtime <<
                 "," << set.rmse;
    if(set.label.empty())
        stringStream << ",'');";
    else
        stringStream << ",'" << set.label << "');";

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
};
