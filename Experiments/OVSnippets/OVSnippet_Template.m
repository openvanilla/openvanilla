//
//  OVSnippet_Template.m
//  OVSnippets
//
//  Created by zonble on 2007/9/24.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "OVSnippet_AppDelegate.h"

@implementation OVSnippet_AppDelegate(Template)

- (IBAction)htmlTemplate:(id)sender 
{
	[self insertSnippet:@"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n<title></title>\n<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />\n</head>\n<body>\n</body>\n</html>"];
}

- (IBAction)bidTemplate:(id)sender
{
	[self insertSnippet:u8string("不錯的賣家，送貨迅速，希望以後還有合作的機會，謝謝囉！")];
	[self insertSnippet:u8string("收到商品了，商品的品質—我很滿意。而回覆問題—也很迅速。")];
	[self insertSnippet:u8string("嗨！我已經匯款、也寄了匯款通知給你囉!如果沒收到再跟我說一下，我再寄一次給你喔。")];
	[self insertSnippet:u8string("親愛的買家您好。我們已確認收到您的匯款，貨今日會寄出，付款速度快、值得推薦好買家。請您留意收件！另外煩請不吝給我一個評價唷！謝謝！")];
	[self insertSnippet:u8string("商品已於○○○寄出囉，郵局小包號碼為○○○，如三日內未收到商品，請來信告知哦。")];
	[self insertSnippet:u8string("很感謝你對我們的支持，希望有機會能再次為您服務，記得有空多來逛逛喔！隨時有新貨色喔！")];
	[self insertSnippet:u8string("賣個東西而已，有必要這樣嗎？第一次見到如此惡劣的賣家！")];
}

@end
