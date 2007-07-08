import os
import stat
import sys


class _PYOVFilterBase(object):
    """ Base class for python filters """
    def __init__(self):
        pass
    def identifier(self):
        return "OVOFPythonBasedFilter";
    def process(self, *args):
        return ""
     

class PYOVFilter(_PYOVFilterBase):
    """ 
    This is the class that wraps the real filter such that the real 
    filter can be modified and reload  after this wrapper is loaded. 
    """
    import _PYOVFilter
    timeStamp = os.stat(_PYOVFilter.__file__)[stat.ST_MTIME] 
    #count = 0
    
    def __init__(self):
        #print >>sys.stderr, "Init OV-Python module"
        pass                                
        
    def identifier(self):
        return "OVOFPythonBasedFilter1";
    
    def process(self, *arg):
        """Check if _PYOVFilter.py is modified, if so, reload it. 
           call _PYOVFilter.process(arg) at the end."""
            
        print >>sys.stderr, "the argument tuple is ", arg
      
        currentTimeStamp = os.stat(PYOVFilter._PYOVFilter.__file__)[stat.ST_MTIME]
        if currentTimeStamp != PYOVFilter.timeStamp:
            PYOVFilter.timeStamp = currentTimeStamp
            try:
                reload(PYOVFilter._PYOVFilter)
            except ImportError:
                print >>sys.stderr, "Import Error"
                raise ImportError

        return PYOVFilter._PYOVFilter.process(*arg)

import re   
class PYOVFilter2(_PYOVFilterBase):
    """ 
    This is the class that wraps the real filter such that the real 
    filter can be modified and reload  after this wrapper is loaded. 
    """

    def __init__(self):
        #print >>sys.stderr, "Init OV-Python module"
        pass                               
        
    def identifier(self):
        return "OVOFPythonBasedFilter2";    
    
    def process(self, *arg):
        inStr = arg[0]
        unistr = inStr.decode("utf-8").replace('a','s')
        return unistr.encode("utf-8")
