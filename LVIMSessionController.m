//
//  InputMethodSessionController.m of «PROJECTNAME»
//
//	Created:	«DATE»
//
//	Copyright:	Copyright  «YEAR» «ORGANIZATIONNAME», All Rights Reserved
//

#import "LVIMSessionController.h"


@implementation LVIMSessionController
- (id)initWithServer:(IMKServer*)server delegate:(id)delegate 
        client:(id)inputClient
{
	NSLog(@"an instance of NumbersInputController created!");
	
	composed=[NSMutableString new];
	
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

-(BOOL)inputText:(NSString*)string key:(NSInteger)keyCode modifiers:(NSUInteger)flags client:(id)sender
{
	NSDictionary*		attributes;
	NSAttributedString* attributedString;

	NSLog(@"received string %@, keycode=%d, modifiers=%d", string, keyCode, flags);

	[composed appendString:string];
	NSLog(@"received %@, now composed=%@", string, composed);
	// [super markForStyle:kTSMHiliteConvertedText atRange:NSMakeRange(0,[composed length])];
	// [super updateComposition];
	
	NSLog(@"changing marked text, length=%d", [composed length]);
		
	attributes = [self markForStyle:kTSMHiliteConvertedText atRange:NSMakeRange(0, [composed length])];
	attributedString = [[NSAttributedString alloc] initWithString:composed attributes:attributes];

	[sender setMarkedText:attributedString
				selectionRange:NSMakeRange([composed length], 0) replacementRange:NSMakeRange(NSNotFound, NSNotFound)];	

	return YES;
}

-(BOOL)didCommandBySelector:(SEL)aSelector client:(id)sender
{
    return (BOOL)[self performSelector:aSelector withObject:sender];
}

- (BOOL)insertNewline:(id)sender
{
	if ([composed length] == 0) return NO;
	[super commitComposition];
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
