//  LVInputMethodController.h

#import <Cocoa/Cocoa.h>
#import <InputMethodKit/InputMethodKit.h>
#import "LeopardVanilla.h"
#import "LVComposingBuffer.h"

@interface LVInputMethodController : IMKInputController {
	NSMutableString *_bufferContent;
	TSComposingBuffer *_composingBuffer;
	CVContext *_context;
	
	id _currentClient;
	NSPoint _cursorPosition;
}
- (void)composingBufferClear;
- (void)composingBufferSend;
- (void)composingBufferUpdate:(BOOL)shouldSend cursorPosition:(int)position highlightFrom:(int)from to:(int)to;
- (void)composingBufferAppend:(NSString*)string;
- (BOOL)isComposingBufferEmpty;
- (Point)getCursorPosition;
- (NSMutableString*)composingBufferContent;
@end
