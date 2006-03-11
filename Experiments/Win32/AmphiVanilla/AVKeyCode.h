#ifndef AVKeyCode_h
#define AVKeyCode_h
#include "OpenVanilla.h"
#include <ctype.h>

class AVKeyCode : public OVKeyCode  {
public:
    AVKeyCode (int p = 0)
	{
		chr = toupper(p);
		isShift_ = isCapsLock_ = isCtrl_ = isAlt_ = isNum_ =0;
	}
    virtual int code()			{ return (isShift_||isCapsLock_||isCtrl_||isAlt_) ? chr : tolower(chr); }
    virtual int isShift()		{ return isShift_; }
    virtual int isCapslock()	{ return isCapsLock_; }
    virtual int isCtrl()		{ return isCtrl_; }
    virtual int isAlt()			{ return isAlt_; }
	virtual int isNum()		{ return isNum_; }
    
    virtual void setCode(int x)		{ chr = toupper(x); }
    virtual void setShift(int x)			{ isShift_ = x; }
    virtual void setCapslock(int x)	{ isCapsLock_ = x; }
    virtual void setCtrl(int x)			{ isCtrl_ = x; }
    virtual void setAlt(int x)			{ isAlt_ = x; }
	virtual void setNum(int x)			{ isNum_ = x; }
protected:
    int chr;
    int isShift_, isCapsLock_, isCtrl_, isAlt_, isNum_;
};
#endif // AVKeyCode_h
