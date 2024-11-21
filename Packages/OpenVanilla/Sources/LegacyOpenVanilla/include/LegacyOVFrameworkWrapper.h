//
// LegacyOVFrameworkWrapper.h
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

#ifndef LegacyOVFrameworkWrapper_h
#define LegacyOVFrameworkWrapper_h

#include "LegacyOpenVanilla.h"
#include "OpenVanilla.h"

class OVLegacyKeyCodeWrapper : public OVKeyCode  {
public:
	OVLegacyKeyCodeWrapper(OpenVanilla::OVKey* key) : m_key(key) {}
    virtual int code() { return m_key->keyCode(); }
    virtual bool isShift() { return m_key->isShiftPressed(); }
    virtual bool isCapslock() { return m_key->isCapsLockOn(); }
    virtual bool isCtrl() { return m_key->isCtrlPressed(); }
    virtual bool isAlt() { return m_key->isAltPressed(); }
    virtual bool isNum() { return m_key->isNumLockOn(); }
    virtual bool isCommand() { return m_key->isCommandPressed(); }

protected:
    OpenVanilla::OVKey* m_key;
};

class OVLegacyBufferWrapper : public OVBuffer {
public:
    OVLegacyBufferWrapper(OpenVanilla::OVTextBuffer* buffer, OpenVanilla::OVCandidateService* candidateService)
        : m_buffer(buffer)
        , m_candidateService(candidateService)
    {
    }

    virtual OVBuffer* clear()
    {
        m_string = "";
        return this;
    }

    virtual OVBuffer* append(const char *s)
    {
        m_string += s;
        return this;
    }

    virtual OVBuffer* send()
    {
        m_buffer->setText(m_string);
        m_buffer->commit();
        m_candidateService->useOneDimensionalCandidatePanel()->hide();
        return this;
    }

    virtual OVBuffer* update()
    {
        m_buffer->setText(m_string);
        m_buffer->updateDisplay();
        return this;
    }

    virtual OVBuffer* update(int cursorPos, int markFrom = -1, int markTo = -1)
    {
        m_buffer->setText(m_string);
        m_buffer->setCursorPosition(cursorPos);
        m_buffer->updateDisplay();
        return this;
    }

    virtual bool isEmpty()
    {
        return m_string.length() == 0;
    }

protected:
    OpenVanilla::OVTextBuffer* m_buffer;
    OpenVanilla::OVCandidateService* m_candidateService;
    string m_string;
};

class OVLegacyCandidateWrapper : public OVCandidate  {
public:
    OVLegacyCandidateWrapper(OpenVanilla::OVCandidateService* candidateService, OpenVanilla::OVLoaderService* loaderService)
    {
        m_panel = candidateService->useOneDimensionalCandidatePanel();
        m_list = m_panel->candidateList();
        m_service = loaderService;
    }

    virtual OVCandidate* clear()
    {
        m_candidateString = "";
        m_list->clear();
        return this;
    }

    virtual OVCandidate* append(const char *s)
    {
        m_candidateString += s;
        return this;
    }

    virtual OVCandidate* hide()
    {
        m_panel->hide();
        return this;
    }

    virtual OVCandidate* show()
    {
        m_panel->show();
        return this;
    }

    virtual OVCandidate* update()
    {
        string candidateKeys;
        vector<string> candidates = OpenVanilla::OVStringHelper::Split(m_candidateString, ' ');
        
        for (vector<string>::const_iterator i = candidates.begin(), e = candidates.end(); i != e; ++i) {
            const string& s = *i;
            size_t len = s.length();
            if (len < 3) {
                continue;
            }

            if (!isprint(s[0]) || s[1] != '.') {
                continue;
            }

            candidateKeys += s.substr(0, 1);
            m_list->addCandidate(s.substr(2));
        }

//        m_panel->setCandidateKeys(candidateKeys, false, m_service);
        m_panel->setCandidateKeys(candidateKeys, m_service);
        m_panel->updateDisplay();
        return this;
    }

    virtual bool onScreen()
    {
        return m_panel->isVisible();
    }

protected:
    OpenVanilla::OVCandidateList* m_list;
    OpenVanilla::OVOneDimensionalCandidatePanel* m_panel;
    OpenVanilla::OVLoaderService* m_service;
    string m_candidateString;
};

class OVLegacyDictionaryWrapper : public OVDictionary {
public:
    virtual bool keyExist(const char *key) { return false; }
    virtual int getInteger(const char *key) { return 0; }
    virtual int setInteger(const char *key, int value) { return 0; }
    virtual const char *getString(const char *key) { return ""; }
    virtual const char *setString(const char *key, const char *value) { return ""; }
};

class OVLegacyServiceWrapper : public OVService {
public:
	OVLegacyServiceWrapper(OpenVanilla::OVLoaderService* service = 0, OpenVanilla::OVTextBuffer* textBuffer = 0)
        : m_service(service)
        , m_textBuffer(textBuffer)
    , m_locale(service ? service->locale() : "en")
    {
    }

    virtual void beep()
    {
        if (m_service) {
            m_service->beep();
        }
    }

    virtual void notify(const char *msg)
    {
        if (m_textBuffer) {
            m_textBuffer->showToolTip(msg);
        }
    }
    virtual const char *locale()
    {
        return m_locale.c_str();
    }

    virtual const char *userSpacePath(const char *modid)
    {
        return "/tmp";
    }

    virtual const char *pathSeparator()
    {
        return "/";
    }

    virtual const char *toUTF8(const char *encoding, const char *src) { return 0; }
    virtual const char *fromUTF8(const char *encoding, const char *src) { return 0; }
    virtual const char *UTF16ToUTF8(unsigned short *src, int len) { return 0; }
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) { return 0; }
    
protected:
    OpenVanilla::OVLoaderService* m_service;
    OpenVanilla::OVTextBuffer* m_textBuffer;
    std::string m_locale;
};


#endif
