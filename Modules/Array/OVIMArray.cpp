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
#include "LegacyOVIMArray.h"
#include "LegacyOVFrameworkWrapper.h"

using namespace OpenVanilla;

OpenVanilla::OVIMArray::OVIMArray(const string& tableRootPath)
    : m_lazyInitialized(false)
    , m_tableRootPath(tableRootPath)
    , m_legacyArrayModule(0)
    , m_cfgAutoSP(true)
    , m_cfgForceSP(false)
{
}

OpenVanilla::OVIMArray::~OVIMArray()
{
    if (m_legacyArrayModule) {
        delete m_legacyArrayModule;
    }
}

OVEventHandlingContext* OpenVanilla::OVIMArray::createContext()
{
    if (!m_legacyArrayModule) {
        if (m_lazyInitialized) {
            return 0;
        }

        m_lazyInitialized = true;
        m_legacyArrayModule = new ::OVIMArray;

        OVLegacyServiceWrapper service;
        OVLegacyDictionaryWrapper dictionary;

        // legacy module requires path with path separator in the end
        string tableRootPath = m_tableRootPath + string(1, OVPathHelper::Separator());
        int success = m_legacyArrayModule->initialize(&dictionary, &service, tableRootPath.c_str());
        if (!success) {
            delete m_legacyArrayModule;
            m_legacyArrayModule = 0;
        }
        else {
            m_legacyArrayModule->setAutoSP(m_cfgAutoSP);
            m_legacyArrayModule->setForceSP(m_cfgForceSP);
        }
    }

    if (m_legacyArrayModule) {
        ::OVIMArrayContext* legacyContext = static_cast<::OVIMArrayContext*>(m_legacyArrayModule->newContext());
        OpenVanilla::OVIMArrayContext* context = new OpenVanilla::OVIMArrayContext(legacyContext);
        return context;
    }
    else {
        return nullptr;
    }
}

const string OpenVanilla::OVIMArray::identifier() const
{
    return string("org.openvanilla.OVIMArray");
}

const string OpenVanilla::OVIMArray::localizedName(const string& locale)
{
    if (locale.find("zh") == 0) {
        return string("行列");
    }

    return string("Array");
}

bool OpenVanilla::OVIMArray::initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService)
{
    return true;
}

void OpenVanilla::OVIMArray::loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    if (moduleConfig->hasKey("SpecialCodePrompt")) {
        m_cfgAutoSP = moduleConfig->isKeyTrue("SpecialCodePrompt");
    }

    if (moduleConfig->hasKey("QuickMode")) {
        m_cfgForceSP = moduleConfig->isKeyTrue("QuickMode");
    }

    if (m_legacyArrayModule) {
        m_legacyArrayModule->setAutoSP(m_cfgAutoSP);
        m_legacyArrayModule->setForceSP(m_cfgForceSP);
    }

}

void OpenVanilla::OVIMArray::saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService)
{
    if (m_legacyArrayModule) {
        m_cfgAutoSP = m_legacyArrayModule->isAutoSP();
        m_cfgForceSP = m_legacyArrayModule->isForceSP();
    }

    moduleConfig->setKeyBoolValue("SpecialCodePrompt", m_cfgAutoSP);
    moduleConfig->setKeyBoolValue("QuickMode", m_cfgForceSP);

    if (!moduleConfig->hasKey("AlphanumericKeyboardLayout")) {
        moduleConfig->setKeyStringValue("AlphanumericKeyboardLayout", "com.apple.keylayout.US");
    }
}
