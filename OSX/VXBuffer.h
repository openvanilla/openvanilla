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
    
    virtual int clear()
        { if (cimbuf) cimbuf->clear(); return 1; }
    virtual int send()
        { if (cimbuf) cimbuf->update(TRUE); return 1; }
    virtual int updatedisplay(int cursorpos=-1, int hilitefrom=-1, int hiliteto=-1)
    {
        if (cimbuf) cimbuf->update(FALSE, cursorpos, hilitefrom, hiliteto);
        return 1;
    }
    virtual int append (void *s, OVEncoding e=ovEncodingUTF8, int l=0)
    {
        int ol=length();
        CFStringRef ref=VXCreateCFString(s, e, l);
        if (!ref) return 0;
        if (cimbuf) cimbuf->put(ref);
        CFRelease(ref);
        return length()-ol;
    }
    
    virtual int length() { if (cimbuf) return cimbuf->length(); return 0; }

    virtual void bind(CIMInputBuffer *buf)
        { cimbuf=buf; }
    
protected:
    CIMInputBuffer *cimbuf;
};

#endif