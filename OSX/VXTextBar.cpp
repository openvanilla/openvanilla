// VXTextBar.cpp

#define OVDEBUG 1

#include "VXTextBar.h"
#include "VXUtility.h"

void VXTBSetRect(Rect *r, int fontsize, int textlen);
void VXTBSetRect(Rect *r, int fontsize, CFStringRef inString);

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
    // windowrect=labelrect;
        
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

OVTextBar* VXTextBar::hide()
{
    if (displaying)
	{
		if (window) HideWindow(window);
		displaying=0;
	}
	return this;
}

OVTextBar* VXTextBar::show()
{
    update();
    if (displaying) return this;
    if (window) ShowWindow(window);
    displaying=1;
	return this;
}

int VXTextBar::setPosition(int x, int y)
{
    pos.h=x;
    pos.v=y;
    return posupdated=1;
}

OVTextBar* VXTextBar::update()
{
    if (!(textupdated || lookupdated || posupdated)) return this;
    
    Rect windowrect, labelrect;

    SetPortWindowPort(window);
   
    if (textupdated)
    {
        if (SetControlData(label, kControlEntireControl, 
            kControlStaticTextCFStringTag, sizeof(CFStringRef), &text) != noErr)
                return this;   // should throw exception
        
        VXTBSetRect(&labelrect, fontsize, text);
        windowrect=labelrect;        
        SetControlBounds(label, &labelrect);
        SizeWindow(window, windowrect.right, windowrect.bottom, TRUE);

        murmur("textupdate: windowsize: right = %d , bottom = %d",windowrect.right, windowrect.bottom);

        textupdated=0;    
    }
    
    if (lookupdated)
    {
        VXTBSetRect(&labelrect, fontsize, text);
        windowrect=labelrect;        
        SetControlBounds(label, &labelrect);
        SizeWindow(window, windowrect.right, windowrect.bottom, TRUE);

        murmur("lookupdated: windowsize: right = %d , bottom = %d",windowrect.right, windowrect.bottom);

        lookupdated=0;
    }
    
    
    if (posupdated)
    {
        posupdated=0;
    }

    VXTBSetRect(&labelrect, fontsize, text);
    VXTBFixPosition(&pos, labelrect.right, labelrect.bottom);
    MoveWindow(window, pos.h, pos.v, TRUE);        
    DrawControls(window);
    return this;
}

#define IntToFixed(a)	   ((Fixed)(a) << 16)

void VXTBSetRect(Rect *r, int fontsize, CFStringRef inString)
{
  ATSUStyle      Style;
  ATSUTextLayout Layout;
  Fixed          fontSize = IntToFixed(fontsize * 8 / 5);

#define ATTRS 1
  ATSUAttributeTag      Tags[ATTRS];
  ATSUAttributeValuePtr Values[ATTRS];
  ByteCount             Sizes[ATTRS];
#undef ATTRS

  murmur("VXTBSetRect: Initializing.");

  Tags[0]   = kATSUSizeTag;
  Sizes[0]  = (ByteCount) sizeof(Fixed);
  Values[0] = &fontSize;

  ATSUCreateStyle(&Style);
  ATSUSetAttributes(Style,1,Tags,Sizes,Values);

  UniChar*     Text;
  UniCharCount TextLength = CFStringGetLength(inString);

  Text = (UniChar*) malloc (TextLength * sizeof(UniChar));
  CFStringGetCharacters(inString,CFRangeMake(0,TextLength) ,Text);

  murmur("VXTBSetRect: Create text layout. TextLength = %d",(int)TextLength);

  ATSUStyle Styles[1];        Styles[0]     = Style;
  UniCharCount runLengths[1]; runLengths[0] = TextLength;

  ATSUCreateTextLayoutWithTextPtr(Text,
		  0,
		  TextLength,
		  TextLength,
		  1,
		  runLengths,
		  Styles,
		  &Layout);

  murmur("VXTBSetRect: measure the Size.");

  Rect r2;
  ATSUMeasureTextImage(Layout,0,TextLength,
	kATSUUseGrafPortPenLoc,kATSUUseGrafPortPenLoc,&r2);

  murmur("VXTBSetRect: Measured: top = %d, left = %d, bottom = %d, right = %d",r2.top,r2.left, r2.bottom, r2.right);

  SetRect(r, 0,0, r2.right - r2.left, r2.bottom - r2.top );
  murmur("VXTBSetRect: done, dispose all objects");

  free(Text);
  ATSUDisposeStyle(Style);
  ATSUDisposeTextLayout(Layout);
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


