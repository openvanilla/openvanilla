MODULEID=OVIMPythonBased
SOURCES=OVIMPythonBased
DATA=PYIM.py  _PYOVHelper.py OVIMPython.py  build/lib.darwin-8.10.0-Power_Macintosh-2.3/_OVIMPython.so

OVIMPython: OVIMPython.i
	/System/Library/Frameworks/Python.framework/Versions/Current/bin/python2.3 setup.py build_ext --swig-cpp -lpython -L/System/Library/Frameworks/Python.framework/Versions/2.3/lib/python2.3/config/  

include ../Mk/ov.module.mk

PYTHON_DIR=/System/Library/Frameworks/Python.framework/Headers
override CFLAGS=-I$(PYTHON_DIR) -F/Library/Frameworks/
override LDFLAGS=-lpython -dl
