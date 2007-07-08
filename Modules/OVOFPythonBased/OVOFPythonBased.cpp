// OVIMPythonBased.cpp: Loader of python based filter for OpenVanilla

// Originally written by Chen-Shan Chin, lukhnos, 2007

// This package is released under the Artistic License,
// please refer to LICENSE.txt for the terms of use and distribution

#define OV_DEBUG
#include <Python.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

class OVOFPythonBasedFilter : public OVOutputFilter {
public:                         

    OVOFPythonBasedFilter(const char *className) {
        
        PyObject *PYOVFilterModule;
        PyObject *PYOVFilterClass;
        
        //Py_Initialize();
        
        //PYOVFilterModule = PyImport_Import(PyString_FromString("PYOVFilter"));
        
        PYOVFilterClass = PyObject_GetAttrString(PYOVFilterModule, (char *) className);
        Py_DECREF(PYOVFilterModule);

        PYOVFilter = PyObject_CallObject(PYOVFilterClass, NULL);    
        Py_DECREF(PYOVFilterClass);

        if (PyErr_Occurred()) {
            PyErr_Print();
            PyErr_Clear();
        }
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath) {
        return 1;
    }
    
    virtual const char *identifier() {
        PyObject *pyRtnStr; 
        pyRtnStr = PyObject_CallMethodObjArgs(PYOVFilter, 
                                              PyString_FromString("identifier"), 
                                              NULL);
        char *result = PyString_AsString(pyRtnStr);
        Py_DECREF(pyRtnStr);  
        return result; 
    }
    
    virtual const char *process (const char *src, OVService *srv) {
        //fprintf(stderr, "passing string to Python: %s\n", src);
        
        srv->notify("Python Filter");
        
        //pyReturnValue = PyObject_CallFunctionObjArgs(PYOVFilter, pyString, NULL);
        PyObject *pyRtnStr;        
        pyRtnStr = PyObject_CallMethodObjArgs(PYOVFilter, 
                                              PyString_FromString("process"), 
                                              PyString_FromString(src), NULL);
        
        if (PyErr_Occurred()) {
            PyErr_Print();
            PyErr_Clear();
            return src;
        }
        
        char *result = PyString_AsString(pyRtnStr);
        Py_DECREF(pyRtnStr);
        return result;
    }
    
    
    ~OVOFPythonBasedFilter() {
        //Py_Finalize();
    }
    
private:

    PyObject *PYOVFilter;
};

extern "C" unsigned int OVGetLibraryVersion() {     
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    
    std::string scriptPath;
	scriptPath = std::string(p) + std::string(s->pathSeparator())  + std::string("OVOFPythonBased");
	std::cerr <<  "script load path = " << scriptPath << std::endl;
	setenv("PYTHONPATH", scriptPath.c_str(), 1);
	Py_Initialize(); 
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {

    PyObject *PYOVFilterModule;
    PYOVFilterModule = PyImport_Import(PyString_FromString("_PYOVHelper"));
    PyObject *pyAvaiableClasses;
    pyAvaiableClasses = PyObject_CallMethodObjArgs(PYOVFilterModule, 
                                                   PyString_FromString("getAllFilters"), 
                                                   NULL);
    int numClasses = PyList_Size(pyAvaiableClasses);
    char *className;
    if (idx < numClasses) {
         className = PyString_AsString(PyList_GetItem(pyAvaiableClasses,idx));
    } else {
        return NULL;
    }                                              
   
    return new OVOFPythonBasedFilter(className);
    
}
