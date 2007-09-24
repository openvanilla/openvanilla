/* SnippetWindow */

#import <Cocoa/Cocoa.h>

@interface SnippetWindow : NSPanel
{
	float _currentWidth;
	float _currentHeight;
	float _currentX;
	float _currentY;	
	BOOL _isZoomed;
	NSSound * _soundEffet;
}
@end
