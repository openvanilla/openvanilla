//
//  OVDisplayController.m
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "OVDisplayController.h"
#import "NSStringExtension.h"
#import "OVHotkeyField.h"

@interface OVDisplayController(Private)
- (NSString *)_shortenedFilename:(NSString *)filename maxLength:(int)maxLength;
- (NSArray *)_enumeratePath:(NSString *)path withExtension:(NSString *)ext;
@end

@implementation OVDisplayController(Private)
- (NSString *)_shortenedFilename:(NSString*)filename maxLength:(int)maxLength
{
    NSString *shortenedFilename = filename;
    if ([filename length] > (size_t)maxLength) {
        NSArray *pathComponents = [filename pathComponents];
        if ([pathComponents count] > 2) {
            shortenedFilename = [NSString stringWithFormat:@"%@%@/.../%@",
								 [pathComponents objectAtIndex:0],
								 [pathComponents objectAtIndex:1],
								 [pathComponents objectAtIndex:[pathComponents count] - 1]];
        }
    }
    return shortenedFilename;
}
- (NSArray *)_enumeratePath:(NSString *)path withExtension:(NSString *)ext
{
    NSString *stdpath = [path stringByStandardizingPath];
    NSMutableArray *a = [[NSMutableArray new] autorelease];
    NSDirectoryEnumerator *direnum = [[NSFileManager defaultManager] enumeratorAtPath:stdpath];
	NSString *pname;
    while (pname = [direnum nextObject]) {
        if ([pname hasSuffix:ext])
            [a addObject:[stdpath stringByAppendingPathComponent:pname]];
        [direnum skipDescendents];
    }
    return a;
}
@end

@implementation OVDisplayController

- (void)loadNib
{
	[super loadNib];
	BOOL loaded = [NSBundle loadNibNamed:@"OVDisplayController" owner:self];
	NSAssert((loaded == YES), @"NIB did not load");	
}

- (void) dealloc
{
	[_beepSound release];
	[_sound release];
	[super dealloc];
}

- (void)setSound
{
	NSLog(_beepSound);
	
	[u_soundMenu removeAllItems];
	[u_soundMenu addItemWithTitle:MSG(@"Default")];
	
	NSArray *aiff = [self _enumeratePath:@"/System/Library/Sounds" withExtension:@".aiff"];
	NSEnumerator *e = [aiff objectEnumerator];
	NSString *soundFile;
	while (soundFile = [e nextObject]) {
		NSString *soundName = [[soundFile lastPathComponent] stringByDeletingPathExtension];
		[u_soundMenu addItemWithTitle:soundName];
		if ([soundFile isEqualToString:_beepSound]) {
			[u_soundMenu selectItemWithTitle:soundName];
		}
	}
	[u_soundMenu addItemWithTitle:MSG(@"Customize...")];
	
	
	if (!_beepSound || ![_beepSound length]) {
		[u_soundMenu selectItemAtIndex:0];
	}
	else if ([u_soundMenu selectedItem] == 0) {
		[u_soundMenu selectItem:[u_soundMenu lastItem]];
		
		NSString *title = [NSString stringWithFormat:@"%@ (%@)", MSG(@"Customized..."), [self _shortenedFilename:[_beepSound lastPathComponent] maxLength:20]];		
		[[u_soundMenu lastItem] setTitle:[NSString stringWithFormat:title]];
	}
}

- (void)awakeFromNib
{
	[u_fontManager setDelegate:self];
	
	NSMutableDictionary *d = [NSMutableDictionary dictionary];
	
	[d setValue:[NSString stringByColor:[NSColor whiteColor]] forKey:@"foreground"];
	[d setValue:[NSString stringByColor:[NSColor blueColor]] forKey:@"background"];
	[d setValue:@"TrebuchetMS" forKey:@"font"];
	[d setValue:[NSNumber numberWithFloat:14.0] forKey:@"size"];
	[d setValue:[NSNumber numberWithFloat:1.0] forKey:@"opacity"];
	[d setValue:@"default" forKey:@"notificationStyle"]; 	//default silent
	[d setValue:[NSNumber numberWithInt:1] forKey:@"useWindowAnimation"];

	[d addEntriesFromDictionary:[self dictionary]];
	
	[u_foreground setColor:[[d valueForKey:@"foreground"] colorByString]];
	[u_background setColor:[[d valueForKey:@"background"] colorByString]];
	
	NSString *fontName = [d valueForKey:@"font"];
	float fontSize = [[d valueForKey:@"size"] floatValue];
	NSFont *font = [NSFont fontWithName:fontName size:14];
	
	[u_fontLabel setFont:font];
	[u_fontLabel setStringValue:[NSString stringWithFormat:@"%@ %.f", [font displayName], fontSize]];
	
	[u_useWindowAnimation setIntValue:[[d valueForKey:@"useWindowAnimation"] intValue]];
	NSString *style = [d valueForKey:@"notificationStyle"];
	
	if ([style isEqualToString:@"silent"]) {
		[u_notificationStyle setIntValue:0];
	}
	else {
		[d setValue:@"default" forKey:@"notificationStyle"];
		[u_notificationStyle setIntValue:1];
	}
	
	float opacity = [[d valueForKey:@"opacity"] floatValue];
	[u_opacityScroll setIntValue:(int)(opacity * 100)];
	[u_opacityLabel setStringValue:[NSString stringWithFormat:@"%d%%", (int)(opacity * 100)]];
	
	[self setDictionary:d];
	[u_preview changeConfig:[self dictionary]];
	[self update];
	
	[u_hotkeyField setModuleController:self];
	_beepSound = @"";
	_beepSound = [_delegate beepSound];
	
	[self setSound];
}

#pragma mark Interface Builder actions.

- (IBAction)changeColor:(id)sender
{
	[self setValue:[NSString stringByColor:[u_foreground color]] forKey:@"foreground"];
	[self setValue:[NSString stringByColor:[u_background color]] forKey:@"background"];	
	[u_preview changeConfig:[self dictionary]];	
	[self updateAndWrite];
}
- (IBAction)changeOpacity:(id)sender
{
	[u_opacityLabel setStringValue:[NSString stringWithFormat:@"%d%%", [sender intValue]]];	
	float opacity = ([sender floatValue] / 100);
	[self setValue:[NSNumber numberWithFloat:opacity] forKey:@"opacity"];
	[self updateAndWrite];
}
- (IBAction)changeSound:(id)sender
{
	if (_sound) {
        if ([_sound isPlaying]) 
			[_sound stop];
    }
	
	if ([sender indexOfSelectedItem] == 0) {
		_beepSound = @"";
	}
	else if ([sender indexOfSelectedItem] == ([[u_soundMenu itemArray] count] - 1)) {
		NSOpenPanel *op = [NSOpenPanel openPanel];
		NSArray *filetypes = [NSArray arrayWithObjects:@"aiff", @"aif", @"aifc", @"wav", @"wave", @"snd", @"au", @"mp3", @"ulw", @"mp4", @"m4a", nil];
        [op setAllowsMultipleSelection:FALSE];
		[op setCanCreateDirectories:NO];
		[op setMessage:MSG(@"Please choose the audio clip which you want to use:")];
		[op setTitle:MSG(@"Choose Audio Clip...")];
		[op setPrompt:MSG(@"Choose")];
		if ([op runModalForDirectory:nil file:nil types:filetypes] == NSOKButton) {
			NSString *soundFile = [[op filenames] objectAtIndex:0];			
			if (![[NSFileManager defaultManager] fileExistsAtPath:soundFile]){
				[u_soundMenu selectItemAtIndex:0];
				_beepSound = @"";
			}
			else {
				NSString *title = [NSString stringWithFormat:@"%@ (%@)", MSG(@"Customized..."), [self _shortenedFilename:[u_soundMenu lastPathComponent] maxLength:20]];	
				[[sender lastItem] setTitle:title];
				_beepSound = [soundFile retain];
			}
        }
        else {
            [sender selectItemWithTitle:MSG(@"Default")];
			[[sender lastItem] setTitle:MSG(@"Customized...")];
			_beepSound = @"";
        }
	}
	else {
		NSString *t = [sender titleOfSelectedItem];
		_beepSound = [NSString stringWithFormat:@"/System/Library/Sounds/%@.aiff", t];
		[[sender lastItem] setTitle:MSG(@"Customized...")];		
	}
	
	[self testSound:sender];
	[_delegate updateBeepSound:_beepSound];
}

- (IBAction)testSound:(id)sender
{
	if (_sound) {
		if ([_sound isPlaying])
			[_sound stop];
	}
	if ([u_soundMenu indexOfSelectedItem] == 0) {
		NSBeep();
	}
	else {
		if (_sound) {
			//[_sound release];
			_sound = nil;			
		}
		if ([_beepSound length]) {			
			_sound = [[NSSound alloc] initWithContentsOfFile:_beepSound byReference:YES];
			if (!_sound) 
				return;
			[_sound play];
			[_sound setDelegate:self];
			[u_testSoundButton setEnabled:NO];
		}
	}
}
- (void)sound:(NSSound *)sound didFinishPlaying:(BOOL)finishedPlaying
{
	[u_testSoundButton setEnabled:YES];
}
- (IBAction)launchFontPanel:(id)sender
{
	NSString *fontName = [[self dictionary] valueForKey:@"font"];
	float fontSize = [[[self dictionary] valueForKey:@"size"] floatValue];
	NSFont *font = [NSFont fontWithName:fontName size:fontSize];
	
	[u_fontManager setSelectedFont:font isMultiple:NO];
	[[u_fontManager fontPanel:YES] orderFront:self];
}

- (void)changeFont:(id)sender
{
    NSFont *newfont = [sender convertFont:[[NSFontManager sharedFontManager] selectedFont]];
	float fontSize = [newfont pointSize];	
	[self setValue:[newfont fontName] forKey:@"font"];	
	[self setValue:[NSNumber numberWithFloat:fontSize] forKey:@"size"];
	
	[u_fontLabel setStringValue:[NSString stringWithFormat:@"%@ %.f", [newfont displayName], fontSize]];
	[u_preview changeConfig:[self dictionary]];	
	[self updateAndWrite];
}

- (IBAction)setDisplayServerSetting:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[u_useWindowAnimation intValue]] forKey:@"useWindowAnimation"];

	if ([u_notificationStyle intValue])
		[self setValue:@"default" forKey:@"notificationStyle"];
	else
		[self setValue:@"silent" forKey:@"notificationStyle"];
	[self updateAndWrite];

}
- (void)setShortcut:(NSString *)shortcut
{
	[super setShortcut:shortcut];
	if (![u_hotkeyField moduleController]) {
		[u_hotkeyField setModuleController:self];
	}
}
- (void)setShortcut:(NSString *)shortcut fromSender:(id)sender
{
	[super setShortcut:shortcut];
	[_delegate updateShortcut:shortcut forModule:@"fastIMSwitch"];
}
- (NSString *)beepSound
{
	return _beepSound;
}
@end
