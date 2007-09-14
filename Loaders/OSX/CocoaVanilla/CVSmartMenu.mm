// CVSmartMenu.mm: Smart menu services (for the "pencil" menu)
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

#include <ctype.h>
#include "CVSmartMenu.h"

void CVDeleteMenu(MenuRef m) {
	int p=CountMenuItems(m);
	DeleteMenuItems(m, 1, p);
}

int CVInsertMenuItem(MenuRef m, int cmdid, NSString *s, int attr, NSString *k) {
    int p=CountMenuItems(m);
    if (InsertMenuItemTextWithCFString(m, (CFStringRef)s, p, attr, cmdid)!=noErr) return 0;
	
	if (k) {
        if ([k length]) {
            CVKeyCode kc(k);
            
            // I CANNOT BELIEVE IT, BUT WE HAVE TO TRANSLATE THESE KEYCODES
            // INTO APPLE'S VIRTUAL KEYCODES, OTHERWISE THIS PART WON'T
            // WORK IN OS X 10.3 !
            UInt16 vcode=0;
            #define VKC(x,y)    case x: vcode=y; break
            switch (kc.code()) {
                VKC(ovkSpace, 0x31);
                VKC(ovkEsc, 0x35);
                VKC(ovkReturn, 0x24);
                VKC(ovkDelete, 0x75);
                VKC(ovkBackspace, 0x33);
                VKC(ovkUp, 0x7e);
                VKC(ovkDown, 0x7d);
                VKC(ovkLeft, 0x7b);
                VKC(ovkRight, 0x7c);
                VKC(ovkHome, 0x73);
                VKC(ovkEnd, 0x77);
                VKC(ovkPageUp, 0x74);
                VKC(ovkPageDown, 0x79);
                VKC(ovkTab, 0x30);
            }
            
            // determine if is OS X 10.4 or above
            if ([NSString instancesRespondToSelector:@selector(writeToFile:atomically:encoding:error:)]) {
                SetMenuItemCommandKey(m, p+1, FALSE, toupper(kc.code()));
            }
            else {
                if (vcode)
                    SetMenuItemCommandKey(m, p+1, TRUE, vcode);
                else
                    SetMenuItemCommandKey(m, p+1, FALSE, toupper(kc.code()));
            }

            SetMenuItemModifiers(m, p+1, kc.convertToMenuModifier());
        }
	}
	
    return p+1;
}

int CVInsertMenuItem(MenuRef m, int cmdid, NSBundle *b, NSString *s, int attr, NSString *k) {
    NSString *ls=b ? [b localizedStringForKey:s value:nil table:nil] : s;
    return CVInsertMenuItem(m, cmdid, ls, attr, k);
}

@implementation CVSmartMenuItem
- (id)initWithData:(int)cid menuPosition:(int)p idTag:(NSString*)it {
    if (self=[super init]) {
        cmdid=cid;
        pos=p;
        chk=NO;
        idtag=[[NSString alloc] initWithString: it];
    }
    return self;
}
- (void)dealloc {
    [idtag release];
    [super dealloc];
}
- (NSString*)description {
    return [NSString stringWithFormat:@"id=%@, checked=%d, commandID=%d, pos=%d", idtag, chk, cmdid, pos];
}
- (int)commandID {
    return cmdid;
}
- (NSString*)idTag {
    return idtag;
}
- (BOOL)checked {
    return chk;
}
- (void)setChecked:(BOOL)c {
    chk=c;
}
- (int)menuPosition {
    return pos;
}
@end


CVSmartMenuGroup::CVSmartMenuGroup(MenuRef m, int startcmd, NSBundle *b, int t) {
    menu=m;
    bundle=b;
    lastcmdid=startcmd;
    type=t;
    itemarray=[NSMutableArray new];
}

CVSmartMenuGroup::~CVSmartMenuGroup() {
    [itemarray release];
}

void CVSmartMenuGroup::insertSeparator() {
    CVInsertMenuItem(menu, 0, @"-", CVSMATTR_SEPARATOR);
}

void CVSmartMenuGroup::insertTitle(NSString *t) {
    CVInsertMenuItem(menu, 0, bundle, t, CVSMATTR_DISABLED);
}

void CVSmartMenuGroup::insertItem(NSString *idtag, NSString *displaytag, NSString *shortcutkey, BOOL bundlelookup) {
    int p=CountMenuItems(menu);
    NSString *rdt=bundlelookup ? [bundle localizedStringForKey:displaytag value:nil table:nil] : displaytag;

    CVSmartMenuItem *i=[CVSmartMenuItem alloc];
    [i initWithData:lastcmdid menuPosition:p+1 idTag: idtag];
    [i autorelease];
    [itemarray addObject: i];

    CVInsertMenuItem(menu, lastcmdid, rdt, 0, shortcutkey);
    lastcmdid++;
}

void CVSmartMenuGroup::checkItem(NSString *idtag) {
    int c=[itemarray count];
    for (int j=0; j<c; j++) {
        CVSmartMenuItem *i=[itemarray objectAtIndex:j];
        // found checked item
        if ([[i idTag] isEqualToString:idtag]) {
            if (type==CVSM_EXCLUSIVE) {
                if (![i checked]) {
                    [i setChecked: YES];
                    CheckMenuItem(menu, [i menuPosition], TRUE);
                }
            }
            else {      // toggle menu item
                if (![i checked]) {
                    [i setChecked:YES];
                    CheckMenuItem(menu, [i menuPosition], TRUE);
                }
                else {
                    [i setChecked:NO];
                    CheckMenuItem(menu, [i menuPosition], FALSE);
                }
            }
        }
        else {      // not the found item, then see if it's an exclusive group
            if (type==CVSM_EXCLUSIVE) {
                if ([i checked]) {
                    [i setChecked:NO];
                    CheckMenuItem(menu, [i menuPosition], FALSE);
                }
            }
        }
    }
}

void CVSmartMenuGroup::uncheckItem(NSString *idtag) {
    int c=[itemarray count];
    for (int j=0; j<c; j++) {
        CVSmartMenuItem *i=[itemarray objectAtIndex:j];
        if ([[i idTag] isEqualToString:idtag]) {
            CheckMenuItem(menu, [i menuPosition], FALSE);
            [i setChecked:NO];
            return;
        }
    }
}

void CVSmartMenuGroup::uncheckAll() {
    int c=[itemarray count];
    for (int j=0; j<c; j++) {
        CVSmartMenuItem *i=[itemarray objectAtIndex:j];
        CheckMenuItem(menu, [i menuPosition], FALSE);
        [i setChecked:NO];
    }
}

void CVSmartMenuGroup::disableItem(NSString *idtag) {
    int c=[itemarray count];
    for (int j=0; j<c; j++) {
        CVSmartMenuItem *i=[itemarray objectAtIndex:j];
        if ([[i idTag] isEqualToString:idtag]) {
            DisableMenuItem(menu, [i menuPosition]);
            return;
        }
    }
}

void CVSmartMenuGroup::checkFirstItem() {
    if (![itemarray count]) return;
    CVSmartMenuItem *i=[itemarray objectAtIndex:0];
    checkItem([i idTag]);
}

void CVSmartMenuGroup::checkItemArray(NSArray *idarray) {
    int c=[idarray count];
    for (int j=0; j<c; j++) checkItem([idarray objectAtIndex:j]);
}

int CVSmartMenuGroup::clickItem(int cmdid) {
    int c=[itemarray count];
    CVSmartMenuItem *i;
    for (int j=0; j<c; j++) {
        i=[itemarray objectAtIndex:j];
        if (cmdid == [i commandID]) {
            checkItem([i idTag]);
            return 1;
        }
    }    
    return 0;
}

CVSmartMenuItem *CVSmartMenuGroup::getMenuItem(int cmdid) {
    int c=[itemarray count];
    for (int j=0; j<c; j++) {
        CVSmartMenuItem *i=[itemarray objectAtIndex:j];
        if (cmdid == [i commandID]) return i;
    }
	return nil;
}

NSArray *CVSmartMenuGroup::getCheckedItems() {
    NSMutableArray *m=[[NSMutableArray new] autorelease];
    int c=[itemarray count];
    for (int j=0; j<c; j++) {
        CVSmartMenuItem *i=[itemarray objectAtIndex:j];
        if ([i checked]) [m addObject:[i idTag]];
    }
    return m;
}
