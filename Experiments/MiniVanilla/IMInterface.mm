#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "IMInterface.h"

bool inited=false;

int IMInitialize(MenuRef mnu)
{
    if (inited) return 1;
	inited=true;
	return 1;
}

int IMContextOpen(IMContext *c)
{
	c->data=NULL;
	return 1;
}

void IMContextClose(IMContext *c)
{
}

void IMContextActivate(IMContext *c)
{
}

void IMContextDeactivate(IMContext *c)
{
}

void IMContextFix(IMContext *c)
{
}

int IMContextEvent(IMContext *c, char charcode, unsigned int modifiers)
{
	fprintf (stderr, "MiniVanilla event, charcode=%04x, modifiers=%08x\n", charcode, modifiers);
    return 0;
}

void IMMenuHandler(IMContext *c, unsigned int cmd)
{
}

