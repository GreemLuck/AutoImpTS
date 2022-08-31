//
// Created by lucie on 27/08/2022.
//
#define PY_SSIZE_T_CLEAN
#include "BayesOpt.h"
#include <Python.h>

void BayesOpt(settings &set){
    Py_Initialize();

    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    // Add BayesOpt module to path
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *pathModule = PyObject_GetAttrString(sys, "path");
    if(!pathModule){
        PyErr_Print();
        printf("Error with python path");
    }

    PyList_Append(pathModule, PyUnicode_FromString("./AutoParam"));

    PyObject* ModuleString = PyUnicode_FromString((char*) "BayesOpt");
    if (!ModuleString) {
        PyErr_Print();
        printf("Error formating python script\n");
    }

    PyObject* Module = PyImport_Import(ModuleString);
    if (!Module) {
        PyErr_Print();
        printf("Error importing python script\n");
    }

    PyObject * Dict = PyModule_GetDict(Module);
    PyObject * Func = PyDict_GetItemString(Dict, "main");
    PyObject * Result = PyObject_CallObject(Func, nullptr);

    if(!Result) {
        if (PyErr_Occurred()) PyErr_Print();
        else std::cerr << "Python exception without error set" << std::endl;
    } else {
        Py_DECREF(Result);
        std::cout << "Called the auto parametrizaiton function" << std::endl;
    }

    // Clean up
    Py_DECREF(Module);
    Py_DECREF(ModuleString);
    PyGILState_Release(gstate);

    Py_Finalize();
}