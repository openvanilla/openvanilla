# -*- coding: utf-8 -*-
import sys
import OVIMPython

class PYIMTestContext(OVIMPython.OVInputMethodContext):
    def keyEvent(self, keycode, buf, candidate, srv): 
        print >>sys.stderr, keycode
        print >>sys.stderr, buf
        print >>sys.stderr, candidate
        print >>sys.stderr, srv
                           
        char = chr(keycode.code());
        print >>sys.stderr, char
        buf.append(char).update()
        srv.notify(char)
        candidate.show()
        candidate.append(char).update()
        return 0
        

class PYIMTest(OVIMPython.OVInputMethod):
    def identifier(self):
        return "OVIMTest"
