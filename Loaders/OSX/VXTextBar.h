// VXTextBar.h

#ifndef __VXTextBar_h
#define __VXTextBar_h

#include <Carbon/Carbon.h>
#include <OpenVanilla/OpenVanilla.h>

const int vxtbDefFontSize=24;

class VXTextBar : public OVTextBar
{
public:
    VXTextBar(int fsize=vxtbDefFontSize);
    virtual ~VXTextBar();
    
    virtual OVTextBar* clear();
    virtual OVTextBar* append(void *s, OVEncoding e=ovEncodingUTF8, int l=0);
    virtual OVTextBar* hide();
    virtual OVTextBar* show();
    virtual OVTextBar* update();
    virtual int onScreen() { return displaying; }
    
    virtual int setPosition(int x, int y);
    virtual void getPosition(int *x, int *y);
	virtual void setFontSize(int s);
	virtual VXTextBar* lock();
	virtual VXTextBar* unlock();
	
protected:
    WindowRef window;
    ControlRef label;
    CFMutableStringRef text;
    Point pos;
    int fontsize;
        
    int lookupdated;
    int textupdated;
    int posupdated;
    int displaying;
	int locked;
};    

#endif
