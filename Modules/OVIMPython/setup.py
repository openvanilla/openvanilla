import distutils
from distutils.core import setup, Extension

setup(name = "Example in SWIG tutorial",
      version = "1.0",
      ext_modules = [Extension("_OVIMPython", ["OVIMPython.i"])])
