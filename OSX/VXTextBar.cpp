// VXTextBar.cpp

#include "VXTextBar.h"
#include "VXUtility.h"

void VXTBSetRect(Rect *r, int fontsize, int textlen);
void VXTBFixPosition(Point *p, int width, int height);

VXTextBar::VXTextBar(int fsize) : fontsize(fsize)
{
    lookupdated=textupdated=posupdated=displaying=0;
    pos.h=pos.v=0;
    text=CFStringCreateMutable(NULL, 0);
    CFStringAppendCString(text, " ", kCFStringEncodingMacRoman);
    
    // create window rectanble
	Rect windowrect, labelrect;
	SetRect(&labelrect, 0, 0, 24, 10);
	SetRect(&windowrect, 100, 100, 200, 40);
//	windowrect=labelrect;
        
    if (CreateNewWindow(kUtilityWindowClass, 
        kWindowStandardHandlerAttribute | kWindowMetalAttribute |   
        kWindowSideTitlebarAttribute, &windowrect, &window) != noErr)
    {
        return;     // should throw exception
    }

    ControlFontStyleRec fontstyle;
    fontstyle.flags=kControlUseSizeMask;
    fontstyle.size=vxtbDefFontSize;
    
    CreateStaticTextControl(window, &labelrect, text, &fontstyle, &label);
    MoveWindow(window, windowrect.left, windowrect.top, true);

}

VXTextBar::~VXTextBar()
{
    if (!window) return;
    HideWindow(window);
    DisposeWindow(window);
    CFRelease(text);
}

OVTextBar* VXTextBar::clear()
{
    CFRange rng=CFRangeMake(0,CFStringGetLength(text));
    CFStringDelete(text, rng);
    textupdated=1;
    return this;
}

OVTextBar* VXTextBar::append(void*s, OVEncoding e, int l)
{
    CFStringRef newstr=VXCreateCFString(s, e, l);
    CFStringAppend(text, newstr);
    CFRelease(newstr);
    textupdated=1;
    return this;
}

int VXTextBar::hide()
{
    if (!displaying) return 1;
    if (window) HideWindow(window);
    return displaying=0;
}

int VXTextBar::show()
{
    update();
    if (displaying) return 1;    
    if (window) ShowWindow(window);
    return displaying=1;
}

int VXTextBar::setPosition(int x, int y)
{
    pos.h=x;
    pos.v=y;
    return posupdated=1;
}

int VXTextBar::update()
{
    if (!(textupdated || lookupdated || posupdated)) return 1;
    
    Rect windowrect, labelrect;
    
    if (textupdated)
    {
        if (SetControlData(label, kControlEntireControl, 
            kControlStaticTextCFStringTag, sizeof(CFStringRef), &text) != noErr)
                return 0;   // should throw exception
        
        VXTBSetRect(&labelrect, fontsize, CFStringGetLength(text));
        windowrect=labelrect;        
        printf ("%d,%d,%d,%d\n", labelrect.left, labelrect.top, labelrect.right, labelrect.bottom);
        SetControlBounds(label, &labelrect);
        SizeWindow(window, windowrect.right, windowrect.bottom, TRUE);

        textupdated=0;    
    }
    
    if (lookupdated)
    {
        VXTBSetRect(&labelrect, fontsize, CFStringGetLength(text));
        windowrect=labelrect;        
        SetControlBounds(label, &labelrect);
        SizeWindow(window, windowrect.right, windowrect.bottom, TRUE);

        lookupdated=0;
    }
    
    
    if (posupdated)
    {
        posupdated=0;
    }

    VXTBSetRect(&labelrect, fontsize, CFStringGetLength(text));
    VXTBFixPosition(&pos, labelrect.right, labelrect.bottom);
    MoveWindow(window, pos.h, pos.v, TRUE);        
    DrawControls(window);
    return 1;
}

void VXTBSetRect(Rect *r, int fontsize, int textlen)
{
    int w=textlen*fontsize;
    int h=fontsize;
    SetRect(r, 0, 0, w, h);
}

void VXTBFixPosition(Point *p, int width, int height)
{
	int heightfixer=height+20;
	int widthfixer=20;
	
	Rect bound, avail;
	GDHandle nextgd=GetDeviceList();
	
	do
	{
		if(!TestDeviceAttribute(nextgd, screenDevice)) continue;
		if(!TestDeviceAttribute(nextgd, screenActive)) continue;
		bound=(*nextgd)->gdRect;
		
		GetAvailableWindowPositioningBounds(nextgd, &avail);
		
		if (PtInRect(*p, &bound)) break;
	} while((nextgd = GetNextDevice(nextgd)) != nil);
	
	if (!PtInRect(*p, &bound))
	{
		bound=(*GetMainDevice())->gdRect;
		GetAvailableWindowPositioningBounds(nextgd, &avail);
	}
	
	if (p->v > avail.bottom - heightfixer) p->v = avail.bottom-heightfixer;
	if (p->h > avail.right - width) p->h = avail.right-width-widthfixer;
	if (p->v < avail.top+GetMBarHeight()) p->v=avail.top+GetMBarHeight();
	if (p->h < avail.left) p->h=avail.left+widthfixer;
}


