// OVAbout.cpp

#include "OVAbout.h"
#include "CIMConst.h"

WindowRef about_window=NULL;

pascal OSStatus AboutWindowEventHandler(EventHandlerCallRef nextHandler, EventRef theEvent, void *userData)
{
   HideWindow(about_window);
   about_window = NULL;
   return noErr;
}

int ShowAbout()
{
    IBNibRef 		nibRef;
    WindowRef 		window;
	CFBundleRef bdl = CFBundleGetBundleWithIdentifier(CFSTR(cimBundleName));

    OSStatus		err;
	
	err = CreateNibReferenceWithCFBundle(bdl, CFSTR("OVLoader"), &nibRef);
    require_noerr( err, CantGetNibRef );
    
    // Then create a window. "MainWindow" is the name of the window object. This name is set in 
    // InterfaceBuilder when the nib is created.
    err = CreateWindowFromNib(nibRef, CFSTR("AboutWindow"), &window);
    require_noerr( err, CantCreateWindow );

    // We don't need the nib reference anymore.
    DisposeNibReference(nibRef);
    
    // The window was created hidden so show it.
    ShowWindow( window );
    
CantCreateWindow:
	fprintf (stderr, "can't create window!\n");
	return err;
CantGetNibRef:
	fprintf (stderr, "can't get nib!\n");
	return err;
}

/*
int ShowAbout()
{
	OSStatus err; 
	ControlRef theLabel;
	ControlFontStyleRec controlFontStyleStruc;
	CFStringRef bufstr;
	EventHandlerUPP aboutWindowEventHandler;
	EventTypeSpec eventTypes[] = {{kEventClassWindow, kEventWindowClose}};
	Rect bound = (*GetMainDevice())->gdRect, rect, frame;
    CFBundleRef myComponentBundle;
    myComponentBundle = CFBundleGetBundleWithIdentifier(CFSTR(cimBundleName));
	
	if (about_window) return 1;

	SetRect(&rect, (int)(bound.right/2) - 200, 100, (int)(bound.right/2) + 200, 340); 
	SetRect(&frame, 20, 20, 380, 310);
	err = CreateNewWindow(kDocumentWindowClass, kWindowCloseBoxAttribute + kWindowStandardHandlerAttribute, &rect, &about_window);
	aboutWindowEventHandler = NewEventHandlerUPP( AboutWindowEventHandler );
	InstallWindowEventHandler(about_window, aboutWindowEventHandler, 1, eventTypes, NULL, NULL);
   
	SetRect(&frame, 20, 20, 380, 310);
	bufstr=CFBundleCopyLocalizedString (myComponentBundle, CFSTR("AboutText"), NULL, NULL);

	controlFontStyleStruc.flags = kControlUseFontMask | kControlUseJustMask;
	controlFontStyleStruc.font   = kControlFontSmallSystemFont;
	controlFontStyleStruc.just  = teJustCenter;
	CreateStaticTextControl(about_window, &frame, bufstr, &controlFontStyleStruc, &theLabel);
	ShowWindow(about_window); 
	return err;
}

*/
