// OVLoader.mm: an implementation example of CarbonInputMethod's IM code

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "IMInterface.h"

MenuRef ovMenu=nil;

#define DEBUG

#ifdef DEBUG
    #define MURMUR(x)   fprintf(stderr, "%s\n", x);
#else
    #define MURMUR(x)
#endif

int IMInitialize(MenuRef mnu)
{
    // we retain the menu for future use
    ovMenu=mnu;
    MURMUR("IM: input method initialized");
    return 1;
}

int IMContextOpen(IMContext *c)
{
    MURMUR("IM: input method context created");
    // TSBundle doesn't care about the data, so if we fail to create
    // our own context data, we must return 0. Here we do nothing
    
    return 1;
}

void IMContextClose(IMContext *c)
{
    MURMUR("IM: input method context closed");
}

void IMContextActivate(IMContext *c)
{
    MURMUR("IM: input method context activated");
}

void IMContextDeactivate(IMContext *c)
{
    MURMUR("IM: input method context deactivated");
}

void IMContextFix(IMContext *c)
{
    MURMUR("IM: input method context fix (sending rest of the buffer out)");
    c->buf->send();
}

int IMContextEvent(IMContext *c, char charcode, unsigned int modifiers)
{
    char msg[256];
    sprintf(msg, "IM: input method context event, char=0x%x, modifier=0x%x",
        (unsigned char)charcode, modifiers);
    MURMUR(msg);

    if (isprint(charcode))
    {
        char s[2];
        sprintf(s, "%c", charcode);
        NSString *ns=[[NSString alloc] initWithCString: s];
        c->buf->append(ns)->update();
        [ns release];
        return 1;
    }
    
    if (charcode==13 && !c->buf->isEmpty())
    {
        NSMutableString *s=c->buf->getContent();
        
        for (unsigned int i=0; i<[s length]; i++)
        {
            if (islower((char)[s characterAtIndex: i]))
            {
                NSString *xs=[[NSString stringWithFormat: @"%c", toupper([s characterAtIndex: i])] autorelease];
                NSRange r;
                r.location=i;
                r.length=1;
                [s replaceCharactersInRange:r withString:xs];
            }
        }
    
        c->buf->send();
        return 1;
    }
    
    return 0;
}

void IMMenuHandler(IMContext *c, unsigned int cmd)
{
    char msg[256];
    sprintf(msg, "IM: menu event, cmd=%d", cmd);
    MURMUR(msg);
}

