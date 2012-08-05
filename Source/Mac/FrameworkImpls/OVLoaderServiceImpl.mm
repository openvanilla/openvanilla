#import "OVLoaderServiceImpl.h"
#import <Cocoa/Cocoa.h>
#import "OVConcreteKeyImpl.h"

using namespace OpenVanilla;

void OVLoaderServiceImpl::beep()
{
    NSBeep();
}

const string OVLoaderServiceImpl::locale() const
{
    return "en";
}

const OVKey OVLoaderServiceImpl::makeOVKey(int characterCode, bool alt, bool opt, bool ctrl, bool shift, bool command, bool capsLock, bool numLock)
{
    OVConcreteKeyImpl keyImpl(characterCode, alt, opt, ctrl, shift, command, capsLock, numLock);
    return OVKey(&keyImpl);
}

const OVKey OVLoaderServiceImpl::makeOVKey(const string& receivedString, bool alt, bool opt, bool ctrl, bool shift, bool command, bool capsLock, bool numLock)
{
    OVConcreteKeyImpl keyImpl(receivedString, alt, opt, ctrl, shift, command, capsLock, numLock);
    return OVKey(&keyImpl);    
}

ostream& OVLoaderServiceImpl::logger(const string& sectionName)
{
    return cerr;
}

OVDatabaseService* OVLoaderServiceImpl::defaultDatabaseService()
{
    return 0;
}

OVDatabaseService* OVLoaderServiceImpl::CINDatabaseService()
{
    return 0;
}
