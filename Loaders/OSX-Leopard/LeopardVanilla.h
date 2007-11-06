// LeopardVanilla.h
// Global Variables Definitions and Constants

#import <Cocoa/Cocoa.h>
#import <InputMethodKit/InputMethodKit.h>
#import "CVLoader.h"

extern IMKServer *_inputMethodServer;
extern MenuRef _sharedCarbonMenu;
extern CVLoader *_sharedLoader;
extern NSLock *_sharedLock;

#define LEOPARD_VANILLA_CONNECTION_NAME @"LeopardVanilla_1_Connection"
