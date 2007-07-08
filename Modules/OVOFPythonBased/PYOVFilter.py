# -*- coding: utf-8 -*-
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
     

class PYOVFilterDynamical(_PYOVFilterBase):
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
        return "PyFilter";
    
    def process(self, *arg):
        """Check if _PYOVFilter.py is modified, if so, reload it. 
           call _PYOVFilter.process(arg) at the end."""
            
        #print >>sys.stderr, "the argument tuple is ", arg
      
        currentTimeStamp = os.stat(PYOVFilterDynamical._PYOVFilter.__file__)[stat.ST_MTIME]
        #print >>sys.stderr, "time stamp is ", currentTimeStamp
        if currentTimeStamp != PYOVFilterDynamical.timeStamp:
            PYOVFilterDynamical.timeStamp = currentTimeStamp
            try:
                reload(PYOVFilterDynamical._PYOVFilter)
            except ImportError:
                print >>sys.stderr, "Import Error"
                raise ImportError

        return PYOVFilterDynamical._PYOVFilter.process(*arg)
  
class PYOVFilterHTMLEntity(_PYOVFilterBase):

    def __init__(self):
        #print >>sys.stderr, "Init OV-Python module"
        pass                               
        
    def identifier(self):
        return "PYHTMLEntity";    
    
    def process(self, *arg):
        entityMap = {u"&": u"&amp;",
                     u">": u"&gt;",
                     u"<": u"&lt;",
                     u"\"": u"&quot;",
                     u"'": u"&apos;"}
        uniStr = arg[0].decode("utf-8")              
        uniStr = "".join([ entityMap.get(x,x) for x in list(uniStr) ])
        return uniStr.encode("utf-8")
                                             
class PYOVFilterHalfToFull(_PYOVFilterBase):
    """ 
    This is the class that wraps the real filter such that the real 
    filter can be modified and reload  after this wrapper is loaded. 
    """

    def __init__(self):
        #print >>sys.stderr, "Init OV-Python module"
        pass                               

    def identifier(self):
        return "PYHalfToFull";    

    def process(self, *arg):
        
        #print >>sys.stderr, arg
        halfWidthChars=list(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
        fullWidthChars=list(u"　！”＃＄％＆’（）＊＋，－．／０１２３４５６７８９：；＜＝＞？＠ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ〔＼〕＾＿‘ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ｛｜｝～");
        
        charMap = dict(zip(halfWidthChars, fullWidthChars))
        
        
        uniStr = arg[0].decode("utf-8")              
        uniStr = "".join([ charMap.get(x,x) for x in list(uniStr) ])
        #print >>sys.stderr, uniStr.encode("utf-8")
        return uniStr.encode("utf-8")

class PYOVFilterMorseCode(_PYOVFilterBase):

    def __init__(self):
        #print >>sys.stderr, "Init OV-Python module"
        pass                               

    def identifier(self):
        return "PYMorseCode";    

    def process(self, *arg):
        alpheNumerics=('a','b','c','d','e','f','g','h','i','j','k',
                       'l','m','n','o','p','q','r','s','t','u','v',
                       'w','x','y','z','0','1','2','3','4','5','6',
                       '7','8','9','.', ',', '?')
        morseCode=(".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",
                   ".. ",".--- ","-.- ",".-.. ","-- ","-. ","--- ",".--. ",
                   "--.- ",".-. ","... ","- ","..- ","...- ",".-- ","-..- ",
                   "-.-- ","--.. ","----- ", ".---- ", "..--- ", "...-- ", 
                   "....- ", "..... ", "-.... ", "--... ", "---.. ", "----. ",
                   ".-.-.- ", "--..-- ", "..--.. ")        
        
        morseMap = dict(zip(alpheNumerics, morseCode))
        
        
        uniStr = arg[0].decode("utf-8")              
        uniStr = "".join([ morseMap.get(x,x) for x in list(uniStr) ])
        return uniStr.encode("utf-8")
        

class PYOVFilter1920(_PYOVFilterBase):

    def __init__(self):
        #print >>sys.stderr, "Init OV-Python module"
        pass                               

    def identifier(self):
        return "PY1920";    

    def process(self, *arg):
        fromThese = list(u"的吃吧嗎嘛只蜂裡啊個看")
        toThese = list(u"底喫罷麼麼祇蠭裏呦箇瞅")        
        
        map1920 = dict(zip(fromThese, toThese))
        
        
        uniStr = arg[0].decode("utf-8")              
        uniStr = "".join([ map1920.get(x,x) for x in list(uniStr) ])
        return uniStr.encode("utf-8")
        
