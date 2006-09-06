// CVSmartMenu.h: Smart menu services (for the "pencil" menu)
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef __CVSmartMenu_h
#define __CVSmartMenu_h

#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>
#include "CVkeyCode.h"

enum {
    CVSM_EXCLUSIVE=0,
    CVSM_MULTIPLE=1
};

enum {
    CVSMATTR_DISABLED=kMenuItemAttrDisabled,
    CVSMATTR_SEPARATOR=kMenuItemAttrSeparator
};

void CVDeleteMenu(id m);
int InsertMenuItem(id m, int cmdid, NSString *s, int attr=0, NSString *k=nil);
int CVInsertMenuItem(id m, int cmdid, NSBundle *b, NSString *s, int attr=0, NSString *k=nil);

@interface CVSmartMenuItem : NSObject {
    NSString *idtag;
    NSString *displaytag;
    int cmdid;
    int pos;
    BOOL chk;
}
- (id)initWithData:(int)cid menuPosition:(int)p idTag:(NSString*)it;
- (NSString*)description;
- (void)dealloc;
- (int)commandID;
- (NSString*)idTag;
- (BOOL)checked;
- (void)setChecked:(BOOL)c;
- (int)menuPosition;
@end

class CVSmartMenuGroup {
public:
    CVSmartMenuGroup(id m, int startcmd, NSBundle *b, int t=CVSM_EXCLUSIVE);
    ~CVSmartMenuGroup();
    void insertSeparator();
    void insertTitle(NSString *t);
    void insertItem(NSString *idtag, NSString *displaytag, NSString *shortcutkey=nil, BOOL bundlelookup=NO);
    void checkItem(NSString *idtag);
    void uncheckItem(NSString *idtag);
    void uncheckAll();
    void disableItem(NSString *idtag);
    void checkFirstItem();
    void checkItemArray(NSArray *idarray);
    int clickItem(int cmdid);
	CVSmartMenuItem* getMenuItem(int cmdid);
    NSArray *getCheckedItems();
    
protected:
    NSBundle *bundle;
    NSMutableArray *itemarray;
    id menu;
    int lastcmdid;
    int type;
};

#endif
