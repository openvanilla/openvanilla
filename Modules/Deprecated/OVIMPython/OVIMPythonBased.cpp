// OVIMPythonBased.cpp: Loader of python based filter for OpenVanilla

// Originally written by Chen-Shan Chin, lukhnos, 2007

// This package is released under the Artistic License,
// please refer to LICENSE.txt for the terms of use and distribution

#define OV_DEBUG
#include <Python.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>


void logPyError() {
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
    }
}


char *SWIG_PackData(char *c, void *ptr, int sz) {      // copied from SWIG wrapper
  static char hex[17] = "0123456789abcdef";
  int i;
  unsigned char *u = (unsigned char *) ptr;
  register unsigned char uu;
  for (i = 0; i < sz; i++,u++) {
    uu = *u;
    *(c++) = hex[(uu & 0xf0) >> 4];
    *(c++) = hex[uu & 0xf];
  }
  return c;
}

PyObject *getPythonPointerObjectFrom(void *ptr, const char * type, PyObject * module) {  //helper function to pass C++ instance pointer to python interpreter
    PyObject *robjStr;
    PyObject *robj;
    char result[1024];
    char *r = result;
    *(r++) = '_';
    r = SWIG_PackData(r,&ptr,sizeof(void *));
    strcpy(r,"_p_");
    r+=3;
    strcpy(r,type);
    robjStr = PyString_FromString(result);
    
    std::string classPtrName = type;
    classPtrName += "Ptr";
    std::cout << classPtrName << std::endl;
    
    robj =  PyObject_CallMethodObjArgs(module,
                                       PyString_FromString( classPtrName.c_str() ),
                                       robjStr,
                                       NULL);
    logPyError();
    return robj; 
}
 

class OVIMPythonBasedContext : public OVInputMethodContext {
    public:
        OVIMPythonBasedContext(std::string pyClassName) {
            myClassName += pyClassName;
            myClassName += "Context";
            
            PyObject *PYIMModule; 
            PyObject *PYIMContextClass;

            
            OVIMPythonModule = PyImport_Import(PyString_FromString("OVIMPython")); 
            
            PYIMModule = PyImport_Import(PyString_FromString("PYIM"));    //elminate reload module in the future
            if (!PYIMModule) {std::cerr << "can not load PYIM.py, check syntex error ";}

            PYIMContextClass = PyObject_GetAttrString(PYIMModule, (char *) myClassName.c_str());
            Py_DECREF(PYIMModule);

            PYIMContext = PyObject_CallObject(PYIMContextClass, NULL);    
            Py_DECREF(PYIMContextClass);
            logPyError();
            
            
        }
        
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv){
        PyObject *kcObj;
        kcObj =  getPythonPointerObjectFrom(k, "OVKeyCode", OVIMPythonModule);
        
        PyObject *bufferObj;
        bufferObj =  getPythonPointerObjectFrom(b, "OVBuffer", OVIMPythonModule);  
        
        PyObject *candiateObj;
        candiateObj =  getPythonPointerObjectFrom(i, "OVCandidate", OVIMPythonModule);  
        
        PyObject *serviceObj;
        serviceObj =  getPythonPointerObjectFrom(srv, "OVService", OVIMPythonModule);  
        
        PyObject *pyRtnVal; 
        pyRtnVal = PyObject_CallMethodObjArgs(PYIMContext, 
                                              PyString_FromString("keyEvent"),
                                              kcObj,
                                              bufferObj,
                                              candiateObj,
                                              serviceObj, 
                                              NULL);
        logPyError();
        return 0;                         
    }   
    
    virtual void start(OVBuffer* b, OVCandidate* i, OVService* srv) {
        PyObject *bufferObj;
        bufferObj =  getPythonPointerObjectFrom(b, "OVBuffer", OVIMPythonModule);  
        
        PyObject *candiateObj;
        candiateObj =  getPythonPointerObjectFrom(i, "OVCandidate", OVIMPythonModule);  
        
        PyObject *serviceObj;
        serviceObj =  getPythonPointerObjectFrom(srv, "OVService", OVIMPythonModule);  
        
        PyObject *pyRtnVal; 
        pyRtnVal = PyObject_CallMethodObjArgs(PYIMContext, 
                                              PyString_FromString("start"),
                                              bufferObj,
                                              candiateObj,
                                              serviceObj, 
                                              NULL);
        logPyError();
    } 
     
    virtual void clear() {
        PyObject_CallMethodObjArgs(PYIMContext, 
                                   PyString_FromString("clear"),
                                   NULL);
    }
    
    virtual void end() {
        PyObject_CallMethodObjArgs(PYIMContext, 
                                   PyString_FromString("end"),
                                   NULL);
    }
    
    private:
        std::string myClassName;
        PyObject *OVIMPythonModule;
        PyObject *PYIMContext;
};

class OVIMPythonBased : public OVInputMethod {
    
public:                         
    OVIMPythonBased(const char *className) {
        
        myClassName = className;
        PyObject *PYIMModule;
        PyObject *PYIMClass;
        
        OVIMPythonModule = PyImport_Import(PyString_FromString("OVIMPython"));
        PYIMModule = PyImport_Import(PyString_FromString("PYIM"));
        if (!PYIMModule) {std::cerr << "can not load PYIM.py, check syntex error ";}
        
        PYIMClass = PyObject_GetAttrString(PYIMModule, (char *) className);
        Py_DECREF(PYIMModule);

        PYIM = PyObject_CallObject(PYIMClass, NULL);    
        Py_DECREF(PYIMClass);

        logPyError();
    }
    
    virtual const char *identifier() {
        PyObject *pyRtnStr;
        pyRtnStr = PyObject_CallMethodObjArgs(PYIM, 
                                              PyString_FromString("identifier"), 
                                              NULL);
        char *result = PyString_AsString(pyRtnStr);
        Py_DECREF(pyRtnStr);  
        return result; 
    }
    
    virtual OVInputMethodContext *newContext() { 
        return new OVIMPythonBasedContext(myClassName); 
    } 
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath) { 
        PyObject *cfgObj;
        cfgObj =  getPythonPointerObjectFrom(moduleCfg, "OVDictionary", OVIMPythonModule);
        PyObject *srvObj;
        srvObj =  getPythonPointerObjectFrom(srv, "OVService", OVIMPythonModule);
        PyObject *modulePathStr;
        modulePathStr = PyString_FromString(modulePath);
        PyObject *pyRtnVal; 
        pyRtnVal = PyObject_CallMethodObjArgs(PYIM, 
                                              PyString_FromString("initialize"),
                                              cfgObj,
                                              srvObj,
                                              modulePathStr, 
                                              NULL);
        return 1;  //should return the converted pyRtnVal
    } 
    
    virtual void update(OVDictionary *moduleCfg, OVService *srv) { 
        PyObject *cfgObj;
        cfgObj =  getPythonPointerObjectFrom(moduleCfg, "OVDictionary", OVIMPythonModule);
        PyObject *srvObj;
        srvObj =  getPythonPointerObjectFrom(srv, "OVService", OVIMPythonModule);
                    
        PyObject_CallMethodObjArgs(PYIM, 
                                   PyString_FromString("update"),
                                   cfgObj,
                                   srvObj,
                                   NULL);
      
    }
 
private:
    PyObject *PYIM;
    PyObject *OVIMPythonModule; 
    std::string myClassName;
};

extern "C" unsigned int OVGetLibraryVersion() {     
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    
    std::string scriptPath;
	scriptPath = std::string(p) + std::string(s->pathSeparator())  + std::string("OVIMPythonBased/");
	std::cerr <<  "script load path = " << scriptPath << std::endl;
	setenv("PYTHONPATH", scriptPath.c_str(), 1);
	Py_Initialize();
	std::cerr << "Python initialized in OVIMPython.\n"; 
	logPyError();
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {

    PyObject *PYOVIMModule;
    PYOVIMModule = PyImport_Import(PyString_FromString("_PYOVHelper"));
    std::cerr << "load _PYOVHelper.\n"; 
    PyObject *pyAvaiableClasses;
    pyAvaiableClasses = PyObject_CallMethodObjArgs(PYOVIMModule, 
                                                   PyString_FromString("getAllIM"), 
                                                   NULL);     
    std::cerr << "get all avaiable classes.\n";                                               
    int numClasses = PyList_Size(pyAvaiableClasses);
    char *className;
    if (idx < numClasses) {
         className = PyString_AsString(PyList_GetItem(pyAvaiableClasses, idx));
    } else {
        return NULL;
    }                                              
   
    return new OVIMPythonBased(className);
    
}
