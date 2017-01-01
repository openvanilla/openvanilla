//
// OVConstants.h
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

#import <Foundation/Foundation.h>

extern NSString *const OVInputMethodConnectionName;

extern NSString *const OVActiveInputMethodIdentifierKey;
extern NSString *const OVAlphanumericKeyboardLayoutKey;
extern NSString *const OVCandidateListStyleNameKey;
extern NSString *const OVCandidateListTextSizeKey;
extern NSString *const OVCandidateTextFontNameKey;
extern NSString *const OVMakeSoundFeedbackOnInputErrorKey;
extern NSString *const OVLastUpdateCheckTimeKey;
extern NSString *const OVNextUpdateCheckTimeKey;
extern NSString *const OVCheckForUpdateKey;
extern NSString *const OVTraditionalToSimplifiedChineseFilterEnabledKey;
extern NSString *const OVSimplifiedToTraditionalChineseFilterEnabledKey;
extern NSString *const OVAssociatedPhrasesAroundFilterEnabledKey;

extern NSString *const OVDefaultInputMethodIdentifier;
extern NSString *const OVDefaultAlphanumericKeyboardLayoutIdentifier;
extern NSString *const OVDefaultCandidateListStyleName;
extern NSString *const OVDefaultTextFontName;
extern const NSUInteger OVDefaultCandidateListTextSize;
extern const NSUInteger OVMaxCandidateListTextSize;
extern const NSUInteger OVMinCandidateListTextSize;

extern NSString *const OVVerticalCandidateListStyleName;
extern NSString *const OVHorizontalCandidateListStyleName;

extern const NSTimeInterval OVNextUpdateCheckInterval;
extern const NSTimeInterval OVNextUpdateCheckRemindLaterInterval;
extern const NSTimeInterval OVNextUpdateCheckRetryInterval;

extern NSString *const OVMainSiteURLString;
extern NSString *const OVUserGuideURLString;
extern NSString *const OVUpdateCheckInfoURLString;
extern NSString *const OVUpdateDownloadURLString;
