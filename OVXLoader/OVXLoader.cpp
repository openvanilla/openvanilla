// OVXLoader.cpp

#define OVDEBUG 1

#include "CIM.h"
#include "OpenVanilla.h"
#include "OpenVanilla/OVUtility.h"
#include "OVLoadable.h"
#include "OVDebugIM.h"

const int ovxMaxIMIDLen=64;
const int ovxMaxPathLen=768;

struct OVXLoadableLibrary
{
    char path[ovMaxPathLen];        // if !strlen(path), this is a static linked IM
    int availableim;
    int unloadable;
    OVLNewType imnew;
    OVLDeleteType imdelete; 
};

const unsigned int ovxIMListMenuPos=0x4f56584c;     // stands for 'OVXL'

struct OVXLoadedIM
{
    OVInputMethod *im;
    char strid[ovxMaxIMIDLen];
    
    OVXLoadableLibrary *fromlib;
    int imid;
    int menupos;
};

class CIMSession        // CarbonInputMethod session data
{
public:
    OVIMSession *imsession;
    VXTextBar *textbar;
    
    int textbarstatus;
};
    
// list of CIMsessions
// active CIMsessions

// preference file monitor


int CIMCustomInitialize(MenuRef mnu)
{
    InsertMenuItemTextWithCFString(mnu, 
        CFSTR("Print total chars typed to stderr"), 0, 0, 'TOTL');
    fprintf (stderr, "custom IM initialized\n");
}

void CIMCustomTerminate()
{
    fprintf (stderr, "custom IM terminated\n");
}

void *CIMCustomOpen()
{
    return new CustomCounter;
}

int CIMCustomClose(void *data)
{
    delete (CustomCounter*)data;
    return 1;
}

int CIMCustomActivate(void *data, CIMInputBuffer *buf)
{
    CustomCounter *cntr=(CustomCounter*)data;
    cntr->activate();
    return 1;
}

int CIMCustomDeactivate(void *data, CIMInputBuffer *buf)
{
    CustomCounter *cntr=(CustomCounter*)data;
    cntr->deactivate();
    return 1;
}

int CIMCustomHandleInput(CIMInputBuffer *buf, unsigned char charcode,
		UInt32 keycode, UInt32 modifiers,  
		Point *pnt, void *data)
{
    CustomCounter *cntr=(CustomCounter*)data;
 
	fprintf (stderr, "point=%d,%d\n", pnt->h, pnt->v);
	      
    	// put printable character into the input buffer, update the display,
	// and returns 1 -- telling app that we have "eaten" that character
	if (isprint(charcode))
	{   
	    fprintf (stderr, "input position=(%d,%d)\n", pnt->h, pnt->v);
        cntr->add();
		buf->put(charcode);
		buf->update();
		return 1;
	}
	
	// this section handles function keys
    switch (charcode)
    {
        case 0x08:  // BACKSPACE
		case 0x7f:	// DELETE
			if (!buf->length()) return 0;
            buf->deletechar();
            buf->update();
            return 1;

        case 0x03:  // Mac's ENTER (but not LF)
        case 0x0D:  // Mac's RETURN (carrige return?)
            if (buf->length())	// if buffer not empty, "send" all chars
			{
				buf->send();
				return 1;
			}
			return 0;
            
		case 0x1b:	// ESC
			if (buf->length())
			{
				buf->clear();
				buf->update();
				return 1;
			}
			return 0;
    }
    
    return 0;	// send back all unprocessed keys
}


int CIMCustomMenuHandler(UInt32 command, MenuRef mnu, void *data)
{
    CustomCounter *cntr=(CustomCounter*)data;
    switch (command)
    {
        case 'TOTL':
            cntr->print();
            return 1;
    }
    
    return 0;
}
