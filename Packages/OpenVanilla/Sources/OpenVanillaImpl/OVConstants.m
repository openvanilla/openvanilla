//
// OVConstants.m
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
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

#import "OVConstants.h"

NSString *const OVInputMethodConnectionName = @"OpenVanilla_1_Connection";

NSString *const OVActiveInputMethodIdentifierKey = @"ActiveInputMethod";
NSString *const OVAlphanumericKeyboardLayoutKey = @"AlphanumericKeyboardLayout";
NSString *const OVAlwayFallbackToAlphanumericKeyboardLayoutOnShiffKey = @"AlwayFallbackToAlphanumericKeyboardLayoutOnShiff";
NSString *const OVCandidateListStyleNameKey = @"CandidateListStyle";
NSString *const OVCandidateListTextSizeKey = @"CandidateListTextSize";
NSString *const OVCandidateTextFontNameKey = @"CandidateTextFontName";
NSString *const OVMakeSoundFeedbackOnInputErrorKey = @"MakeSoundFeedbackOnInputError";
NSString *const OVLastUpdateCheckTimeKey = @"LastUpdateCheckTime";
NSString *const OVNextUpdateCheckTimeKey = @"NextUpdateCheckTime";
NSString *const OVCheckForUpdateKey = @"CheckForUpdate";
NSString *const OVTraditionalToSimplifiedChineseFilterEnabledKey = @"EnableTraditionalToSimplifiedChineseFilter";
NSString *const OVSimplifiedToTraditionalChineseFilterEnabledKey = @"EnableSimplifiedToTraditionalChineseFilter";
NSString *const OVAssociatedPhrasesAroundFilterEnabledKey = @"EnableAssociatedPhrasesAroundFilter";

NSString *const OVDefaultInputMethodIdentifier = @"org.openvanilla.OVIMTableBased.cj";
NSString *const OVDefaultAlphanumericKeyboardLayoutIdentifier = @"com.apple.keylayout.US";
NSString *const OVDefaultCandidateListStyleName = @"vertical";
const NSUInteger OVDefaultCandidateListTextSize = 16;
const NSUInteger OVMaxCandidateListTextSize = 256;
const NSUInteger OVMinCandidateListTextSize = 12;

NSString *const OVVerticalCandidateListStyleName = @"vertical";
NSString *const OVHorizontalCandidateListStyleName = @"horizontal";

#if DEBUG
const NSTimeInterval OVNextUpdateCheckInterval = 600.0;
const NSTimeInterval OVNextUpdateCheckRemindLaterInterval = 3600.0;
const NSTimeInterval OVNextUpdateCheckRetryInterval = 300.0;
#else
const NSTimeInterval OVNextUpdateCheckInterval = 86400.0;
const NSTimeInterval OVNextUpdateCheckRemindLaterInterval = 86400.0 * 7.0;
const NSTimeInterval OVNextUpdateCheckRetryInterval = 3600.0;
#endif

NSString *const OVMainSiteURLString = @"https://openvanilla.org";
NSString *const OVUserGuideURLString = @"https://openvanilla.org/docs/";
NSString *const OVUpdateCheckInfoURLString = @"https://openvanilla.org/updates/Info.plist";
NSString *const OVUpdateDownloadURLString = @"https://openvanilla.org/updates/";
