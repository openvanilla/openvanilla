// ovfilter.mm: command-line tool for invoking OpenVanilla filters
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#import "CVEmbeddedLoader.h"
#import "CVLoaderUtility.h"
#import "CVDictionary.h"
#import "CVKeyCode.h"
#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"
#import "CVPreviewView.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: ovfilter [-l] name_of_the_output_filter\n");
		return 1;
	}
	
	NSApplicationLoad();
	[NSAutoreleasePool new];

    CVEmbeddedLoader *loader=[[CVEmbeddedLoader alloc] init];
    if (!loader) {
		fprintf(stderr, "ovfilter: CVEmbeddedLoader initialization failed\n");
		return 1;
    }
	
	if (!strcasecmp(argv[1], "-l")) {
		NSArray *l=CVGetModulesByType([loader moduleList], @"OVOutputFilter");
		fprintf (stderr, "ovfilter: %d output filter(s) available:\n", [l count]);
		for (unsigned int i=0; i<[l count]; i++) {
			CVModuleWrapper *w=[l objectAtIndex:i];
			fprintf (stderr, "%s\n", [[w identifier] UTF8String]);
		}
		return 0;
	}
	
	
	NSString *fn=[NSString stringWithUTF8String:argv[1]];
	CVModuleWrapper *m=CVFindModule([loader moduleList], fn, @"OVOutputFilter");
	
	if (!m) {
		fprintf(stderr, "ovfilter: cannot find output filter %s\n", [fn UTF8String]);
		return 1;
	}

    NSMutableDictionary *md=[[[loader config] dictionary] valueForKey:fn default:[[NSMutableDictionary new] autorelease]];
	
    CVDictionary mcd(md);
    BOOL r=[m initializeWithConfig:&mcd service:[loader service]];
	if (!r) {
		fprintf(stderr, "ovfilter: output filter %s initialization failed\n", [fn UTF8String]);
		return 1;
	}

	OVOutputFilter *of=(OVOutputFilter*)[m module];

	while (!feof(stdin)) {
        [loader clearNotification];
		char buf[5121];
		fgets(buf, 5120, stdin);
		const char *result=of->process(buf, [loader service]);
		fprintf(stdout, "%s", result);
		NSString *nf=[loader notifyMessage];
		if ([nf length]) fprintf(stderr, "ovfilter: notification as follows\n%s\n", [nf UTF8String]);
	}
	return 0;
}
