#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <string>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <Python.h>  

static PyObject *PYOVFilter;
static PyObject *PYOVFilterInstance;                        
static PyObject *PYOVFilter_process;

class OVOFPythonBasedFilter : public OVOutputFilter {
public:
    OVOFPythonBasedFilter() {
        buf = NULL;
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath) {
        return 1;
    }
    virtual const char *identifier() { return "OVOFPythonBasedFilter"; }
    
    virtual const char *process (const char *src, OVService *srv) {
        fprintf(stderr, "passing string to Python: %s\n", src);
        
        if (buf) free(buf);
        
        PyObject *pValue;
        PyObject *pArgs;
        pArgs =  PyString_FromString(src);        
        pValue = PyObject_CallMethodObjArgs(PYOVFilterInstance, PyString_FromString("process"), pArgs, NULL);

        char *result = PyString_AsString(pValue);
        buf = (char*)calloc(1, strlen(result + 1));
        strcpy(buf, result);
        return buf;
    }
    
protected:
    char *buf;
};

extern "C" unsigned int OVGetLibraryVersion() {     
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    std::string scriptPath;
	scriptPath = std::string(p) + std::string(s->pathSeparator())  + std::string("OVOFPythonBased");

	fprintf(stderr, "script load path = %s\n", scriptPath.c_str());	
	setenv("PYTHONPATH", scriptPath.c_str(), 1);
	
    //Load python here
    Py_Initialize();
    PyObject *pName = PyString_FromString("PYOVFilter");
    if (!pName) return 0;
    
    PyObject *pModule = PyImport_Import(pName);
    if (!pModule) return 0;
    
    Py_DECREF(pName);
    PYOVFilter = PyObject_GetAttrString(pModule, "PYOVFilter");
    if (!PYOVFilter) return 0;
        
    PYOVFilterInstance = PyObject_CallObject(PYOVFilter, NULL);    
    if (!PYOVFilterInstance) return 0;
    
    PYOVFilter_process = PyObject_GetAttrString(PYOVFilter, "process");
    if (!PYOVFilter_process) return 0;

    //de-reference PYOVFilter and PYOVFilterInstance?
    //Py_Finalize(); //should be called when OV exiting .... where should I put it?    
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    return (idx==0) ? new OVOFPythonBasedFilter : NULL;
}
