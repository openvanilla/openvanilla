//
//  OVIMArrayController.m
//  OVPreference

#import "OVIMArrayController.h"


@implementation OVIMArrayController

- (void)setDictinary:(NSDictionary *)dictionary
{
	[super setDictinary:dictionary];
	if (![[self dictionary] objectForKey:[NSString stringWithUTF8String:"快打模式"]]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:[NSString stringWithUTF8String:"快打模式"]];
	}
	[_quickModeCheckBox setIntValue:[[[self dictionary] valueForKey:[NSString stringWithUTF8String:"快打模式"]] intValue]];

	if (![[self dictionary] objectForKey:[NSString stringWithUTF8String:"特別碼提示"]]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:[NSString stringWithUTF8String:"特別碼提示"]];
	}	
	[_specialCodeCheckBox setIntValue:[[[self dictionary] valueForKey:[NSString stringWithUTF8String:"特別碼提示"]] intValue]];
}

- (IBAction)setQuickModeAction:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[sender intValue]] forKey:[NSString stringWithUTF8String:"快打模式"]];
	[self writeSetting];
}
- (IBAction)setSpecialCodeAction:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[sender intValue]] forKey:[NSString stringWithUTF8String:"特別碼提示"]];
	[self writeSetting];
}


@end
