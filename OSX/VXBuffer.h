// VXBuffer.h

#ifndef __VXBuffer_h
#define __VXBuffer_h

#include <Carbon/Carbon.h>
#include "OpenVanilla.h"
#include "VXUtility.h"

class VXBuffer : public OVBuffer
{
public:
    VXBuffer() : cimbuf(NULL) {}
    
    virtual OVBuffer* clear()
        { if (cimbuf) cimbuf->clear(); return this; }
    virtual OVBuffer* send()
        { if (cimbuf) cimbuf->update(TRUE); return this; }
    virtual OVBuffer* updatedisplay(int cursorpos=-1, int hilitefrom=-1, 
        int hiliteto=-1)
    {
        if (cimbuf) cimbuf->update(FALSE, cursorpos, hilitefrom, hiliteto);
        return this;
    }
    virtual OVBuffer* append (void *s, OVEncoding e=ovEncodingUTF8, int l=0)
    {
        CFStringRef ref=VXCreateCFString(s, e, l);
        if (!ref) return this;
        if (cimbuf) cimbuf->put(ref);
        CFRelease(ref);
        return this;
    }
    
    virtual int length() { if (cimbuf) return cimbuf->length(); return 0; }

    virtual void bind(CIMInputBuffer *buf)
        { cimbuf=buf; }
    
protected:
    CIMInputBuffer *cimbuf;
};

#endif