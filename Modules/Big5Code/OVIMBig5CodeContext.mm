#include "OVIMBig5CodeContext.h"
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>

using namespace OpenVanilla;

OVIMBig5CodeContext::OVIMBig5CodeContext(OpenVanilla::OVIMBig5Code *module) :
m_module(module) {

}

void OVIMBig5CodeContext::candidateCanceled(OVCandidateService *candidateService, OVTextBuffer *readingText, OVTextBuffer *composingText, OVLoaderService *loaderService) {

}

bool OVIMBig5CodeContext::candidateSelected(OVCandidateService *candidateService, const string &text, size_t index, OVTextBuffer *readingText, OVTextBuffer *composingText, OVLoaderService *loaderService) {
    return false;
}

bool OVIMBig5CodeContext::candidateNonPanelKeyReceived(OVCandidateService *candidateService, const OVKey *key, OVTextBuffer *readingText, OVTextBuffer *composingText, OVLoaderService *loaderService) {
    return false;
}

void OVIMBig5CodeContext::startSession(OVLoaderService *loaderService) {

}

bool OVIMBig5CodeContext::handleKey(OVKey *key, OVTextBuffer *readingText, OVTextBuffer *composingText, OVCandidateService *candidateService, OVLoaderService *loaderService) {

    bool isNumPadKey = key->isNumLockOn();
    if (key->keyCode() >= '0' && key->keyCode() <= '9') {
        isNumPadKey = !isNumPadKey;
    }

    if ((!key->receivedString().size() || isNumPadKey || key->isCombinedFunctionKey()) && readingText->isEmpty()) {
        return false;
    }

    if (key->keyCode() == OVKeyCode::Esc) {
        if (m_code.length()) {
            readingText->clear();
            readingText->updateDisplay();
            composingText->clear();
            m_code.clear();
            return true;
        }
        return false;
    }
    else if (key->keyCode() == OVKeyCode::Backspace) {
        if (m_code.length()) {
            m_code.pop_back();
            if (m_code.length()) {
                readingText->setText(string("[內碼]") + m_code);
            } else {
                readingText->clear();
            }
            readingText->updateDisplay();
            return true;
        }
        return false;
    }


    char c = static_cast<char>(key->keyCode());
    c = toupper(c);

    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {
        m_code.push_back(c);
        if (m_code.length() >= 4) {
            long big5Code = (long)strtol(m_code.c_str(), NULL, 16);
            char bytes[3] = {0};
            bytes[0] = (big5Code >> CHAR_BIT) & 0xff;
            bytes[1] = static_cast<char>(big5Code & 0xff);
            CFStringRef cfString = CFStringCreateWithCString(NULL, bytes, kCFStringEncodingBig5);
            if (cfString == NULL) {
                m_code.clear();
                readingText->clear();
                readingText->updateDisplay();
                loaderService->beep();
                return true;
            }
            const char *utf8String = [(__bridge NSString *)cfString UTF8String];
            readingText->clear();
            readingText->updateDisplay();
            composingText->setText(string(utf8String));
            composingText->commit();
            m_code.clear();
        }
        else {
            readingText->setText(string("[內碼]") + m_code);
            readingText->updateDisplay();
        }
        return true;
    }

    loaderService->beep();
    return true;
}
