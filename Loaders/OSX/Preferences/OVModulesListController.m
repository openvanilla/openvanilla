// OVIMListController.m : The controller for the list of OpenVanilla modules.
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

#import "OVModulesListController.h"
#import "OVShortcutHelper.h"

@implementation OVModulesListController

#define DragDropSimplePboardType 	@"MyCustomOutlineViewPboardType"

- (void)awakeFromNib
{
	if (m_inputMethods == nil)
		m_inputMethods = [NSMutableArray new];
	if (m_ouputFilters == nil)
		m_ouputFilters = [NSMutableArray new];

    [u_outlineView registerForDraggedTypes:[NSArray arrayWithObjects:DragDropSimplePboardType, NSStringPboardType, NSFilenamesPboardType, nil]];
    [u_outlineView setDraggingSourceOperationMask:NSDragOperationEvery forLocal:YES];	
}

// This function is called when the main preference controller
// finishes generating the list of all modules.
// This function must be called once.
- (void)expandAll
{
	[u_outlineView setDelegate:self];
	[u_outlineView setDataSource:self];
	[u_outlineView reloadData];
	
	if (m_inputMethods) {
		[u_outlineView expandItem:m_inputMethods];
		if ([m_inputMethods count]) {
			[u_outlineView selectRowIndexes:[NSIndexSet indexSetWithIndex:1] byExtendingSelection:NO];
			id selectedController = [m_inputMethods objectAtIndex:0];
			[self switchToView:[selectedController view]];
			[u_hotkeyField setModuleController:selectedController];
		}
		else {
			[u_hotkeyView setHidden:YES];
		}
	}
	if (m_ouputFilters) {
		[u_outlineView expandItem:m_ouputFilters];
	}
}
- (void)dealloc
{
	[m_inputMethods release];
	[m_ouputFilters release];
	[super dealloc];
}
- (NSView *)view
{
	return u_mainView;
}
- (void)reload;
{
	[u_outlineView reloadData];
}
- (void)addInputMethod:(id)inputMethod
{
	if (m_inputMethods == nil) {
		m_inputMethods = [NSMutableArray new];
	}
	[m_inputMethods addObject:inputMethod];
}
- (void)addOutputFilter:(id)outputFilter
{
	if (m_ouputFilters == nil) {
		m_ouputFilters = [NSMutableArray new];
	}	
	[m_ouputFilters addObject:outputFilter];
}
- (void)switchToView:(NSView *)view
{
	if ([[u_settingView subviews] count]) {
		[[[u_settingView subviews] objectAtIndex:0] removeFromSuperview];
	}
	if (view) {
		NSRect frame = [view frame];
		frame.size.height = [u_settingView frame].size.height;
		frame.size.width = [u_settingView frame].size.width;
		[view setFrame:frame];
		[u_settingView addSubview:view];
	}
}

#pragma mark outlineView delegate methods.

- (int)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item
{
	if (item == nil) {
		return 2;
	}	
	else if (item == m_inputMethods) {
		return [m_inputMethods count];
	}
	else if (item == m_ouputFilters) {
		return [m_ouputFilters count];
	}
	return 0;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView shouldExpandItem:(id)item
{
	return YES;
}
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldCollapseItem:(id)item
{
	return NO;
}
- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item
{
	if (item == nil || item == m_inputMethods || item == m_ouputFilters) {
		return YES;
	}
	return NO;
}
- (id)outlineView:(NSOutlineView *)outlineView child:(int)index ofItem:(id)item
{
	if (item == nil) {
		if (index == 0)
			return m_inputMethods;
		else if (index == 1)
			return m_ouputFilters;
	}
	else if (item == m_inputMethods) {
		return [m_inputMethods objectAtIndex:index];
	}
	else if (item == m_ouputFilters) {
		return [m_ouputFilters objectAtIndex:index];
	}
	
	return nil;
}
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectTableColumn:(NSTableColumn *)tableColumn
{
	return NO;
}
- (BOOL)outlineView:(NSOutlineView *)outlineView isGroupItem:(id)item
{
	if (item == m_inputMethods || item == m_ouputFilters) {
		return YES;
	}	
	return NO;
}
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	if ([[tableColumn identifier] isEqualToString:@"enabled"]) {
		return YES;
	}
	return NO;
}
- (NSCell *)outlineView:(NSOutlineView *)outlineView dataCellForTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	if ([[tableColumn identifier] isEqualToString:@"enabled"]) {
		if (item == nil || item == m_inputMethods || item == m_ouputFilters)
			return [[[NSCell alloc] initTextCell:@""] autorelease];
	}
	return [tableColumn dataCell];
}
- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
	if ([[tableColumn identifier] isEqualToString:@"localizedName"]) {
		if (item == nil) {
			return @"/";
		}
		else if (item == m_inputMethods) {
			return MSG(@"Input Methods");
		}
		else if (item == m_ouputFilters) {
			return MSG(@"Output Filters");
		}
		else if ([item isKindOfClass:[OVModuleController class]]) {
			return [item localizedName];;
		}
	}
	else if ([[tableColumn identifier] isEqualToString:@"enabled"]) {
		if (![item isKindOfClass:[OVModuleController class]]) {
			return nil;
		}		
		else {
			BOOL enabled = [item isEnabled];
			return [NSNumber numberWithBool:enabled];
		}
	}
	else if ([[tableColumn identifier] isEqualToString:@"shortcut"]) {
		if (![item isKindOfClass:[OVModuleController class]]) {
			return nil;
		}		
		else {		
			return [OVShortcutHelper readableShortCut:[item shortcut]];
		}
	}
	return nil;
}
- (void)setCurrentItem:(id)item
{
	id tmp = m_currentItem;
	m_currentItem = [item retain];
	[tmp release];	
}
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectItem:(id)item
{
	if (item == nil || item == m_inputMethods || item == m_ouputFilters) {
		return NO;
	}
	if (![item isKindOfClass:[OVModuleController class]]) {
		return;
	}	
	[self setCurrentItem:item];
	return YES;
}
- (void)outlineViewSelectionDidChange:(NSNotification *)notification
{
	[u_hotkeyView setHidden:NO];
	[u_hotkeyField setModuleController:m_currentItem];
	[self switchToView:[m_currentItem view]];
}
- (float)indentationPerLevel
{
	return 0;
}
- (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item
{
	if (item == nil || item == m_inputMethods || item == m_ouputFilters)
		return;
	if (![item isKindOfClass:[OVModuleController class]])
		return;

	if ([[tableColumn identifier] isEqualToString:@"enabled"]) {
		BOOL enabled = [object boolValue];
		[item setEnabled:enabled];
		NSString *identifier = [item moduleIdentifer];
		NSLog(identifier);
		if (m_preferenceController) {
			if (enabled == YES)
				[m_preferenceController removeModuleFromExcludeList:identifier];
			else
				[m_preferenceController addModuleToExcludeList:identifier];
		}		
	}
}
- (BOOL)outlineView:(NSOutlineView *)outlineView writeItems:(NSArray *)items toPasteboard:(NSPasteboard *)pboard {

    id draggingItem = [items objectAtIndex:0];
	if (![m_ouputFilters containsObject:[items objectAtIndex:0]])
		return NO;
//	
//	m_draggingItem = draggingItem;
	NSData *data = [NSKeyedArchiver archivedDataWithRootObject:items];
	[pboard declareTypes:[NSArray arrayWithObjects:DragDropSimplePboardType, nil] owner:self];	
    [pboard setData:data forType:DragDropSimplePboardType]; 
    return YES;
}
- (NSDragOperation)outlineView:(NSOutlineView *)outlineView validateDrop:(id <NSDraggingInfo>)info proposedItem:(id)item proposedChildIndex:(int)childIndex
{
	if (childIndex < 0)
		return NSDragOperationNone;
	else if (item == m_inputMethods)
		return NSDragOperationNone;		

	return NSDragOperationGeneric;
}
- (BOOL)outlineView:(NSOutlineView *)outlineView acceptDrop:(id <NSDraggingInfo>)info item:(id)item childIndex:(int)childIndex
{
	NSPasteboard* pboard = [info draggingPasteboard];
	NSData *rowData = [pboard dataForType:DragDropSimplePboardType];
    NSArray	*items = [NSKeyedUnarchiver unarchiveObjectWithData:rowData];
	
	id draggingItem = [items objectAtIndex:0];
	
	[m_ouputFilters insertObject:[draggingItem copy] atIndex:childIndex];
	[m_ouputFilters removeObject:draggingItem];
	[draggingItem release];
	[u_outlineView reloadData];
	
	NSMutableArray *outputFilterOrder = [NSMutableArray array];
	NSEnumerator *enumerator = [m_ouputFilters objectEnumerator];
	id controller;
	while (controller = [enumerator nextObject]) {
		NSString *identifier = [controller moduleIdentifer];
		[outputFilterOrder addObject:identifier];
	}
	[m_preferenceController updateOutputFilterOrder:outputFilterOrder];
	
	return YES;
}
@end
