// OVInterface.h

#ifndef __OVInterface_h
#define __OVInterface_h

@protocol OVKey
- (int)code;
- (BOOL)isShift;
- (BOOL)isCapslock;
- (BOOL)isCtrl;
- (BOOL)isOpt;
- (BOOL)isCommand;
- (BOOL)isPrintable;
- (BOOL)isAlpha;
- (BOOL)isNumeric;
- (int)upper;
- (int)lower;
}
@end

@protocol OVStringBufffer
- clear;
- update;
- append:(const char*)str;
@end

@protocol OVPreeditArea <OVStringBuffer>
- send;
- updateWithCursor:(int)cursorPos highLightFrom:(int)from highLightTo:(int)to;
- appendChar:(char)c;
- (BOOL)isEmpty;
@end

@protocol OVInfoBox <OVStringBuffer>
- setSelectKey:(const char*)key;
- setValidAndCloseKey:(conar char*)ley;
- show;
- hide;
- finalize;
- next;
- (BOOL)isOnScreen; 
@end

@protocol OVService
- beep;
@end;

@protocol OVIMContext
- activate;
- deactivate;
- clear;
- (int)keyEvent:(id)key preedit:(id)area infoBox:(id)box service:(id)srv;
- (int)candidateEvent:(id)key preedit:(id)area infoBox:(id)box service:(id)srv;
@end

@protocol OVComponent
- (NSString*)identifier;
- (NSString*)localizedName: (const char*)locale;
- (BOOL)initialize: (NSArray*)modulePref modulePath:(const char*)mpath
  userPath:(const char*)upath seperator:(const char*)spr;
- terminate: (NSArray*)modulePref;
- update: (NSArray*)modulePref;
@end;

@protocol OVInputMethod <OVComponent>
- createNewContext;
@end;

@protocol OVBufferFilter <OVComponent>
- (NSString*) 
@end;

#endif