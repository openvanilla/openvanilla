// CVLoader.h: The CocoaVanilla Loader (OVLoader for OS X)
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __CVLoader_h
#define __CVLoader_h

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include "TSComposingBuffer.h"
#include "CVConfig.h"
#include "CVService.h"
#include "CVSmartMenu.h"
#include "CVCandidate.h"
#include "CVBuffer.h"

class CVContext;

class CVLoader {
public:
    CVLoader();
    virtual ~CVLoader();
    int init(MenuRef m);
    CVContext *newContext();
    void setActiveContext(CVContext *c);
    void menuHandler(unsigned int cmd);
protected:
	id connectDisplayServer();
	void refreshMenu();
	void createMenuGroups();
	void checkMenuItems();
    void initializeModules(NSArray *src, NSMutableArray *dst, CVSmartMenuGroup *fallout);
    void syncMenuAndConfig(); // sync menu items&config, reassemble IM/OF arrays
    void pourModuleArrayIntoMenu(NSArray *ma, CVSmartMenuGroup *g, NSArray *orderedby=nil);
	void showOutputFilterStatus(NSString *modid, BOOL s);
	void switchToLastPrimaryIM();
    BOOL checkIfLastAtomicInitFailed(int timeout);  // unit in 1/100 second
	NSString *MSG(NSString *m);

    CVService *srv;
    CVCandidate *candi;
    CVContext *activecontext;
    CVConfig *cfg;
    NSMutableDictionary *loaderdict;        // dictionary for loader config
	NSMutableDictionary *menudict;		   // dictionary for menu keys
	NSBundle *loaderbundle;
	MenuRef immenu;
	id dspsrvr;						// display server (ObjC remote object)

    NSMutableArray *modarray;       // all loaded moudles
    NSMutableArray *imarray;        // current in-use KP & IM modules
    NSMutableArray *ofarray;        // current in-use OF modules
    CVSmartMenuGroup *kpmenugroup;  // key preprocessor menu group
    CVSmartMenuGroup *immenugroup;  // input method menu group
    CVSmartMenuGroup *ofmenugroup;  // output filter menu group
    friend class CVContext;
};

class CVContext {
public:
    CVContext(CVLoader *p);
    virtual ~CVContext();
    void activate(TSComposingBuffer *b);
    void deactivate();
    void fix();
    int event(char charcode, int modifiers);
    
    void showPrimaryIM();
    void repositionInfoBoxes();
    void clearAll();
    void syncConfig(int forced=0);
    
protected:
    CVLoader *loader;
    CVBuffer *buf;
    NSMutableArray *contexts;
    CVTimeTag stamp;
    CVCandidateState *candistate;
	
	// key receiver test
	id keyrcvr;
};

#endif
