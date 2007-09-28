/* AZYWController */

#import <Cocoa/Cocoa.h>

@interface AZYWController : NSWindowController
{
    IBOutlet id image;
	float _windowAlpha;
	float _fadeAlpha;	
	NSImage * _image1;
	NSImage * _image2;
	NSTimer * _fadeTimer;	
	NSSound * _sound;
}

- (void) show;
@end
