// VXTextBar.cpp

#include "VXTextBar.h"
#include "VXUtility.h"

const int vxtbDefFontSize=24;

void VXTBSetRect(Rect &r, int fontsize, int textlen)
{
    int w=textlen*fontsize;
    int h=fontsize;
    SetRect(r, 0, 0, w, h);
}


VXTextBar::VXTextBar(int fsize) : fontsize(fsize)
{
    lookupdated=textupdated=posupdated=displaying=0;
    text=CFStringCreateMutable(NULL, 0);
    CFStringAppendString(text, " ", kCFStringEncodingMacRoman);
    
    // create window rectanble
	Rect windowrect, labelrect;
	SetRect(&labelrect, 0, 0, 24, 10);
	windowrect=labelrect;
        
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

int VXTextBar::clear()
{
    CFRange rng=CFRangeMake(0,CFStringGetLength(text));
    CFStringDelete(text, rng);
    return textupdated=1;
}

int VXTextBar::append(void*s, OVEncoding e, int l)
{
    CFStringRef newstr=VXCreateCFString(s, e, l);
    CFStringAppend(text, newstr);
    CFRelease(newstr);
    return textupdated=1;
}

int VXTextBar::hide()
{
    if (!displaying) return 1;
    if (window) HideWindow(window);
    displaying=0;
    return 1;
}

int VXTextBar::show()
{
    update();
    if (displaying) return 1;    
    if (window) ShowWindow(window);
    displaying=1;
    return 1;
}


int VXTextBar::update()
{
    if (!(textupdated || lookupdated || postupdated)) return 1;
    
    Rect windowrect, labelrect;
    
    if (textupdated)
    {
        if (SetControlData(label, kControlEntireControl, 
            kControlStaticTextCFStringTag, sizeof(CFStringRef), &text) != noErr)
                return 0;
        
        VXTBSetRect(labelrect, fontsize, CFStringGetLength(text));
        windowrect=labelrect;        
        SetControlBounds(label, &labelrect);
        SizeWindow(window, windowrect.right, windowrect.bottom);

        textupdated=0;    
    }
    
    if (lookupdated)
    {
        VXTBSetRect(labelrect, fontsize, CFStringGetLength(text));
        windowrect=labelrect;        
        SetControlBounds(label, &labelrect);
        SizeWindow(window, windowrect.right, windowrect.bottom);

        lookupdated=0;
    }
    
    if (posupdated)
    {
        // fix position
        // move window
        posupdated=0;
    }
    
    ComponentResult r;
    r=SetControlData(label, kControlEntireControl, kControlStaticTextCFStringTag,
        sizeof(CFStringRef), &text);
    if (r!=noErr) return 0;
    
    DrawControls(window);
    return 1;
}
