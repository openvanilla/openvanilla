//
// OVIMArray.cpp
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
//
// Based on the work of vgod
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

#include "OVIMArray.h"
#include "OVIMArrayContext.h"

using namespace OpenVanilla;

OVIMArray::OVIMArray(const string& tableRootPath)
{
}

OVIMArray::~OVIMArray()
{
}

OVEventHandlingContext* OVIMArray::createContext()
{
    return 0;
}

const string OVIMArray::identifier() const
{
    return string("org.openvanilla.OVIMArray");
}

const string OVIMArray::localizedName(const string& locale)
{
    if (locale.find("zh") == 0) {
        return string("行列");
    }

    return string("Array");
}

bool OVIMArray::initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService)
{
    return false;
}

void OVIMArray::loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    if (moduleConfig->hasKey("SpecialCodePrompt")) {
        m_cfgAutoSP = moduleConfig->isKeyTrue("SpecialCodePrompt");
    }

    if (moduleConfig->hasKey("QuickMode")) {
        m_cfgForceSP = moduleConfig->isKeyTrue("QuickMode");
    }        
}

void OVIMArray::saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    moduleConfig->setKeyBoolValue("SpecialCodePrompt", m_cfgAutoSP);    
    moduleConfig->setKeyBoolValue("QuickMode", m_cfgForceSP);

    if (!moduleConfig->hasKey("AlphanumericKeyboardLayout")) {
        moduleConfig->setKeyStringValue("AlphanumericKeyboardLayout", "com.apple.keylayout.US");
    }
}
