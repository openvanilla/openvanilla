#import "ServerDelegate.h"
#import "NSStringExtension.h"
#import <WebKit/WebKit.h>

@implementation ServerDelegate
- (void)setConfig:(NSDictionary*)cfg {
	NSLog(@"received config!");
	NSLog([cfg description]);
}
- (void)awakeFromNib {
	NSLog(@"setting up OVDisplayServer");

	NSConnection *c=[NSConnection defaultConnection];
	[c setRootObject:self];
    if ([c registerName:@"OVDisplayServer"]) NSLog(@"OVDisplayServer registered");
	else NSLog(@"OVDisplayServer registration failed");

	NSLog(@"awake from nib!");
	[text setStringValue:@"hello, world!"];
	[text2 setStringValue:@"world, hello!"];
	[window setLevel:NSScreenSaverWindowLevel];
	[window2 setLevel:NSScreenSaverWindowLevel];
	[window orderOut:self];
	[window2 orderOut:self];
	
	// [window setAlphaValue:];
	[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(evil) userInfo:nil repeats:YES];
	
	o=fopen("/tmp/ovib-hack", "r");
	if (!o) {
		system("echo > /tmp/ovib-hack");
	}

	o=fopen("/tmp/ovib-hack", "r");
	if (o) fseek(o, 0, SEEK_END);
}
- (IBAction)adjustWindowLevel:(id)sender
{
	NSLog(@"adjust!");
	[window setLevel:NSScreenSaverWindowLevel];
	[window orderFront:nil];
	[window display];
}
- (void)evil {
	// NSLog(@"evil!");

	char buf[25600];
	int p=0;
	if (!o) return;
	clearerr(o);
	while (1) {
		int c=fgetc(o);
		if (c==EOF) break;
		if (c==0xa) c=32;
		buf[p++]=c;
		if (p==25598) break;
	}
	buf[p]=0;
	if (!p) return;
	NSArray *a=[[NSString stringWithUTF8String:buf] splitBySpaceWithQuote];
	int i, ac=[a count];
	for (i=0;i<ac;i++) {
		const char *s=[[a objectAtIndex:i] UTF8String];
		id w, t;
		if (!strcmp(s, "candi")) {
			w=window;
			t=text;
		}
		else {
			w=window2;
			t=text2;
		}
		i++;
		
		s=[[a objectAtIndex:i] UTF8String];
		
		if (!strcmp(s, "hide")) {
			[w orderOut:self];
		}
		else if (!strcmp(s, "show")) {
			[w orderFront:self];
		}
		else if (!strcmp(s, "update")) {
			i++;
			int x=[[a objectAtIndex:i++] intValue];
			int y=[[a objectAtIndex:i++] intValue];
			NSPoint np={(float)x, (float)y};
			[w setFrameTopLeftPoint:np];
				
			NSString *msg=[a objectAtIndex:i];
			[t setStringValue:msg];
            
            if (w==window) {
                WebFrame *wf=[web mainFrame];
                NSString *html=[NSString stringWithFormat:
                    @"<html>%@<p><font size=\"+1\">%@</font><p><font size=\"+2\">%@</font><p></html>", msg, msg, msg
                    ];
                [wf loadHTMLString: html
                    baseURL: [NSURL URLWithString: @"http://localhost"]];
            }
		} 
	}
}
@end
