#import "AZYWController.h"

#define FADEWAIT       0
#define FADEINTERVAL   0.1
#define FADEVALUE      0.025 

@implementation AZYWController

- (void) awakeFromNib {
	NSLog(@"Loading");
	NSBundle *b = [NSBundle bundleWithIdentifier:@"org.openvanilla.antizyw"];
	_image1=[[NSImage alloc] initByReferencingFile:[b pathForResource:@"ZhuyinAway" ofType:@"png"]];
	_image2=[[NSImage alloc] initByReferencingFile:[b pathForResource:@"ZhuyinKill" ofType:@"png"]];
	_sound = [[NSSound alloc] initWithContentsOfFile:[b pathForResource:@"away" ofType:@"mp3"] byReference:YES];
	[image setImage:_image1];
	_fadeTimer = nil;
	_windowAlpha = 1.0;	
}

- (void)stopTimer {
    if (_fadeTimer) [_fadeTimer invalidate];
    _fadeTimer=nil;
	[[self window] setAlphaValue:_windowAlpha];
}
- (void)fadeStart {
    [_fadeTimer invalidate];
    _fadeTimer=[NSTimer scheduledTimerWithTimeInterval:FADEINTERVAL target:self selector:@selector(fadeWork) userInfo:nil repeats:YES];
    _fadeAlpha=_windowAlpha;
}
- (void)fadeWork {
    _fadeAlpha-=FADEVALUE;
    if (_fadeAlpha<=0) {
        [[self window] orderOut:self];
        [self stopTimer];
        return;
    }
	[[self window] setAlphaValue:_fadeAlpha];	
}

- (void)notifyFade {
    if (_fadeTimer) [self stopTimer];
    _fadeTimer=[NSTimer scheduledTimerWithTimeInterval:FADEWAIT target:self selector:@selector(fadeStart) userInfo:nil repeats:NO];
	NSLog(@"Fading");
}

- (void) show{
	int i = rand() % 2;
	if(i == 1 ) {
		[image setImage:_image1];
	} else {
		[image setImage:_image2];
	}
	if([_sound isPlaying]) {
		[_sound stop];
	}
	[_sound play];
	[[self window] orderFront:self];	
	[[self window] setAlphaValue:_windowAlpha];	
	[self notifyFade];
}

@end
