// OVAbout.h: about window for OVLoader

#ifndef __OVAbout_h
#define __OVAbout_h

#include <Carbon/Carbon.h>

pascal OSStatus AboutWindowEventHandler(EventHandlerCallRef nextHandler, 
	EventRef theEvent, void *userData);
int ShowAbout();

#endif
