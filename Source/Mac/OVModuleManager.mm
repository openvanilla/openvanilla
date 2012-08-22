//
// OVModuleManager.mm
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

#import "OVModuleManager.h"
#import "OVLoaderServiceImpl.h"
#import "OVCandidateServiceImpl.h"
#import "OVIMTableBased.h"

using namespace OpenVanilla;

@implementation OVModuleManager
@synthesize loaderService = _loaderService;
@synthesize candidateService = _candidateService;
@synthesize inputMethod = _inputMethod;

+ (OVModuleManager *)defaultManager
{
    static OVModuleManager *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[OVModuleManager alloc] init];
    });

    return instance;
}

- (id)init
{
    self = [super init];
    if (self) {
        _loaderService = new OVLoaderServiceImpl;
        _candidateService = new OVCandidateServiceImpl(_loaderService);


        NSString *tableRoot = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"DataTables"];
        NSString *cjTable = [tableRoot stringByAppendingPathComponent:@"cj-ext.cin"];

        _inputMethod = new OVIMTableBased([cjTable UTF8String]);
        OVPathInfo info;
        bool result = self.inputMethod->initialize(&info, self.loaderService);
        NSLog(@"%s %d", __PRETTY_FUNCTION__, result);
    }
    return self;
}

- (void)dealloc
{
    delete _loaderService;
    delete _candidateService;
    delete _inputMethod;
    [super dealloc];
}
@end
