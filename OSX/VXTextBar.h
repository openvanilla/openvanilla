// VXTextBar.h

#ifndef __VXTextBar_h
#define __VXTextBar_h

#include <Carbon/Carbon.h>
#include "OpenVanilla.h"

class VXTextBar : public OVTextBar
{
public:
    VXTextBar(int fsize=vxtbDefFontSize);
    virtual ~VXTextBar();
    
    virtual int clear();
    virtual int append(void *s, OVEncoding e=ovEncodingUTF8, int l=0);
    virtual int onscreen() { return displaying; }
    virtual int hide();
    virtual int show();
    virtual int update();
    
protected:
    WindowRef window;
    ControlRef label;
    CFMutableStringRef text;
    int fontsize;
        
    int lookupdated;
    int textupdated;
    int posupdated;
    int displaying;
};    

#endif