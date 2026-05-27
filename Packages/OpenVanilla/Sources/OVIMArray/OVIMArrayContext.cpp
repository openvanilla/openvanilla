//
// OVIMArrayContext.cpp
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

#include "OVIMArrayContext.h"
#include "OVIMArray.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <sstream>

using namespace OpenVanilla;
using namespace OpenVanilla::OVIMArrayConstants;

OpenVanilla::OVIMArrayContext::OVIMArrayContext(OVIMArray* module)
    : m_module(module)
    , m_keySequence(module->table(MainTable))
    , m_state(StateWaitKey1)
{
    m_selectionKeys = table(MainTable)->findProperty("selkey");
    if (!m_selectionKeys.length()) {
        m_selectionKeys = "123456789";
    }
}

OpenVanilla::OVIMArrayContext::~OVIMArrayContext()
{
}

void OpenVanilla::OVIMArrayContext::startSession(OVLoaderService* loaderService)
{
    clear();
}

void OpenVanilla::OVIMArrayContext::stopSession(OVLoaderService* loaderService)
{
    clear();
}

void OpenVanilla::OVIMArrayContext::clear()
{
    m_keySequence.clear();
    changeState(StateWaitKey1);
}

bool OpenVanilla::OVIMArrayContext::isComposing() const
{
    return m_state == StateWaitCandidate;
}

void OpenVanilla::OVIMArrayContext::updateDisplay(OVTextBuffer* readingText)
{
    readingText->setText(readingFromKeySequence());
    readingText->updateDisplay();
}

int OpenVanilla::OVIMArrayContext::updateCandidate(OVCINDataTable* table, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    m_candidateStrings.clear();
    vector<pair<string, string> > pairs = table->findChardefWithWildcard(OVWildcard(m_keySequence.getSeq()));
    for (vector<pair<string, string> >::const_iterator pair = pairs.begin(), end = pairs.end(); pair != end; ++pair) {
        m_candidateStrings.push_back(pair->second);
    }

    if (!m_candidateStrings.size()) {
        clearCandidate(candidateService);
        return 1;
    }

    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
    panel->candidateList()->setCandidates(m_candidateStrings);

    vector<pair<OVKey, string> > keyLabelPairs;
    for (string::const_iterator key = m_selectionKeys.begin(), end = m_selectionKeys.end(); key != end; ++key) {
        string label(1, *key);
        keyLabelPairs.push_back(make_pair(loaderService->makeOVKey(label), label));
    }
    panel->setCandidateKeysAndLabels(keyLabelPairs);
    if (m_candidateStrings.size() < m_selectionKeys.length()) {
        panel->setCandidatesPerPage(m_candidateStrings.size());
    }
    panel->setHighlightIndex(0);
    panel->updateDisplay();
    panel->show();
    return 1;
}

int OpenVanilla::OVIMArrayContext::waitKey1(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (m_keySequence.length() != 1) {
        return 0;
    }

    if (!m_keySequence.hasWildcardCharacter()) {
        updateCandidate(table(ShortTable), candidateService, loaderService);
    }

    char keycode = m_keySequence.getSeq()[0];
    if (keycode == 't') {
        readingText->setText("的");
        readingText->updateDisplay();
    }

    if (isprint(static_cast<unsigned char>(keycode)) && m_keySequence.valid(keycode)) {
        changeState(StateWaitKey2);
    }

    return 1;
}

int OpenVanilla::OVIMArrayContext::waitKey2(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (m_keySequence.length() != 2) {
        return 0;
    }

    char keycode = m_keySequence.getSeq()[1];
    if (isWSeq(m_keySequence.getSeq()[0], m_keySequence.getSeq()[1])) {
        updateCandidate(table(MainTable), candidateService, loaderService);
        if (m_candidateStrings.size()) {
            composingText->setText(m_candidateStrings[0]);
            composingText->updateDisplay();
        }
        changeState(StateWaitCandidate);
    }
    else {
        if (!m_keySequence.hasWildcardCharacter()) {
            updateCandidate(table(ShortTable), candidateService, loaderService);
        }

        if (isprint(static_cast<unsigned char>(keycode)) && m_keySequence.valid(keycode)) {
            changeState(StateWaitKey3);
        }
    }
    return 1;
}

int OpenVanilla::OVIMArrayContext::waitKey3(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (m_keySequence.length() >= 3 && !m_keySequence.hasWildcardCharacter()) {
        updateCandidate(table(MainTable), candidateService, loaderService);
    }
    return 1;
}

int OpenVanilla::OVIMArrayContext::waitCandidate(const OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    unsigned int keycode = key->keyCode();
    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();

    if (keycode == OVKeyCode::Esc || keycode == OVKeyCode::Backspace) {
        clearAll(readingText, composingText, candidateService);
        changeState(StateWaitKey1);
        return RetDone;
    }

    if (keycode == OVKeyCode::Down || keycode == OVKeyCode::Right || (panel->pageCount() > 1 && keycode == OVKeyCode::Space)) {
        panel->goToNextPage();
        panel->updateDisplay();
        return RetDone;
    }

    if (keycode == OVKeyCode::Up || keycode == OVKeyCode::Left) {
        panel->goToPreviousPage();
        panel->updateDisplay();
        return RetDone;
    }

    bool defaultSelectionKey = keycode == OVKeyCode::Return || (panel->pageCount() <= 1 && keycode == OVKeyCode::Space);
    bool notSelectionKey = keycode > 0 && keycode < 128 && m_selectionKeys.find(static_cast<char>(keycode)) == string::npos;

    string output;
    if (defaultSelectionKey) {
        if (selectCandidate(panel->currentPage() * panel->candidatesPerPage(), output)) {
            sendAndReset(output, readingText, composingText, candidateService, loaderService);
            return RetDone;
        }
    }
    else if (keycode < 128 && selectCandidate(static_cast<char>(keycode), candidateService, output)) {
        sendAndReset(output, readingText, composingText, candidateService, loaderService);
        return RetDone;
    }
    else if (notSelectionKey && selectCandidate(panel->currentPage() * panel->candidatesPerPage(), output)) {
        sendAndReset(output, readingText, composingText, candidateService, loaderService);
        return RetContinue;
    }

    return RetPass;
}

bool OpenVanilla::OVIMArrayContext::handleKey(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    unsigned int keycode = key->keyCode();
    const bool asciiKey = keycode > 0 && keycode < 128;
    const bool validKey = asciiKey && (m_keySequence.valid(static_cast<char>(keycode)) || (m_keySequence.getSeq()[0] == 'w' && isdigit(static_cast<unsigned char>(keycode))));

    if (!m_keySequence.length() && !key->isPrintable()) {
        return false;
    }

    if (key->isCombinedFunctionKey() && !m_keySequence.length()) {
        return false;
    }

    if (key->isKeyCodePrintable() && key->isCapsLockOn() && m_keySequence.length() == 0) {
        char output = key->isShiftPressed() ? static_cast<char>(toupper(keycode)) : static_cast<char>(tolower(keycode));
        composingText->setText(string(1, output));
        composingText->commit();
        return true;
    }

    if (key->isShiftPressed() && key->isKeyCodePrintable() && m_keySequence.length() == 0 && keycode != '*' && keycode != '?') {
        composingText->setText(string(1, static_cast<char>(tolower(keycode))));
        composingText->commit();
        return true;
    }

    if (!m_keySequence.length() && !validKey) {
        if (key->receivedString().length()) {
            composingText->setText(key->receivedString());
        }
        else if (asciiKey) {
            composingText->setText(string(1, static_cast<char>(tolower(keycode))));
        }
        else {
            return false;
        }
        composingText->commit();
        return true;
    }

    if (keycode == OVKeyCode::Esc) {
        clearAll(readingText, composingText, candidateService);
        changeState(StateWaitKey1);
        return true;
    }

    if (!m_keySequence.hasWildcardCharacter() && (keycode == '?' || keycode == '*')) {
        clearCandidate(candidateService);
        changeState(StateWaitKey3);
    }

    if (m_state == StateWaitCandidate) {
        int result = waitCandidate(key, readingText, composingText, candidateService, loaderService);
        if (result != RetContinue) {
            return result;
        }
    }

    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
    if (panel->isVisible() && asciiKey && isdigit(static_cast<unsigned char>(keycode)) && !(m_keySequence.length() == 1 && isWSeq(m_keySequence.getSeq()[0], static_cast<char>(keycode)))) {
        string candidate;
        if (selectCandidate(static_cast<char>(keycode), candidateService, candidate)) {
            if (candidate != "?") {
                sendAndReset(candidate, readingText, composingText, candidateService, loaderService);
            }
            else {
                loaderService->beep();
                if (m_state <= StateWaitKey3) {
                    updateCandidate(table(ShortTable), candidateService, loaderService);
                }
            }
        }
        return true;
    }

    if (m_keySequence.length() && keycode == '\'') {
        commitKeySeq(PhraseTable, "無此詞彙", readingText, composingText, candidateService, loaderService);
        return true;
    }

    if (m_keySequence.length() && keycode == OVKeyCode::Space) {
        if (isForceSPSeq()) {
            bool newState = !m_module->isForceSP();
            stringstream stream;
            stream << "快打模式：" << (newState ? "啟用" : "關閉");
            m_module->setForceSP(newState);
            composingText->showToolTip(stream.str());
            clearAll(readingText, composingText, candidateService);
            changeState(StateWaitKey1);
            return true;
        }
        commitKeySeq(MainTable, "無此字", readingText, composingText, candidateService, loaderService);
        return true;
    }

    bool handled = false;
    if (key->isKeyCodePrintable() && validKey) {
        if (m_keySequence.length() >= 5 || (m_keySequence.length() == 4 && keycode != 'i')) {
            return true;
        }
        m_keySequence.add(static_cast<char>(keycode));
        updateDisplay(readingText);
        handled = true;
    }
    else if (keycode == OVKeyCode::Delete || keycode == OVKeyCode::Backspace) {
        m_keySequence.remove();
        updateDisplay(readingText);
        if (m_keySequence.length() == 0 && panel->isVisible()) {
            clearCandidate(candidateService);
        }
        changeBackState(m_state);
        handled = true;
    }
    else {
        loaderService->beep();
        updateDisplay(readingText);
        return true;
    }

    dispatchStateHandler(key, readingText, composingText, candidateService, loaderService);
    return handled;
}

void OpenVanilla::OVIMArrayContext::candidateCanceled(OVCandidateService* candidateService, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
    clearAll(readingText, composingText, candidateService);
    changeState(StateWaitKey1);
}

bool OpenVanilla::OVIMArrayContext::candidateSelected(OVCandidateService* candidateService, const string& text, size_t index, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
    sendAndReset(text, readingText, composingText, candidateService, loaderService);
    return true;
}

bool OpenVanilla::OVIMArrayContext::candidateNonPanelKeyReceived(OVCandidateService* candidateService, const OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
    int result = waitCandidate(key, readingText, composingText, candidateService, loaderService);
    return result != RetPass;
}

void OpenVanilla::OVIMArrayContext::sendAndReset(const string& text, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (text == "⎔") {
        clearAll(readingText, composingText, candidateService);
        changeState(StateWaitKey1);
        composingText->showToolTip("無此字");
        return;
    }

    bool notifySP = false;
    string notifyText;

    if (m_module->isForceSP() || m_module->isAutoSP()) {
        OVFastKeyValuePairMap* map = table(SpecialTable)->chardefMap();
        string matchKey;
        for (size_t index = 0, size = map->size(); index < size; index++) {
            pair<string, string> kv = map->keyValuePairAtIndex(index);
            if (text == kv.second) {
                matchKey = kv.first;
                break;
            }
        }

        if (!matchKey.empty()) {
            int specialCodeLength = static_cast<int>(matchKey.length());
            const char* specialCode = matchKey.c_str();
            if (!equal(specialCode, specialCode + specialCodeLength, m_keySequence.getSeq())) {
                string keyNames;
                queryKeyName(specialCode, keyNames);
                notifyText = text + ": " + keyNames;
                notifySP = true;
            }
        }
    }

    if (!(m_module->isForceSP() && notifySP)) {
        composingText->setText(text);
        composingText->commit();
    }
    else {
        composingText->clear();
        composingText->updateDisplay();
        loaderService->beep();
    }

    if (notifySP) {
        composingText->showToolTip(notifyText);
    }

    clearCandidate(candidateService);
    m_keySequence.clear();
    readingText->clear();
    readingText->updateDisplay();
    changeState(StateWaitKey1);
}

void OpenVanilla::OVIMArrayContext::clearAll(OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService)
{
    clearCandidate(candidateService);
    readingText->clear();
    readingText->updateDisplay();
    composingText->clear();
    composingText->updateDisplay();
    m_keySequence.clear();
}

void OpenVanilla::OVIMArrayContext::clearCandidate(OVCandidateService* candidateService)
{
    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
    panel->candidateList()->clear();
    panel->hide();
    panel->cancelEventHandler();
    m_candidateStrings.clear();
}

bool OpenVanilla::OVIMArrayContext::selectCandidate(size_t index, string& out) const
{
    if (index >= m_candidateStrings.size()) {
        return false;
    }
    out = m_candidateStrings[index];
    return true;
}

bool OpenVanilla::OVIMArrayContext::selectCandidate(char key, OVCandidateService* candidateService, string& out) const
{
    size_t selectionIndex = m_selectionKeys.find(key);
    if (selectionIndex == string::npos) {
        return false;
    }

    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
    size_t candidateIndex = panel->currentPage() * panel->candidatesPerPage() + selectionIndex;
    return selectCandidate(candidateIndex, out);
}

bool OpenVanilla::OVIMArrayContext::isWSeq(char a, char b) const
{
    return a == 'w' && isdigit(static_cast<unsigned char>(b));
}

bool OpenVanilla::OVIMArrayContext::isForceSPSeq() const
{
    return m_keySequence.length() == 4 && !memcmp(m_keySequence.getSeq(), ",,sp", 4);
}

void OpenVanilla::OVIMArrayContext::queryKeyName(const char* keys, string& outKeyNames)
{
    int len = static_cast<int>(strlen(keys));
    for (int i = 0; i < len; i++) {
        string inKey(keys, i, 1);
        outKeyNames.append(table(MainTable)->findKeyname(inKey));
    }
}

void OpenVanilla::OVIMArrayContext::commitKeySeq(size_t tableIndex, const char* errorMessage, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (m_keySequence.hasOnlyWildcardCharacter()) {
        clearAll(readingText, composingText, candidateService);
        changeState(StateWaitKey1);
        return;
    }

    vector<pair<string, string> > pairs = table(tableIndex)->findChardefWithWildcard(OVWildcard(m_keySequence.getSeq()));
    m_candidateStrings.clear();
    for (vector<pair<string, string> >::const_iterator pair = pairs.begin(), end = pairs.end(); pair != end; ++pair) {
        m_candidateStrings.push_back(pair->second);
    }

    string candidate;
    if (m_candidateStrings.size() == 1) {
        if (selectCandidate(0, candidate)) {
            sendAndReset(candidate, readingText, composingText, candidateService, loaderService);
        }
    }
    else if (m_candidateStrings.size() > 1) {
        updateCandidate(table(tableIndex), candidateService, loaderService);
        if (selectCandidate(0, candidate)) {
            composingText->setText(candidate);
            composingText->updateDisplay();
        }
        changeState(StateWaitCandidate);
    }
    else {
        clearAll(readingText, composingText, candidateService);
        changeState(StateWaitKey1);
        composingText->showToolTip(errorMessage);
    }
}

void OpenVanilla::OVIMArrayContext::dispatchStateHandler(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    switch (m_state) {
        case StateWaitKey1:
            waitKey1(key, readingText, composingText, candidateService, loaderService);
            break;
        case StateWaitKey2:
            waitKey2(key, readingText, composingText, candidateService, loaderService);
            break;
        case StateWaitKey3:
            waitKey3(key, readingText, composingText, candidateService, loaderService);
            break;
        default:
            break;
    }
}

void OpenVanilla::OVIMArrayContext::changeBackState(State state)
{
    switch (state) {
        case StateWaitCandidate:
        case StateWaitKey2:
            changeState(StateWaitKey1);
            break;
        case StateWaitKey3:
            if (m_keySequence.length() == 2) {
                changeState(StateWaitKey2);
            }
            else if (m_keySequence.length() == 1) {
                changeState(StateWaitKey1);
            }
            break;
        default:
            break;
    }
}

void OpenVanilla::OVIMArrayContext::changeState(State state)
{
    m_state = state;
}

OVCINDataTable* OpenVanilla::OVIMArrayContext::table(size_t index) const
{
    return m_module->table(index);
}

string OpenVanilla::OVIMArrayContext::readingFromKeySequence() const
{
    string reading;
    m_keySequence.compose(reading);
    return reading;
}
