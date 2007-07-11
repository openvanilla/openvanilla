# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _OVIMPython

def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


try:
    from weakref import proxy as weakref_proxy
except:
    weakref_proxy = lambda x: x


class OVBase(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVBase, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, OVBase, name)
    def __repr__(self):
        return "<C OVBase instance at %s>" % (self.this,)
    def __del__(self, destroy=_OVIMPython.delete_OVBase):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self, *args):
        _swig_setattr(self, OVBase, 'this', _OVIMPython.new_OVBase(*args))
        _swig_setattr(self, OVBase, 'thisown', 1)

class OVBasePtr(OVBase):
    def __init__(self, this):
        _swig_setattr(self, OVBase, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVBase, 'thisown', 0)
        _swig_setattr(self, OVBase,self.__class__,OVBase)
_OVIMPython.OVBase_swigregister(OVBasePtr)

class OVKeyCode(OVBase):
    __swig_setmethods__ = {}
    for _s in [OVBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVKeyCode, name, value)
    __swig_getmethods__ = {}
    for _s in [OVBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVKeyCode, name)
    def __repr__(self):
        return "<C OVKeyCode instance at %s>" % (self.this,)
    def code(*args): return _OVIMPython.OVKeyCode_code(*args)
    def isShift(*args): return _OVIMPython.OVKeyCode_isShift(*args)
    def isCapslock(*args): return _OVIMPython.OVKeyCode_isCapslock(*args)
    def isCtrl(*args): return _OVIMPython.OVKeyCode_isCtrl(*args)
    def isAlt(*args): return _OVIMPython.OVKeyCode_isAlt(*args)
    def isOpt(*args): return _OVIMPython.OVKeyCode_isOpt(*args)
    def isCommand(*args): return _OVIMPython.OVKeyCode_isCommand(*args)
    def isNum(*args): return _OVIMPython.OVKeyCode_isNum(*args)
    def isFunctionKey(*args): return _OVIMPython.OVKeyCode_isFunctionKey(*args)
    def __init__(self, *args):
        if self.__class__ == OVKeyCode:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVKeyCode, 'this', _OVIMPython.new_OVKeyCode(*args))
        _swig_setattr(self, OVKeyCode, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVKeyCode):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVKeyCode(self)
        return weakref_proxy(self)

class OVKeyCodePtr(OVKeyCode):
    def __init__(self, this):
        _swig_setattr(self, OVKeyCode, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVKeyCode, 'thisown', 0)
        _swig_setattr(self, OVKeyCode,self.__class__,OVKeyCode)
_OVIMPython.OVKeyCode_swigregister(OVKeyCodePtr)

class OVBuffer(OVBase):
    __swig_setmethods__ = {}
    for _s in [OVBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVBuffer, name, value)
    __swig_getmethods__ = {}
    for _s in [OVBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVBuffer, name)
    def __repr__(self):
        return "<C OVBuffer instance at %s>" % (self.this,)
    def clear(*args): return _OVIMPython.OVBuffer_clear(*args)
    def append(*args): return _OVIMPython.OVBuffer_append(*args)
    def send(*args): return _OVIMPython.OVBuffer_send(*args)
    def update(*args): return _OVIMPython.OVBuffer_update(*args)
    def isEmpty(*args): return _OVIMPython.OVBuffer_isEmpty(*args)
    def __init__(self, *args):
        if self.__class__ == OVBuffer:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVBuffer, 'this', _OVIMPython.new_OVBuffer(*args))
        _swig_setattr(self, OVBuffer, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVBuffer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVBuffer(self)
        return weakref_proxy(self)

class OVBufferPtr(OVBuffer):
    def __init__(self, this):
        _swig_setattr(self, OVBuffer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVBuffer, 'thisown', 0)
        _swig_setattr(self, OVBuffer,self.__class__,OVBuffer)
_OVIMPython.OVBuffer_swigregister(OVBufferPtr)

class OVCandidate(OVBase):
    __swig_setmethods__ = {}
    for _s in [OVBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVCandidate, name, value)
    __swig_getmethods__ = {}
    for _s in [OVBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVCandidate, name)
    def __repr__(self):
        return "<C OVCandidate instance at %s>" % (self.this,)
    def clear(*args): return _OVIMPython.OVCandidate_clear(*args)
    def append(*args): return _OVIMPython.OVCandidate_append(*args)
    def hide(*args): return _OVIMPython.OVCandidate_hide(*args)
    def show(*args): return _OVIMPython.OVCandidate_show(*args)
    def update(*args): return _OVIMPython.OVCandidate_update(*args)
    def onScreen(*args): return _OVIMPython.OVCandidate_onScreen(*args)
    def __init__(self, *args):
        if self.__class__ == OVCandidate:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVCandidate, 'this', _OVIMPython.new_OVCandidate(*args))
        _swig_setattr(self, OVCandidate, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVCandidate):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVCandidate(self)
        return weakref_proxy(self)

class OVCandidatePtr(OVCandidate):
    def __init__(self, this):
        _swig_setattr(self, OVCandidate, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVCandidate, 'thisown', 0)
        _swig_setattr(self, OVCandidate,self.__class__,OVCandidate)
_OVIMPython.OVCandidate_swigregister(OVCandidatePtr)

class OVDictionary(OVBase):
    __swig_setmethods__ = {}
    for _s in [OVBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVDictionary, name, value)
    __swig_getmethods__ = {}
    for _s in [OVBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVDictionary, name)
    def __repr__(self):
        return "<C OVDictionary instance at %s>" % (self.this,)
    def keyExist(*args): return _OVIMPython.OVDictionary_keyExist(*args)
    def getInteger(*args): return _OVIMPython.OVDictionary_getInteger(*args)
    def setInteger(*args): return _OVIMPython.OVDictionary_setInteger(*args)
    def getString(*args): return _OVIMPython.OVDictionary_getString(*args)
    def setString(*args): return _OVIMPython.OVDictionary_setString(*args)
    def getIntegerWithDefault(*args): return _OVIMPython.OVDictionary_getIntegerWithDefault(*args)
    def getStringWithDefault(*args): return _OVIMPython.OVDictionary_getStringWithDefault(*args)
    def __init__(self, *args):
        if self.__class__ == OVDictionary:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVDictionary, 'this', _OVIMPython.new_OVDictionary(*args))
        _swig_setattr(self, OVDictionary, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVDictionary):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVDictionary(self)
        return weakref_proxy(self)

class OVDictionaryPtr(OVDictionary):
    def __init__(self, this):
        _swig_setattr(self, OVDictionary, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVDictionary, 'thisown', 0)
        _swig_setattr(self, OVDictionary,self.__class__,OVDictionary)
_OVIMPython.OVDictionary_swigregister(OVDictionaryPtr)

class OVService(OVBase):
    __swig_setmethods__ = {}
    for _s in [OVBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVService, name, value)
    __swig_getmethods__ = {}
    for _s in [OVBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVService, name)
    def __repr__(self):
        return "<C OVService instance at %s>" % (self.this,)
    def beep(*args): return _OVIMPython.OVService_beep(*args)
    def notify(*args): return _OVIMPython.OVService_notify(*args)
    def locale(*args): return _OVIMPython.OVService_locale(*args)
    def userSpacePath(*args): return _OVIMPython.OVService_userSpacePath(*args)
    def pathSeparator(*args): return _OVIMPython.OVService_pathSeparator(*args)
    def toUTF8(*args): return _OVIMPython.OVService_toUTF8(*args)
    def fromUTF8(*args): return _OVIMPython.OVService_fromUTF8(*args)
    def UTF16ToUTF8(*args): return _OVIMPython.OVService_UTF16ToUTF8(*args)
    def UTF8ToUTF16(*args): return _OVIMPython.OVService_UTF8ToUTF16(*args)
    def __init__(self, *args):
        if self.__class__ == OVService:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVService, 'this', _OVIMPython.new_OVService(*args))
        _swig_setattr(self, OVService, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVService):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVService(self)
        return weakref_proxy(self)

class OVServicePtr(OVService):
    def __init__(self, this):
        _swig_setattr(self, OVService, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVService, 'thisown', 0)
        _swig_setattr(self, OVService,self.__class__,OVService)
_OVIMPython.OVService_swigregister(OVServicePtr)

class OVInputMethodContext(OVBase):
    __swig_setmethods__ = {}
    for _s in [OVBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVInputMethodContext, name, value)
    __swig_getmethods__ = {}
    for _s in [OVBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVInputMethodContext, name)
    def __repr__(self):
        return "<C OVInputMethodContext instance at %s>" % (self.this,)
    def start(*args): return _OVIMPython.OVInputMethodContext_start(*args)
    def clear(*args): return _OVIMPython.OVInputMethodContext_clear(*args)
    def end(*args): return _OVIMPython.OVInputMethodContext_end(*args)
    def keyEvent(*args): return _OVIMPython.OVInputMethodContext_keyEvent(*args)
    def __init__(self, *args):
        if self.__class__ == OVInputMethodContext:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVInputMethodContext, 'this', _OVIMPython.new_OVInputMethodContext(*args))
        _swig_setattr(self, OVInputMethodContext, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVInputMethodContext):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVInputMethodContext(self)
        return weakref_proxy(self)

class OVInputMethodContextPtr(OVInputMethodContext):
    def __init__(self, this):
        _swig_setattr(self, OVInputMethodContext, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVInputMethodContext, 'thisown', 0)
        _swig_setattr(self, OVInputMethodContext,self.__class__,OVInputMethodContext)
_OVIMPython.OVInputMethodContext_swigregister(OVInputMethodContextPtr)

class OVModule(OVBase):
    __swig_setmethods__ = {}
    for _s in [OVBase]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVModule, name, value)
    __swig_getmethods__ = {}
    for _s in [OVBase]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVModule, name)
    def __repr__(self):
        return "<C OVModule instance at %s>" % (self.this,)
    def moduleType(*args): return _OVIMPython.OVModule_moduleType(*args)
    def identifier(*args): return _OVIMPython.OVModule_identifier(*args)
    def localizedName(*args): return _OVIMPython.OVModule_localizedName(*args)
    def initialize(*args): return _OVIMPython.OVModule_initialize(*args)
    def update(*args): return _OVIMPython.OVModule_update(*args)
    def __init__(self, *args):
        if self.__class__ == OVModule:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVModule, 'this', _OVIMPython.new_OVModule(*args))
        _swig_setattr(self, OVModule, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVModule):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVModule(self)
        return weakref_proxy(self)

class OVModulePtr(OVModule):
    def __init__(self, this):
        _swig_setattr(self, OVModule, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVModule, 'thisown', 0)
        _swig_setattr(self, OVModule,self.__class__,OVModule)
_OVIMPython.OVModule_swigregister(OVModulePtr)

class OVInputMethod(OVModule):
    __swig_setmethods__ = {}
    for _s in [OVModule]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVInputMethod, name, value)
    __swig_getmethods__ = {}
    for _s in [OVModule]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVInputMethod, name)
    def __repr__(self):
        return "<C OVInputMethod instance at %s>" % (self.this,)
    def newContext(*args): return _OVIMPython.OVInputMethod_newContext(*args)
    def moduleType(*args): return _OVIMPython.OVInputMethod_moduleType(*args)
    def __init__(self, *args):
        if self.__class__ == OVInputMethod:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVInputMethod, 'this', _OVIMPython.new_OVInputMethod(*args))
        _swig_setattr(self, OVInputMethod, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVInputMethod):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVInputMethod(self)
        return weakref_proxy(self)

class OVInputMethodPtr(OVInputMethod):
    def __init__(self, this):
        _swig_setattr(self, OVInputMethod, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVInputMethod, 'thisown', 0)
        _swig_setattr(self, OVInputMethod,self.__class__,OVInputMethod)
_OVIMPython.OVInputMethod_swigregister(OVInputMethodPtr)

class OVOutputFilter(OVModule):
    __swig_setmethods__ = {}
    for _s in [OVModule]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, OVOutputFilter, name, value)
    __swig_getmethods__ = {}
    for _s in [OVModule]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, OVOutputFilter, name)
    def __repr__(self):
        return "<C OVOutputFilter instance at %s>" % (self.this,)
    def process(*args): return _OVIMPython.OVOutputFilter_process(*args)
    def moduleType(*args): return _OVIMPython.OVOutputFilter_moduleType(*args)
    def __init__(self, *args):
        if self.__class__ == OVOutputFilter:
            args = (None,) + args
        else:
            args = (self,) + args
        _swig_setattr(self, OVOutputFilter, 'this', _OVIMPython.new_OVOutputFilter(*args))
        _swig_setattr(self, OVOutputFilter, 'thisown', 1)
    def __del__(self, destroy=_OVIMPython.delete_OVOutputFilter):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __disown__(self):
        self.thisown = 0
        _OVIMPython.disown_OVOutputFilter(self)
        return weakref_proxy(self)

class OVOutputFilterPtr(OVOutputFilter):
    def __init__(self, this):
        _swig_setattr(self, OVOutputFilter, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, OVOutputFilter, 'thisown', 0)
        _swig_setattr(self, OVOutputFilter,self.__class__,OVOutputFilter)
_OVIMPython.OVOutputFilter_swigregister(OVOutputFilterPtr)

ovkEsc = _OVIMPython.ovkEsc
ovkSpace = _OVIMPython.ovkSpace
ovkReturn = _OVIMPython.ovkReturn
ovkDelete = _OVIMPython.ovkDelete
ovkBackspace = _OVIMPython.ovkBackspace
ovkUp = _OVIMPython.ovkUp
ovkDown = _OVIMPython.ovkDown
ovkLeft = _OVIMPython.ovkLeft
ovkRight = _OVIMPython.ovkRight
ovkHome = _OVIMPython.ovkHome
ovkEnd = _OVIMPython.ovkEnd
ovkPageUp = _OVIMPython.ovkPageUp
ovkPageDown = _OVIMPython.ovkPageDown
ovkTab = _OVIMPython.ovkTab

