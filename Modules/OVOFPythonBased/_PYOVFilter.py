# -*- coding: utf-8 -*-
import sys
import re

def process(inStr):
    
    #print >>sys.stderr, "The argument tuple is ", inStr
    
    unistr = inStr.decode("utf-8")
    
    # demos a basic string substitution
    r = re.compile(u"ㄅ")
    s = r.sub(u"吧", unistr)
    
    r = re.compile("a")
    s = r.sub("z", s)
    
    return s.encode("utf-8")
