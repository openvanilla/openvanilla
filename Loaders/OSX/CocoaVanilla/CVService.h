// CVService.h: Cocoa-Vanilla implementation of OVService
#ifndef __CVService_h
#define __CVSerivce_h

#include <OpenVanilla/OpenVanilla.h>
#include "CVInfoBox.h"

class CVService : public OVService {
public:
    CVService(NSString *usrpath, CVInfoBox *b);
    virtual ~CVService();
    virtual void beep();
    virtual void notify(const char *msg);
    virtual const char *locale();
    virtual const char *userSpacePath(const char *modid);
    virtual const char *pathSeparator();
    virtual const char *toUTF8(const char *encoding, const char *src);
    virtual const char *fromUTF8(const char *encoding, const char *src);
    virtual const char *UTF16ToUTF8(unsigned short *src, int len);
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr);
    
    virtual void closeNotification();
    virtual void fadeNotification();
    virtual void setNotificationPosition(Point p);
protected:
    CFStringRef u8buf;
    char *userbuf;
    UniChar *u16buf;
    
    NSString *lctag;
    NSString *userspace;
    CVInfoBox *infobox;
};

#endif
