#import "CVPreviewView.h"
#import "NSStringExtension.h"
#import "NSDictionaryExtension.h"
@implementation CVPreviewView
- (void)awakeFromNib {
    NSLog(@"view awake from nib!");
}

- (id)initWithFrame:(NSRect)frameRect
{
	if ((self = [super initWithFrame:frameRect]) != nil) {
        text=[[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 100, 50)];
        [text setEditable:NO];
        [text setSelectable:NO];
        [text setBordered:NO];
        [text setDrawsBackground:YES];
        [text setFrameOrigin:NSMakePoint(0,0)];
        [text setStringValue:@"OpenVanilla"];
        [self addSubview:text];
	}
	return self;
}

- (void)drawRect:(NSRect)rect
{
    [[NSColor whiteColor] set];
    NSRectFill(rect);
}

- (void)changeConfig:(NSDictionary*)d {
	// float alpha=[[d valueForKey:@"opacity" default:@"1.0"] floatValue];
    float alpha=1.0;
	NSColor *fc=[[d valueForKey:@"foreground" default:@"1.0 1.0 1.0"] colorByString];
	NSString *img=[d valueForKey:@"backgroundImage" default:@""];
	NSString *font=[d valueForKey:@"font" default:@"Lucida Grande"];
	float s=[[d valueForKey:@"size" default:@"18"] floatValue];

    if ([img length]) {
		NSImage *i=[[NSImage alloc] initByReferencingFile:img];
		if (i && [i isValid]) {
			[i autorelease];
			NSColor *c=[NSColor colorWithPatternImage:i];
			[text setBackgroundColor:[c colorWithAlphaComponent:alpha]];
		}
	}
    else {
        // no image, use color
        NSString *bcstr=[d valueForKey:@"background" default:@"1.0 1.0 1.0"];
        if ([bcstr isEqualToString:@"transparent"]) {
            [text setBackgroundColor:[NSColor clearColor]];
//           [text setBackgroundColor:[NSColor whiteColor]];
        }
        else {
            NSColor *bc=[bcstr colorByString];
            [text setBackgroundColor:[bc colorWithAlphaComponent:alpha]];
        }
    }
    
    [text setFont:[NSFont fontWithName:font size:s]];
	[text setTextColor:[fc colorWithAlphaComponent:alpha]];    
    [text sizeToFit];
    NSRect r=[text frame];
    NSRect vf=[self frame];
    [text setFrameOrigin:NSMakePoint((vf.size.width-r.size.width)/2, (vf.size.height-r.size.height)/2)];
    
    [self setNeedsDisplay:TRUE];
}

@end
