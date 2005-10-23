#import "CVEmbeddedLoader.h"
#import "CVLoaderUtility.h"
#import "CVDictionary.h"
#import "CVKeyCode.h"
#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"
#import "CVPreviewView.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: ovfilter [-l] name_of_the_output_filter\n");
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
