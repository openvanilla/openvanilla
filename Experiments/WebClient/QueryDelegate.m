#import <WebKit/WebKit.h>
#import "QueryDelegate.h"

@implementation QueryDelegate
- (void)awakeFromNib {
	NSLog(@"Awaken from .nib!");
	
	// register two global notifications
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notification:) name:@"WebProgressStartedNotification" object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(notification:) name:@"WebProgressFinishedNotification" object:nil];

	[textField setDelegate:self];
	[self query:nil];
}
- (void)controlTextDidChange:(NSNotification*)n {
	[self query:[n object]];
}
- (void)notification:(NSNotification*)n {
	NSLog(@"Notification received, name=%@", [n name]);
	if ([[n name] isEqualToString: @"WebProgressStartedNotification"]) {
		[progressIndicator startAnimation:self];
		[queryButton setTitle:@"Stop"];
	}
	else if ([[n name] isEqualToString: @"WebProgressFinishedNotification"]) {
		[progressIndicator stopAnimation:self];
		[queryButton setTitle:@"Start"];
	}
	
	WebScriptObject *script=[webView windowScriptObject];
	[script setValue:self forKey:@"richClient"];
}
- (IBAction)query:(id)sender
{
	if ([[sender className] isEqualToString:@"NSButton"]) {
		if ([[sender title] isEqualToString:@"Stop"]) {
			NSLog(@"user interruption");
			[webView stopLoading:self];
			return;
		}
	}

	// we need to escape the URL string first
	NSString *base=@"http://nex.gugod.org:3000/im/liu5/";
	// NSString *base=@"http://openvanilla.org/lookupservice/lookup-pinyin.php?query=";
	NSString *qstr=[NSString stringWithFormat:@"%@%@", base, [textField stringValue]];
	NSString *escaped=[qstr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	NSLog(@"requesting URL: %@", escaped);
	
	NSURLRequest *urlreq=[NSURLRequest requestWithURL:[NSURL URLWithString:escaped]];
	[[webView mainFrame] loadRequest: urlreq];
}
+ (BOOL)isSelectorExcludedFromWebScript:(SEL)aSelector {
	return NO;
}
- (void)aboutWindow {
	[[NSApplication sharedApplication] orderFrontStandardAboutPanelWithOptions:nil];
}
@end
