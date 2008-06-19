MODULEID=OVOFPythonBased
SOURCES=OVOFPythonBased
DATA=PYOVFilter.py _PYOVFilter.py _PYOVHelper.py

include ../Mk/ov.module.mk

PYTHON_DIR=/System/Library/Frameworks/Python.framework/Headers
override CFLAGS=-I$(PYTHON_DIR) -F/Library/Frameworks/
override LDFLAGS=-lpython -dl
