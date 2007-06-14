//
//  InputMethodSessionController.m of «PROJECTNAME»
//
//	Created:	«DATE»
//
//	Copyright:	Copyright  «YEAR» «ORGANIZATIONNAME», All Rights Reserved
//

#import "LVIMSessionController.h"

#import "CVLoader.h"

CVLoader *cvloader;
CVContext *context;
id current_sender;
TSComposingBuffer *buf;

@implementation LVIMSessionController
- (int)tscbGetX
{
//	NSRect r = [current_sender firstRectForCharacterRange:NSMakeRange(0,0)];
	NSRect r;
	[current_sender attributesForCharacterIndex:0 lineHeightRectangle:&r];
	NSLog(@"(%f,%f)[%f,%f]", r.origin.x, r.origin.y, r.size.width, r.size.height);
	return (int)r.origin.x;
}
- (int)tscbGetY
{
//	NSRect r=[current_sender firstRectForCharacterRange:NSMakeRange(0,0)];
	NSRect r;
	[current_sender attributesForCharacterIndex:0 lineHeightRectangle:&r];

	r.origin.y=1200-r.origin.y;
	NSLog(@"(%f,%f)[%f,%f]", r.origin.x, r.origin.y, r.size.width, r.size.height);
	return (int)r.origin.y;
}
- (void)tscbClear
{
	[composed setString:@""];
}

- (void)tscbSend
{
	[self commitComposition:current_sender];
	[composed setString:[NSString string]];	
}

- (void)tscbUpdate:(BOOL)send withCursor:(int)cursor from:(int)markFrom to:(int)markTo
{
	NSDictionary*		attributes;
	NSAttributedString* attributedString;

	attributes = [self markForStyle:kTSMHiliteConvertedText atRange:NSMakeRange(0, [composed length])];
	attributedString = [[NSAttributedString alloc] initWithString:composed attributes:attributes];

	[current_sender setMarkedText:attributedString
				selectionRange:NSMakeRange([composed length], 0) replacementRange:NSMakeRange(NSNotFound, NSNotFound)];	

	if (send) [self tscbSend];
}

- (void)tscbAppend:(NSString*)s
{
	[composed appendString:s];	
}

- (BOOL)tscbIsEmpty
{
	if (![composed length]) return YES;
	return NO;
}

- (NSString*)tscbGetContent
{
	return composed;
}

- (id)initWithServer:(IMKServer*)server delegate:(id)delegate 
        client:(id)inputClient
{
	buf = new TSComposingBuffer(self);
	
	NSLog(@"an instance of NumbersInputController created!");
	composed=[NSMutableString new];
	
	NSLog(@"creating CVLoader");
	cvloader = new CVLoader;
	
	NSLog(@"initing CVLoader");
	cvloader->init(nil);
	
	NSLog(@"creating context");
	context=cvloader->newContext();
	
	cvloader->setActiveContext(context);
	// context->syncConfig(1);
	context->activate(buf);
	
	return [super initWithServer:server delegate:delegate client:inputClient];
}

-(void)dealloc
{
	[composed release];
	[super dealloc];
}

-(id)composedString:(id)sender
{
	return composed;
}

- (void)activateServer:(id)sender
{
	current_sender = sender;
	NSLog(@"LVIMSC is activated!");
	// [super activateServer];
	context->activate(buf);

}
-(void)commitComposition:(id)sender 
{
	NSLog(@"commitComposition");
	NSString *c = composed;
	if (!c || ![c length]) c = @"";
	
/*	NSString*		text = [self composedBuffer];

	if ( text == nil || [text length] == 0 ) {
		text = [self originalBuffer];
	}
*/
	[sender insertText:composed replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
//	[sender insertText:text replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
	
//	[self setComposedBuffer:@""];
//	[self setOriginalBuffer:@""];
//	_insertionIndex = 0;
//	_didConvert = NO;
}

-(BOOL)inputText:(NSString*)string key:(NSInteger)keyCode modifiers:(NSUInteger)flags client:(id)sender
{
	current_sender=sender;
	
	unichar c=[string characterAtIndex:0];
	char cc=c;
	NSLog(@"string %@ char %c (value %d), keycode %d modifiers %d, casted into char %d", string, c, c, keyCode, flags, cc);
	
	
	
	int x=context->event(c, flags);
	NSLog(@"returns %d", x);
	return (BOOL)x;
}

/*
-(BOOL)didCommandBySelector:(SEL)aSelector client:(id)sender
{
	unsigned int x=(unsigned int)[self performSelector:aSelector withObject:sender];
    // return reinterpret_cast<BOOL>(x);
	return (BOOL)x;
}

*/

- (BOOL)insertNewline:(id)sender
{
	if ([composed length] == 0) return NO;
	[self commitComposition:sender];
	[composed setString:[NSString string]];

	return YES;
}

/*
-(BOOL)inputText:(NSString*)string key:(NSInteger)keyCode modifiers:(NSUInteger)flags client:(id)sender
{
	return YES;
}
*/

/*
Implement one of the three ways to receive input from the client. 
Here are the three approaches:
                 
                 1.  Support keybinding.  
                        In this approach the system takes each keydown and trys to map the keydown to an action method that the input method has implemented.  If an action is found the system calls didCommandBySelector:client:.  If no action method is found inputText:client: is called.  An input method choosing this approach should implement
                        -(BOOL)inputText:(NSString*)string client:(id)sender;
                        -(BOOL)didCommandBySelector:(SEL)aSelector client:(id)sender;
                        
                2. Receive all key events without the keybinding, but do "unpack" the relevant text data.
                        Key events are broken down into the Unicodes, the key code that generated them, and modifier flags.  This data is then sent to the input method's inputText:key:modifiers:client: method.  For this approach implement:
                        -(BOOL)inputText:(NSString*)string key:(NSInteger)keyCode modifiers:(NSUInteger)flags client:(id)sender;
                        
                3. Receive events directly from the Text Services Manager as NSEvent objects.  For this approach implement:
                        -(BOOL)handleEvent:(NSEvent*)event client:(id)sender;
*/
@end
