// CVSmartMenu.h

#ifndef __CVSmartMenu_h
#define __CVSmartMenu_h

#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>

enum {
    CVSM_EXCLUSIVE=0,
    CVSM_MULTIPLE=1
};

enum {
    CVSMATTR_DISABLED=kMenuItemAttrDisabled,
    CVSMATTR_SEPARATOR=kMenuItemAttrSeparator
};

void CVDeleteMenu(MenuRef m);
int CVInsertMenuItem(MenuRef m, int cmdid, NSString *s, int attr=0);
int CVInsertMenuItem(MenuRef m, int cmdid, NSBundle *b, NSString *s, int attr=0);

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
    CVSmartMenuGroup(MenuRef m, int startcmd, NSBundle *b, int t=CVSM_EXCLUSIVE);
    ~CVSmartMenuGroup();
    
    void insertSeparator();
    void insertTitle(NSString *t);  // looks up in bundle's localization table
    void insertItem(NSString *idtag, NSString *displaytag, BOOL bundlelookup=NO);
    void checkItem(NSString *idtag);
    void uncheckItem(NSString *idtag);
    void uncheckAll();
    void disableItem(NSString *idtag);
    void checkFirstItem();          // works if there's an item in the array
    void checkItemArray(NSArray *idarray);
    int clickItem(int cmdid);
    NSArray *getCheckedItems();     // an array of id tags
    
protected:
    NSBundle *bundle;
    NSMutableArray *itemarray;
    MenuRef menu;
    int lastcmdid;
    int type;
};

#endif
