// OVIMExample.cpp

// #include <stdio.h>
#include "openvanilla.h"

class OVExampleIM;

class OVExampleSession : public OVIMSession
{
public:
    OVExampleSession(OVExampleIM *p) : parent(p)
    {
        // fprintf (stderr, "new im session instance created\n");
    }
    
    virtual ~OVExampleSession()
    {
        // fprintf (stderr, "im session instance destroyed\n");
    }
    
    virtual int activate(OVService *)
    {
        // fprintf (stderr, "im session activated\n");
    }
    
    virtual int deactivate(OVService *)
    {
        // fprintf (stderr, "im session deactivated\n");
    }
    
    virtual int keyevent(OVKeyCode *key, OVBuffer *buf,
        OVTextBar *textbar, OVService *srv)
    {
        // fprintf (stderr, "recevied key code=%d\n", key->code());
    
        if (key->iscode(3, 0xa, 0xd, 3))
        {
            // fprintf (stderr, "enter key\n");
            if (!buf->length()) return 0;
            buf->send();
            textbar->clear();
            // fprintf (stderr, "buffer not empty, sending\n");
            return 1;
        }
        
        if (key->code()=='a')
        {
            textbar->append("中");
            textbar->update();
            buf->append("中");
            buf->updatedisplay();
            // fprintf (stderr, "a chinese is inputed\n");
        }
        
        if (key->isprintable())
        {
            char str[2];
            str[1]=0;
            str[0]=key->code();
            // fprintf (stderr, "key=%s\n", str);
            textbar->append(str);
            textbar->update();
            buf->append(str);
            buf->updatedisplay();
        }
        
        return 0;
    }
    
protected:
    OVExampleIM *parent;
};

class OVExampleIM : public OVInputMethod
{
public:
    OVExampleIM()
    {
        // fprintf (stderr, "new IM instance created\n");
    }
    
    virtual ~OVExampleIM()
    {
        // fprintf (stderr, "IM instance destroyed\n");
    }
    
    OVIMSession *newsession() { return new OVExampleSession(this); }
};


extern "C" OVExampleIM *create()
{
    return new OVExampleIM;
}

extern "C" void destroy(OVExampleIM *o)
{
    delete o;
}
