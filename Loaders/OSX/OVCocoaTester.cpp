#include <Carbon/Carbon.h>

typedef int EntryPointType();
int main()
{
	CFURLRef url=CFURLCreateWithFileSystemPath(NULL,
		CFSTR("/Users/lukhnos/logiciel/OV0.7/Loaders/OSX/build/OVCocoa.bundle"),
		kCFURLPOSIXPathStyle, true);
		
	CFBundleRef bundle=CFBundleCreate(NULL, url);

    // CFBundleRef bundle=CFBundleGetBundleWithIdentifier(CFSTR("org.openvanilla.loader.cocoa"));
	if (!bundle)
	{
		fprintf (stderr, "bundle not found!\n");
		return 1;
	}
	
	EntryPointType *entry=(EntryPointType*)CFBundleGetFunctionPointerForName(bundle,
		CFSTR("EntryPoint"));
	if (!entry)
	{
		fprintf (stderr, "function not loaded!\n");
		return 1;
	}
	
	if (!entry())
	{
		fprintf (stderr, "entry point entered but failed!\n");
		return 1;
	}
	
	
	CFRelease(url);
	// CFRelease(bundle);
	return 0;
}
