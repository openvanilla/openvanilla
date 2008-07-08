// OVDisplayController.mm : The controller for OpenVanilla Display Server settings.
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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
            shortenedFilename = [NSString stringWithFormat:@"%@%@/.../%@", [pathComponents objectAtIndex:0], [pathComponents objectAtIndex:1], [pathComponents objectAtIndex:[pathComponents count] - 1]];
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
	[m_beepSound release];
	[m_sound release];
	[super dealloc];
}

- (void)setSound
{
	[u_soundMenu removeAllItems];
	[u_soundMenu addItemWithTitle:MSG(@"Default")];
	
	NSArray *aiff = [self _enumeratePath:@"/System/Library/Sounds" withExtension:@".aiff"];
	NSEnumerator *e = [aiff objectEnumerator];
	NSString *soundFile;
	while (soundFile = [e nextObject]) {
		NSString *soundName = [[soundFile lastPathComponent] stringByDeletingPathExtension];
		[u_soundMenu addItemWithTitle:soundName];
		if ([soundFile isEqualToString:m_beepSound]) {
			[u_soundMenu selectItemWithTitle:soundName];
		}
	}
	[u_soundMenu addItemWithTitle:MSG(@"Customize...")];
	
	if (!m_beepSound || ![m_beepSound length]) {
		[u_soundMenu selectItemAtIndex:0];
	}
	else if ([u_soundMenu selectedItem] == 0) {
		[u_soundMenu selectItem:[u_soundMenu lastItem]];
		
		NSString *title = [NSString stringWithFormat:@"%@ (%@)", MSG(@"Customized..."), [self _shortenedFilename:[m_beepSound lastPathComponent] maxLength:20]];		
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
	[d setValue:[[NSNumber numberWithFloat:14.0] stringValue] forKey:@"size"];
	[d setValue:[[NSNumber numberWithFloat:1.0] stringValue] forKey:@"opacity"];
	[d setValue:@"default" forKey:@"notificationStyle"]; 
	[d setValue:[[NSNumber numberWithInt:1] stringValue] forKey:@"useWindowAnimation"];

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

	m_beepSound = [_delegate beepSound];
	
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
	if (m_sound) {
        if ([m_sound isPlaying]) 
			[m_sound stop];
    }
	
	if ([sender indexOfSelectedItem] == 0) {
		m_beepSound = @"";
	}
	else if ([sender indexOfSelectedItem] == ([[u_soundMenu itemArray] count] - 1)) {
		NSOpenPanel *openPanel = [NSOpenPanel openPanel];
		NSArray *filetypes = [NSArray arrayWithObjects:@"aiff", @"aif", @"aifc", @"wav", @"wave", @"snd", @"au", @"mp3", @"ulw", @"mp4", @"m4a", nil];
        [openPanel setAllowsMultipleSelection:FALSE];
		[openPanel setCanCreateDirectories:NO];
		[openPanel setMessage:MSG(@"Please choose the audio clip which you want to use:")];
		[openPanel setTitle:MSG(@"Choose Audio Clip...")];
		[openPanel setPrompt:MSG(@"Choose")];
		if ([openPanel runModalForDirectory:nil file:nil types:filetypes] == NSOKButton) {
			NSString *soundFile = [[openPanel filenames] objectAtIndex:0];			
			if (![[NSFileManager defaultManager] fileExistsAtPath:soundFile]){
				[u_soundMenu selectItemAtIndex:0];
				m_beepSound = @"";
			}
			else {
				NSString *title = [NSString stringWithFormat:@"%@ (%@)", MSG(@"Customized..."), [self _shortenedFilename:[u_soundMenu lastPathComponent] maxLength:20]];	
				[[sender lastItem] setTitle:title];
				m_beepSound = [soundFile retain];
			}
        }
        else {
            [sender selectItemWithTitle:MSG(@"Default")];
			[[sender lastItem] setTitle:MSG(@"Customized...")];
			m_beepSound = @"";
        }
	}
	else {
		NSString *t = [sender titleOfSelectedItem];
		m_beepSound = [NSString stringWithFormat:@"/System/Library/Sounds/%@.aiff", t];
		[[sender lastItem] setTitle:MSG(@"Customized...")];		
	}
	
	[self testSound:sender];
	[_delegate updateBeepSound:m_beepSound];
}

- (IBAction)testSound:(id)sender
{
	if (m_sound) {
		if ([m_sound isPlaying])
			[m_sound stop];
	}
	if ([u_soundMenu indexOfSelectedItem] == 0) {
		NSBeep();
	}
	else {
		if (m_sound) {
			//[m_sound release];
			m_sound = nil;			
		}
		if ([m_beepSound length]) {			
			m_sound = [[NSSound alloc] initWithContentsOfFile:m_beepSound byReference:YES];
			if (!m_sound) 
				return;
			[m_sound play];
			[m_sound setDelegate:self];
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
	if (_delegate)
		[_delegate updateShortcut:shortcut forModule:@"fastIMSwitch"];
}
- (NSString *)beepSound
{
	return m_beepSound;
}
@end
