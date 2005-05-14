// CVSmartMenu.mm

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
		CVKeyCode kc(k);
		SetMenuItemCommandKey(m, p+1, FALSE, toupper(kc.code()));
		SetMenuItemModifiers(m, p+1, kc.convertToMenuModifier());
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
    NSEnumerator *e=[itemarray objectEnumerator];
    CVSmartMenuItem *i;
    while (i=[e nextObject]) {
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
    NSEnumerator *e=[itemarray objectEnumerator];
    CVSmartMenuItem *i;
    while (i=[e nextObject]) {
        if ([[i idTag] isEqualToString:idtag]) {
            CheckMenuItem(menu, [i menuPosition], FALSE);
            [i setChecked:NO];
            return;
        }
    }
}

void CVSmartMenuGroup::uncheckAll() {
    NSEnumerator *e=[itemarray objectEnumerator];
    CVSmartMenuItem *i;
    while (i=[e nextObject]) {
        CheckMenuItem(menu, [i menuPosition], FALSE);
        [i setChecked:NO];
    }
}

void CVSmartMenuGroup::disableItem(NSString *idtag) {
    NSEnumerator *e=[itemarray objectEnumerator];
    CVSmartMenuItem *i;
    while (i=[e nextObject]) {
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
    NSEnumerator *e=[idarray objectEnumerator];
    NSString *s;
    while (s=[e nextObject]) checkItem(s);
}

int CVSmartMenuGroup::clickItem(int cmdid) {
    NSEnumerator *e=[itemarray objectEnumerator];
    CVSmartMenuItem *i;
    while (i=[e nextObject]) {
        if (cmdid == [i commandID]) {
            checkItem([i idTag]);
            return 1;
        }
    }    
    return 0;
}

CVSmartMenuItem *CVSmartMenuGroup::getMenuItem(int cmdid) {
    NSEnumerator *e=[itemarray objectEnumerator];
    CVSmartMenuItem *i;
    while (i=[e nextObject]) if (cmdid == [i commandID]) return i;
	return nil;
}

NSArray *CVSmartMenuGroup::getCheckedItems() {
    NSMutableArray *m=[[NSMutableArray new] autorelease];
    NSEnumerator *e=[itemarray objectEnumerator];
    CVSmartMenuItem *i;
    while (i=[e nextObject]) {
        if ([i checked]) [m addObject:[i idTag]];
    }
    return m;
}
