//
// OVIMTableBasedContext.cpp
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

#include "OVIMTableBasedContext.h"
#include "OVIMTableBased.h"

using namespace OpenVanilla;

// A macOS-specific OVKeyCode that represents the Enter key on the numpad,
// or the key resulting from hitting Fn+Return.
constexpr unsigned int kMacEnter = 3;

OVIMTableBasedContext::OVIMTableBasedContext(OVIMTableBased* module)
    : m_module(module)
{
}

void OVIMTableBasedContext::startSession(OVLoaderService* loaderService)
{
    m_module->checkTable();
    m_components.clear();
}

bool OVIMTableBasedContext::handleKey(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (!m_module->m_table) {
        return false;
    }

    bool isNumPadKey = key->isNumLockOn();
    // If m_configOnlyUseNumPadNumbersForRadicals is true, "flip" the num pad key flag, so that
    // the number keys in the standard key row would become num pad keys, and only the numbers
    // coming from the num pad are treated as "normal" numbers.
    //
    // This is so as to allow certain input method tables to only use the num pad numbers as the
    // radicals while allowing the numbers in the standard row to function as number keys.
    if (key->keyCode() >= '0' && key->keyCode() <= '9' && m_module->m_configOnlyUseNumPadNumbersForRadicals) {
        isNumPadKey = !isNumPadKey;
    }

    if ((!key->receivedString().size() || isNumPadKey || key->isCombinedFunctionKey()) && readingText->isEmpty()) {
        return false;
    }

    if ((!key->keyCode() || key->isDirectTextKey()) && readingText->isEmpty() && key->receivedString().size()) {
        composingText->setText(key->receivedString());
        composingText->commit();
        return true;
    }
    
    if (key->isCapsLockOn() || isNumPadKey || (key->isShiftPressed() && key->isKeyCodeAlpha() && !m_module->m_table->keynameMap()->isPairMapMixedCase())) {
        if (!readingText->isEmpty()) {
            readingText->clear();
            readingText->updateDisplay();
            m_components.clear();
        }

        string commitString;
        if (key->receivedString().size() > 1) {
            composingText->setText(key->receivedString());
        }
        else {
            if (!key->isPrintable()) {
                composingText->clear();
                return true;
            }

            char c = key->keyCode();
            if (c >= 'A' && c <= 'Z') {
                c = tolower(c);
            }
            else {
                c = toupper(c);
            }

            if (key->isCapsLockOn() && key->isShiftPressed()) {
                c = toupper(c);
            }
            composingText->setText(string(1, c));
        }
        composingText->commit();
        return true;
    }
    
    if (key->receivedString().size() && key->keyCode() != 32 && !key->isDirectTextKey()) {
        if (isValidKeyString(string(1, key->keyCode()))) {
            if (m_components.size() >= m_module->m_configMaximumRadicalLength) {
                loaderService->beep();
                return true;
            }

            m_components.push_back(string(1, key->keyCode()));
        }
        else {
            if (readingText->isEmpty()) {
                if (key->receivedString().size()) {
                    composingText->setText(key->receivedString());
                    composingText->commit();
                    return true;
                }

                return false;
            }
            else {
                readingText->setText(currentReading());
                readingText->updateDisplay();
                loaderService->beep();
                return true;
            }
        }
    }
    else if (key->keyCode() == OVKeyCode::Space || key->keyCode() == OVKeyCode::Return || key->keyCode() == kMacEnter) {
        if (readingText->isEmpty()) {

            if (key->keyCode() == OVKeyCode::Space) {
                composingText->setText(key->receivedString());
                composingText->commit();
                return true;
            }

            return false;
        }

        if (compose(readingText, composingText, candidateService, loaderService)) {
            return true;
        } else if (m_module->m_configClearReadingBufferAtCompositionError) {
            m_components.clear();
            readingText->clear();
            readingText->updateDisplay();
            return true;
        }
    }
    else if (key->keyCode() == OVKeyCode::Esc || key->keyCode() == OVKeyCode::Enter) {
        m_components.clear();
        readingText->clear();
        readingText->updateDisplay();
        return true;
    }
    else if (key->keyCode() == OVKeyCode::Backspace) {
        return handleBackspace(readingText, composingText, candidateService, loaderService);
    }
    else if (key->isDirectTextKey() && key->receivedString().size()) {
        m_components.clear();
        readingText->clear();
        readingText->updateDisplay();
        composingText->setText(key->receivedString());
        composingText->commit();
        return true;
    }
    else {
        loaderService->beep();
    }

    readingText->setText(currentReading());
    readingText->updateDisplay();

    bool excludeEndKey = (m_components.size() > 1) && (key->keyCode() == m_module->m_configMatchOneChar || key->keyCode() == m_module->m_configMatchZeroOrMoreChar);

    if ((isEndKey(key) && !excludeEndKey) || (m_module->m_configShouldComposeAtMaximumRadicalLength && m_components.size() >= m_module->m_configMaximumRadicalLength)) {
        compose(readingText, composingText, candidateService, loaderService);
    }
    else {
        if (m_module->m_configComposeWhileTyping) {
            compose(readingText, composingText, candidateService, loaderService, false);
        }
    }

    return true;
}

void OVIMTableBasedContext::candidateCanceled(OVCandidateService* candidateService, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
    readingText->clear();
    readingText->updateDisplay();
    m_components.clear();
}

bool OVIMTableBasedContext::candidateSelected(OVCandidateService* candidateService, const string& text, size_t index, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
    m_components.clear();
    readingText->clear();
    readingText->updateDisplay();
    composingText->setText(text);
    composingText->commit();
    return true;
}


bool OVIMTableBasedContext::candidateNonPanelKeyReceived(OVCandidateService* candidateService, const OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();

    if (!m_module->m_table) {
        panel->cancelEventHandler();
        return false;
    }

    OVCandidateList* candidates = panel->candidateList();
    string currentCandidate = candidates->candidateAtIndex(panel->currentHightlightIndexInCandidateList());

    if (key->keyCode() == OVKeyCode::Backspace) {
        panel->cancelEventHandler();
        return handleBackspace(readingText, composingText, candidateService, loaderService);
    }

    if (key->keyCode() == OVKeyCode::Space) {
        if (m_module->m_configSendFirstCandidateWithSpaceWithOnePageList) {
            panel->hide();
            panel->cancelEventHandler();
            m_components.clear();
            readingText->clear();
            readingText->updateDisplay();
            composingText->setText(panel->candidateList()->candidateAtIndex(0));
            composingText->commit();
            return true;
        }
    }

    if (key->keyCode() == kMacEnter) {
        panel->hide();
        panel->cancelEventHandler();
        m_components.clear();
        readingText->clear();
        readingText->updateDisplay();
        composingText->setText(panel->candidateList()->candidateAtIndex(0));
        composingText->commit();
        return true;
    }

    if (!(currentCandidate.length() && isValidKeyString(string(1, key->keyCode())) && !key->isDirectTextKey())) {
        m_components.clear();
        readingText->clear();
        readingText->updateDisplay();
        panel->hide();
        panel->cancelEventHandler();
        return false;
    }

    if (m_module->m_configComposeWhileTyping) {
        if (isValidKeyString(string(1, key->keyCode())) && m_components.size() < m_module->m_configMaximumRadicalLength) {
            m_components.push_back(string(1, key->keyCode()));
            readingText->setText(currentReading());
            readingText->updateDisplay();

            bool excludeEndKey = (m_components.size() > 1) && (key->keyCode() == m_module->m_configMatchOneChar || key->keyCode() == m_module->m_configMatchZeroOrMoreChar);
            bool hasCandidates = false;

            if ((isEndKey(key) && !excludeEndKey) || (m_module->m_configShouldComposeAtMaximumRadicalLength && m_components.size() >= m_module->m_configMaximumRadicalLength)) {
                hasCandidates = compose(readingText, composingText, candidateService, loaderService);
            }
            else if (m_module->m_configComposeWhileTyping) {
                hasCandidates = compose(readingText, composingText, candidateService, loaderService, false);
            }

            if (!hasCandidates) {
                panel->hide();
                panel->cancelEventHandler();
            }

            return true;
        }
    }

    composingText->setText(currentCandidate);
    composingText->commit();

    panel->hide();
    panel->cancelEventHandler();
    m_components.clear();
    m_components.push_back(string(1, key->keyCode()));
    readingText->setText(currentReading());
    readingText->updateDisplay();

    bool excludeEndKey = (m_components.size() > 1) && (key->keyCode() == m_module->m_configMatchOneChar || key->keyCode() == m_module->m_configMatchZeroOrMoreChar);
    if ((isEndKey(key) && !excludeEndKey) || (m_module->m_configShouldComposeAtMaximumRadicalLength && m_components.size() >= m_module->m_configMaximumRadicalLength)) {
        compose(readingText, composingText, candidateService, loaderService);
    }

    return true;
}

const string OVIMTableBasedContext::currentReading()
{
    string reading;
    for (vector<string>::const_iterator i = m_components.begin(), e = m_components.end(); i != e; ++i) {
        string r = keyNameForKeyString(*i);
        if (r.length()) {
            reading += r;
        }
        else {
            reading += *i;
        }
    }

    return reading;
}

const string OVIMTableBasedContext::currentQueryKey()
{
    return OVStringHelper::Join(m_components);
}

bool OVIMTableBasedContext::stringContainsWildcard(const string& str)
{
    char o = m_module->m_configMatchOneChar;
    char z = m_module->m_configMatchZeroOrMoreChar;
    for (string::const_iterator i = str.begin(), e = str.end(); i != e; ++i) {
        if (*i == o || *i == z) {
            return true;
        }
    }

    return false;
}

const string OVIMTableBasedContext::keyNameForKeyString(const string& keyString)
{
    string result = m_module->m_table->findKeyname(keyString);
    if (!result.size() && keyString.size() == 1) {
        if (keyString[0] == m_module->m_configMatchOneChar) {
            result = string(1, m_module->m_configMatchOneChar);
        }
        else if (keyString[0] == m_module->m_configMatchZeroOrMoreChar) {
            result = string(1, m_module->m_configMatchZeroOrMoreChar);
        }
    }

    return result;
}

bool OVIMTableBasedContext::compose(OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService, bool sendWhenOnlyOneCandidate)
{
    if (!m_module->m_table) {
        return false;
    }

    string queryKey = currentQueryKey();

    vector<string> results;

    if (stringContainsWildcard(queryKey) && queryKey.size() > 1) {
        OVWildcard predicate(queryKey, m_module->m_configMatchOneChar, m_module->m_configMatchZeroOrMoreChar);
        vector<pair<string, string> > pairedResults = m_module->m_table->findChardefWithWildcard(predicate);
        for (vector<pair<string, string> >::iterator i = pairedResults.begin(), e = pairedResults.end(); i != e; ++i) {
            results.push_back((*i).second);
        }
    }
    else {
        results = m_module->m_table->findChardef(queryKey);
    }

    if (!results.size()) {
        if (sendWhenOnlyOneCandidate) {
            loaderService->beep();
        }

        if (!m_module->m_configComposeWhileTyping && m_module->m_configClearReadingBufferAtCompositionError) {
            m_components.clear();
            readingText->clear();
            readingText->updateDisplay();
            return false;
        }
    }
    else if (results.size() == 1 && sendWhenOnlyOneCandidate) {
        OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();

        if (panel->isVisible()) {
            panel->hide();
            panel->cancelEventHandler();
        }

        m_components.clear();
        readingText->clear();
        readingText->updateDisplay();
        composingText->setText(results[0]);
        composingText->commit();
        return true;
    }
    else {
        OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();

        OVCandidateList* candidates = panel->candidateList();
        candidates->setCandidates(results);

        string candidateKeys = m_module->m_table->findProperty("selkey");
        if (!candidateKeys.length()) {
            candidateKeys = "123456789";
        }

        if (m_module->m_configUseSpaceAsFirstCandidateSelectionKey) {
            candidateKeys = string(" ") + candidateKeys;
        }

        if (results.size() < candidateKeys.length()) {
            candidateKeys = candidateKeys.substr(0, results.size());
        }

        panel->setCandidateKeys(candidateKeys, loaderService);

        OVKeyVector nextPageKeys;
        if (results.size() <= candidateKeys.length() && (m_module->m_configUseSpaceAsFirstCandidateSelectionKey || m_module->m_configSendFirstCandidateWithSpaceWithOnePageList)) {
            OVKeyVector v = panel->defaultNextPageKeys();
            OVKey spaceKey = loaderService->makeOVKey(OVKeyCode::Space, false, false, false, false, false, false, false);
            for (OVKeyVector::const_iterator i = v.begin(), e = v.end(); i != e; ++i) {
                if (!(*i == spaceKey)) {
                    nextPageKeys.push_back(*i);
                }
            }
        }
        else {
            nextPageKeys = panel->defaultNextPageKeys();
        }

        panel->setNextPageKeys(nextPageKeys);

        if (results.size() <= candidateKeys.size()) {
            panel->setCandidatesPerPage(results.size());
        }

        panel->updateDisplay();
        panel->show();
        panel->yieldToCandidateEventHandler();
        return true;
    }

    return false;
}

bool OVIMTableBasedContext::handleBackspace(OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (!m_components.size()) {
        return false;
    }

    m_components.pop_back();
    readingText->setText(currentReading());
    readingText->updateDisplay();

    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();

    if (m_module->m_configComposeWhileTyping) {
        if (!compose(readingText, composingText, candidateService, loaderService, false)) {
            panel->hide();
            panel->cancelEventHandler();
        }
    }
    else {
        if (panel->isVisible()) {
            panel->hide();
            panel->cancelEventHandler();
        }
    }

    return true;
}

bool OVIMTableBasedContext::isEndKey(const OVKey* key)
{
    string endKey = m_module->m_table->findProperty("endkey");
    return endKey.find((char)key->keyCode()) != string::npos;
}

bool OVIMTableBasedContext::isValidKeyString(const string& keyString)
{
    return keyNameForKeyString(keyString).length() > 0;
}
