//
// OVPlistBackedKeyValueMapImpl.cpp
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

#include "OVPlistBackedKeyValueMapImpl.h"
#include <iostream>

using namespace OpenVanilla;

OVPlistBackedKeyValueMapImpl::OVPlistBackedKeyValueMapImpl(CFStringRef bundleID)
{
	m_bundleID = CFStringCreateCopy(kCFAllocatorDefault, bundleID);
}

OVPlistBackedKeyValueMapImpl::~OVPlistBackedKeyValueMapImpl()
{
    CFRelease(m_bundleID);
}


bool OVPlistBackedKeyValueMapImpl::isReadOnly()
{
    return false;
}

bool OVPlistBackedKeyValueMapImpl::setKeyStringValue(const string& key, const string& value)
{
    CFStringRef cfKey = CFStringCreateWithCString(kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8);
    CFStringRef cfValue = CFStringCreateWithCString(kCFAllocatorDefault, value.c_str(), kCFStringEncodingUTF8);
    CFPreferencesSetAppValue(cfKey, cfValue, m_bundleID);
    CFPreferencesAppSynchronize(m_bundleID);
    CFRelease(cfKey);
    CFRelease(cfValue);
    return true;
}

bool OVPlistBackedKeyValueMapImpl::hasKey(const string& key)
{
    bool exist = false;
    CFStringRef cfKey = CFStringCreateWithCString(kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8);
    CFPreferencesAppSynchronize(m_bundleID);
    CFPropertyListRef obj = CFPreferencesCopyAppValue(cfKey, m_bundleID);
    if (obj) {
        exist = true;
        CFRelease(obj);
    }
    CFRelease(cfKey);
    return exist;
}

const string OVPlistBackedKeyValueMapImpl::stringValueForKey(const string& key)
{
    CFStringRef cfKey = CFStringCreateWithCString(kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8);
    CFPreferencesAppSynchronize(m_bundleID);
    CFPropertyListRef obj = CFPreferencesCopyAppValue(cfKey, m_bundleID);
    string result;
    if (obj) {
        if (CFGetTypeID(obj) == CFStringGetTypeID()) {
            size_t maxLength = CFStringGetLength((CFStringRef)obj) * 4 + 1;
            char *buf = (char *)calloc(maxLength, 1);
            Boolean success = CFStringGetCString((CFStringRef)obj, buf, maxLength, kCFStringEncodingUTF8);
            if (success) {
                result = buf;
            }
            free(buf);
        }
        CFRelease(obj);
    }
    CFRelease(cfKey);
    return result;
}
        
bool OVPlistBackedKeyValueMapImpl::setKeyIntValue(const string& key, int value)
{
    CFStringRef cfKey = CFStringCreateWithCString(kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8);
    CFNumberRef cfValue = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &value);
    CFPreferencesSetAppValue(cfKey, cfValue, m_bundleID);
    CFPreferencesAppSynchronize(m_bundleID);
    CFRelease(cfKey);
    CFRelease(cfValue);
    return true;
}

bool OVPlistBackedKeyValueMapImpl::setKeyBoolValue(const string& key, bool value)
{
    CFStringRef cfKey = CFStringCreateWithCString(kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8);
    CFPreferencesSetAppValue(cfKey, (value ? kCFBooleanTrue : kCFBooleanFalse), m_bundleID);
    CFPreferencesAppSynchronize(m_bundleID);
    CFRelease(cfKey);
    return true;
}

int OVPlistBackedKeyValueMapImpl::intValueForKey(const string& key)
{
    int result;
    CFStringRef cfKey = CFStringCreateWithCString(kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8);
    Boolean keyExists;
    result = (int)CFPreferencesGetAppIntegerValue(cfKey, m_bundleID, &keyExists);
    CFRelease(cfKey);
    return result;
}

bool OVPlistBackedKeyValueMapImpl::isKeyTrue(const string& key)
{
    Boolean result;
    Boolean keyExists;
    CFStringRef cfKey = CFStringCreateWithCString(kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8);
    result = CFPreferencesGetAppBooleanValue(cfKey, m_bundleID, &keyExists);
    CFRelease(cfKey);
    return result;
}

bool OVPlistBackedKeyValueMapImpl::shouldDelete()
{
    return false;
}

OVKeyValueMapImpl* OVPlistBackedKeyValueMapImpl::copy()
{
    return this;
}
