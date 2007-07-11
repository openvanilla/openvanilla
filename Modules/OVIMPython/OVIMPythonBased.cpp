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
   if (PyErr_Occurred()) {
       PyErr_Print();
       PyErr_Clear();
   }
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

            if (PyErr_Occurred()) {
                PyErr_Print();
                PyErr_Clear();
            }
            
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
        if (PyErr_Occurred()) {
            PyErr_Print();
            PyErr_Clear();
        }
        return 0;                         
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
        
        PYIMModule = PyImport_Import(PyString_FromString("PYIM"));
        if (!PYIMModule) {std::cerr << "can not load PYIM.py, check syntex error ";}
        
        
        PYIMClass = PyObject_GetAttrString(PYIMModule, (char *) className);
        Py_DECREF(PYIMModule);

        PYIM = PyObject_CallObject(PYIMClass, NULL);    
        Py_DECREF(PYIMClass);

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
    
 
private:
    PyObject *PYIM;
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
	if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
    }
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
    std::cerr << "get all avaiable class.\n";                                               
    int numClasses = PyList_Size(pyAvaiableClasses);
    char *className;
    if (idx < numClasses) {
         className = PyString_AsString(PyList_GetItem(pyAvaiableClasses, idx));
    } else {
        return NULL;
    }                                              
   
    return new OVIMPythonBased(className);
    
}
