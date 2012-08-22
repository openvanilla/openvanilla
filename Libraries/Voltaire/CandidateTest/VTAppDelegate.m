//
// VTAppDelegate.m
//
// Copyright (c) 2012 Lukhnos D. Liu (http://lukhnos.org)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import "VTAppDelegate.h"
#import "VTVerticalCandidateController.h"
#import "VTHorizontalCandidateController.h"


@interface VTAppDelegate (Private) <VTCandidateControllerDelegate>
@end

@implementation VTAppDelegate

- (void)dealloc
{
    [_keyLabelsFont release];
    [_candidatesFont release];
    [_candidates release];
    [_candidatesSource release];
    [_verticalCandidateController release];
    [_horizontalCandidateController release];
    [super dealloc];
}

- (void)setup
{
    _candidateController.delegate = self;        
    _candidateController.keyLabelFont = _keyLabelsFont;
    _candidateController.candidateFont = _candidatesFont;
    _candidateController.keyLabels = _keyLabels; 

    NSRect frameRect = [[self window] frame];
    _candidateController.windowTopLeftPoint = NSMakePoint(frameRect.origin.x + frameRect.size.width + 10.0, frameRect.origin.y + frameRect.size.height);    

    [_candidateController reloadData];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    _keyLabelsFont = [[NSFont systemFontOfSize:24.0] retain];
    _candidatesFont = [[NSFont systemFontOfSize:36.0] retain];
    
    _candidatesSource = [[NSArray arrayWithObjects:@"中文", @"輸入", @"法", @"café", @"あ", @"😄", @"查理", @"简体字", @"lô-má-jī", @"𠀳 (CJK Ext B)", @"文字", @"很長的文字", @"更長更長的文字", @"foo", @"bar", @"☆", @"☆☆", @"☆☆☆", @"☆☆☆☆", @"☆☆☆☆☆", nil] retain];

    _horizontalCandidateController = [[VTHorizontalCandidateController alloc] init];
    _verticalCandidateController = [[VTVerticalCandidateController alloc] init];
    _candidateController = _verticalCandidateController;

    [self changeKeyLabelsAction:nil];
    [self changeCandidateAmountAction:nil];
    [self setup];
}

- (IBAction)showAction:(id)sender
{
    [self setup];
    _candidateController.visible = YES;

}

- (IBAction)hideAction:(id)sender
{
    _candidateController.visible = NO;
}

- (void)moveUp:(id)sender
{
    if (_candidateController == _verticalCandidateController) {
        [_candidateController highlightPreviousCandidate];
    }
    else {
        [_candidateController showPreviousPage];
    }
}

- (void)moveDown:(id)sender
{
    if (_candidateController == _verticalCandidateController) {
        [_candidateController highlightNextCandidate];
    }
    else {
        [_candidateController showNextPage];
    }
}

- (void)moveLeft:(id)sender
{
    if (_candidateController == _horizontalCandidateController) {
        [_candidateController highlightPreviousCandidate];
    }
}

- (void)moveRight:(id)sender
{
    if (_candidateController == _horizontalCandidateController) {
        [_candidateController highlightNextCandidate];
    }
}

- (void)pageUp:(id)sender
{
    [_candidateController showPreviousPage];
}

- (void)pageDown:(id)sender
{
    [_candidateController showNextPage];
    
}

- (IBAction)changeWindowTypeAction:(id)sender
{
    BOOL visible = _candidateController.visible;
    _candidateController.visible = NO;

    if ([[sender selectedCell] tag] == 0) {
        _candidateController = _verticalCandidateController;
    }
    else {
        _candidateController = _horizontalCandidateController;
    }
    
    [self setup];
    _candidateController.visible = visible;
}

- (IBAction)changeCandidateAmountAction:(id)sender
{
    id tmp = _candidates;
    
    if ([[sender title] isEqualToString:@"One Page (short)"]) {
        _candidates = [[_candidatesSource subarrayWithRange:NSMakeRange(0, [_keyLabels count] / 2)] retain];
    }
    else if ([[sender title] isEqualToString:@"One Page (all keys)"]) {
        _candidates = [[_candidatesSource subarrayWithRange:NSMakeRange(0, [_keyLabels count])] retain];
    }
    else {
        _candidates = [_candidatesSource copy];
    }
    
    [tmp release];
    [self setup];
}

- (IBAction)changeKeyLabelsAction:(id)sender
{
    id tmp = _keyLabels;
    
    if ([[sender title] isEqualToString:@"Sel Keys: asdfghjk"]) {
        _keyLabels = [[NSArray arrayWithObjects:@"a", @"s", @"d", @"f", @"g", @"h", @"j", @"k", nil] retain];
    }
    else {        
        _keyLabels = [[NSArray arrayWithObjects:@"1", @"2", @"3", @"4", @"5", @"6", @"7", @"8", @"9", nil] retain];
    }
    
    [tmp release];
    [self setup];
}

- (void)fontChanged:(id)sender
{
    id tmp = _currentFont;
    
    if (_currentFont == _keyLabelsFont) {
        _keyLabelsFont = [[sender convertFont:_currentFont] retain];
        _currentFont = _keyLabelsFont;
    }
    else {
        _candidatesFont = [[sender convertFont:_currentFont] retain];        
        _currentFont = _candidatesFont;
    }
    
    [tmp release];
    [self setup];
}

- (IBAction)changeFontAction:(id)sender
{
    if ([[sender title] isEqualToString:@"Set Key Labels Font"]) {
        _currentFont = _keyLabelsFont;
    }
    else {
        _currentFont = _candidatesFont;
    }
    
    
    [[NSFontManager sharedFontManager] setAction:@selector(fontChanged:)];
    [[NSFontManager sharedFontManager] setTarget:self];
    [[NSFontManager sharedFontManager] setSelectedFont:_currentFont isMultiple:NO];
    [[[NSFontManager sharedFontManager] fontPanel:YES] orderFront:self];
}
@end


@implementation VTAppDelegate (Private)
- (NSUInteger)candidateCountForController:(VTCandidateController *)controller
{
    return [_candidates count];
}

- (id)candidateController:(VTCandidateController *)controller candidateAtIndex:(NSUInteger)index
{
    return [_candidates objectAtIndex:index];
}

- (void)candidateController:(VTCandidateController *)controller didSelectCandidateAtIndex:(NSUInteger)index
{
    NSRunAlertPanel(@"Candidate Selected", @"Index: %lu, candidate: %@", @"OK", nil, nil, index, [_candidates objectAtIndex:index]);
}
@end
