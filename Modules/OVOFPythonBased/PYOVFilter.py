# -*- coding: utf-8 -*-
import sys
import re

class PYOVFilter(object):
  def __init__(self):
    sys.stderr.write("Init OV-Python module\n")
    pass
  def process(self,str):
    sys.stderr.write("Received: ")
    sys.stderr.write(str)
    sys.stderr.write("\n")
    
    unistr = str.decode("utf-8")
    
    # demos a basic string substitution
    r = re.compile(u"ㄅ")
    s = r.sub(u"吧", unistr)
    
    r = re.compile("a")
    s = r.sub("z", s)
    
    return s.encode("utf-8")
