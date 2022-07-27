#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <filesystem>

#include "Performance/Benchmark.h"
#include "parser.h"
#include "sqlite3.h"

#include <iostream>

void trmf_test();
void sql_insert(settings&);

std::string dataname2folder(std::string dataset){
    if(dataset == "airq")
        return "air_quality";
    if(dataset == "drift10")
        return "drift";
    return dataset;
}

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
    settings set {"airq", "cd", {}, 100, 0, 0};
    parse(argc, argv, set);

    //trmf_test();
    //sql_test();

    Performance::Start_Benchmark(set);

    cout.precision(15);
    cout.flush();
    cout << "Dataset : " << set.dataset << endl <<
            "Algorithm : " << set.algorithm << endl <<
            "Parameters : ";
    for(const auto& value: set.params)
        cout << value << ", ";
    cout << endl << "Ticks : " << set.tick << endl <<
            "Runtime : " << set.runtime << endl <<
            "RMSE : " << set.rmse << endl;

    sql_insert(set);

    return 0;
}

void trmf_test(){
    // Testing Python interpreter
    Py_Initialize();

    std::string currentPath = std::filesystem::current_path();

    string trmf_module_path = currentPath + "/Algorithm/";
    char char_path[trmf_module_path.length() +1];
    // TODO: add path to python interpreter and invoke the function in trmfpy.py
    const wchar_t module_path[] = L"Algorithm/";
    const char module_name[] = "trmfpy";

    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("Algorithms/"));
    PyObject * Module = PyImport_ImportModule(module_name);
    PyObject * Dict = PyModule_GetDict(Module);
    PyObject * Func = PyDict_GetItemString(Dict, "main");

    if(PyCallable_Check(Func)) {
        PyObject_CallObject(Func, NULL);
        PyErr_Print();
    }
    else
        PyErr_Print();

    Py_DECREF(Module);
    Py_DECREF(Dict);
    Py_DECREF(Func);

    Py_Finalize();
}

void sql_insert(settings &set){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Open database
    rc = sqlite3_open("Results", &db);

    if(rc){
        cerr << "Can't open the database: " << sqlite3_errmsg(db) << endl;
        return;
    } else {
        cout << "Opened database successfully" << endl;
    }

    std::ostringstream stringStream;
    stringStream << "INSERT INTO CDREC(Dataset,Ticks,Tolerance,Truncation,Max_iter,Runtime,Rmse) " <<
                    "VALUES('" << set.dataset << "'," << set.tick << "," << set.params[1] << "," <<
                    set.params[2] << "," << set.params[0] << "," << set.runtime << "," << set.rmse << ");";
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