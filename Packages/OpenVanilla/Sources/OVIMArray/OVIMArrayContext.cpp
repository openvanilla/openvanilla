//
// OVIMArrayContext.cpp
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

#include "OVIMArrayContext.h"
#include "OVIMArray.h"

#include <cctype>
#include <sstream>

using namespace OpenVanilla;

namespace {

// Legacy key code constants used by the original Array engine.
const int kEscKey = 27;
const int kSpaceKey = 32;
const int kReturnKey = 13;
const int kDeleteKey = 127;
const int kBackspaceKey = 8;
const int kUpKey = 30;
const int kDownKey = 31;
const int kLeftKey = 28;
const int kRightKey = 29;

} // namespace

OpenVanilla::OVIMArrayContext::OVIMArrayContext(OVIMArray* module)
    : m_module(module)
    , m_state(WaitKey1)
    , m_candidatePage(0)
    , m_candidatesOnDuty(false)
{
    m_selKeys = module->mainTable()->findProperty("selkey");
    if (!m_selKeys.length()) {
        m_selKeys = "123456789";
    }
}

OpenVanilla::OVIMArrayContext::~OVIMArrayContext()
{
}

void OpenVanilla::OVIMArrayContext::startSession(OVLoaderService* loaderService)
{
    m_components.clear();
    m_candidates.clear();
    m_candidatesOnDuty = false;
    m_candidatePage = 0;
    changeState(WaitKey1);
}

void OpenVanilla::OVIMArrayContext::stopSession(OVLoaderService* loaderService)
{
    startSession(loaderService);
}

bool OpenVanilla::OVIMArrayContext::isComposing()
{
    return m_state == WaitCandidate;
}

// =========================================================================
// Key sequence helpers
// =========================================================================

bool OpenVanilla::OVIMArrayContext::isKeynameKey(char c) const
{
    return m_module->mainTable()->findKeyname(string(1, c)).length() > 0;
}

string OpenVanilla::OVIMArrayContext::composedReading() const
{
    string reading;
    for (vector<string>::const_iterator i = m_components.begin(); i != m_components.end(); ++i) {
        reading += m_module->mainTable()->findKeyname(*i);
    }
    return reading;
}

bool OpenVanilla::OVIMArrayContext::hasWildcard() const
{
    for (vector<string>::const_iterator i = m_components.begin(); i != m_components.end(); ++i) {
        if (*i == "?" || *i == "*") {
            return true;
        }
    }
    return false;
}

bool OpenVanilla::OVIMArrayContext::hasOnlyWildcard() const
{
    if (!m_components.size()) {
        return false;
    }

    for (vector<string>::const_iterator i = m_components.begin(); i != m_components.end(); ++i) {
        if (*i != "?" && *i != "*") {
            return false;
        }
    }
    return true;
}

string OpenVanilla::OVIMArrayContext::queryKey() const
{
    return OVStringHelper::Join(m_components);
}

string OpenVanilla::OVIMArrayContext::queryKeyName(const string& keys) const
{
    string outKeyNames;
    for (size_t i = 0; i < keys.length(); i++) {
        outKeyNames += m_module->mainTable()->findKeyname(keys.substr(i, 1));
    }
    return outKeyNames;
}

// =========================================================================
// Symbol table prefixes (w and hg of the v2023/v2026 Array30 tables)
// =========================================================================

bool OpenVanilla::OVIMArrayContext::isSymbolPrefix(const string& seq) const
{
    if (seq.size() == 1) {
        return seq[0] == 'w';
    }
    if (seq.size() == 2) {
        return seq[0] == 'h' && seq[1] == 'g';
    }
    return false;
}

bool OpenVanilla::OVIMArrayContext::isSymbolSeq(const string& seq) const
{
    return seq.size() > 1
        && isdigit((unsigned char)seq[seq.size() - 1])
        && isSymbolPrefix(seq.substr(0, seq.size() - 1));
}

bool OpenVanilla::OVIMArrayContext::isForceSPSeq() const
{
    return queryKey() == ",,sp";
}

// =========================================================================
// Candidate window helpers (the panel is display-only; this context handles
// every key itself, as the original engine did)
// =========================================================================

void OpenVanilla::OVIMArrayContext::showCandidatePage(OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
    if (!panel) {
        return;
    }

    size_t perPage = m_selKeys.length();
    size_t start = m_candidatePage * perPage;
    if (start >= m_candidates.size()) {
        start = 0;
    }

    size_t count = m_candidates.size() - start;
    if (count > perPage) {
        count = perPage;
    }

    vector<string> pageList(m_candidates.begin() + start, m_candidates.begin() + start + count);
    panel->candidateList()->setCandidates(pageList);
    panel->setCandidateKeys(m_selKeys.substr(0, count), loaderService);
    panel->updateDisplay();
    panel->show();
}

void OpenVanilla::OVIMArrayContext::clearCandidate(OVCandidateService* candidateService)
{
    m_candidatesOnDuty = false;
    OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
    if (panel) {
        panel->candidateList()->clear();
        panel->hide();
        panel->updateDisplay();
    }
}

bool OpenVanilla::OVIMArrayContext::candidateListHasOnlyOnePage() const
{
    return m_candidates.size() <= m_selKeys.length();
}

bool OpenVanilla::OVIMArrayContext::selectCandidateBySelKey(char key, string& outText)
{
    size_t perPage = m_selKeys.length();
    for (size_t i = 0; i < perPage; i++) {
        if (m_selKeys[i] == key) {
            size_t index = m_candidatePage * perPage + i;
            if (index < m_candidates.size()) {
                outText = m_candidates[index];
                return true;
            }
        }
    }

    return false;
}

void OpenVanilla::OVIMArrayContext::pageDown(OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    size_t perPage = m_selKeys.length() ? m_selKeys.length() : 1;
    size_t totalPages = (m_candidates.size() + perPage - 1) / perPage;
    if (totalPages <= 1) {
        return;
    }
    m_candidatePage += 1;
    if (m_candidatePage >= totalPages) {
        m_candidatePage = 0;
    }
    showCandidatePage(candidateService, loaderService);
}

void OpenVanilla::OVIMArrayContext::pageUp(OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (m_candidatePage > 0) {
        m_candidatePage -= 1;
        showCandidatePage(candidateService, loaderService);
    }
}

// =========================================================================
// State handlers
// =========================================================================

void OpenVanilla::OVIMArrayContext::updateDisplay(OVTextBuffer* composingText)
{
    composingText->setText(composedReading());
    composingText->updateDisplay();
}

void OpenVanilla::OVIMArrayContext::updateCandidate(OpenVanilla::OVCINDataTable* table, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (!table) {
        clearCandidate(candidateService);
        return;
    }

    m_candidates.clear();
    vector<pair<string, string> > pairs = table->findChardefWithWildcard(OVWildcard(queryKey()));
    for (vector<pair<string, string> >::iterator i = pairs.begin(); i != pairs.end(); ++i) {
        m_candidates.push_back((*i).second);
    }

    if (!m_candidates.size()) {
        clearCandidate(candidateService);
        return;
    }

    m_candidatesOnDuty = true;
    m_candidatePage = 0;
    showCandidatePage(candidateService, loaderService);
}

void OpenVanilla::OVIMArrayContext::showSymbolCandidates(OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    updateCandidate(m_module->mainTable(), composingText, candidateService, loaderService);
    if (m_candidates.empty()) {
        return;
    }
    composingText->setText(m_candidates[0]);
    composingText->updateDisplay();
    changeState(WaitCandidate);
}

int OpenVanilla::OVIMArrayContext::waitKey1(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (m_components.size() != 1) {
        return 0;
    }

    if (!hasWildcard()) {
        updateCandidate(m_module->shortcodeTable(), composingText, candidateService, loaderService);
    }

    char keycode = m_components[0][0];

    if (keycode == 't') {
        composingText->setText("\xE7\x9A\x84");    // 的
        composingText->updateDisplay();
    }

    if (isprint((unsigned char)keycode) && isKeynameKey(keycode)) {
        changeState(WaitKey2);
    }

    return 1;
}

int OpenVanilla::OVIMArrayContext::waitKey2(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (m_components.size() != 2) {
        return 0;
    }

    char keycode = m_components[1][0];

    if (isSymbolSeq(queryKey())) {
        showSymbolCandidates(composingText, candidateService, loaderService);
    }
    else {
        if (!hasWildcard()) {
            updateCandidate(m_module->shortcodeTable(), composingText, candidateService, loaderService);
        }

        if (isprint((unsigned char)keycode) && isKeynameKey(keycode)) {
            changeState(WaitKey3);
        }
    }

    return 1;
}

int OpenVanilla::OVIMArrayContext::waitKey3(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    if (isSymbolSeq(queryKey())) {
        showSymbolCandidates(composingText, candidateService, loaderService);
        return 1;
    }

    if (m_components.size() >= 3) {
        if (!hasWildcard()) {
            updateCandidate(m_module->mainTable(), composingText, candidateService, loaderService);
        }
    }

    return 1;
}

int OpenVanilla::OVIMArrayContext::waitCandidate(OVKey* key, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    const char keycode = (char)key->keyCode();

    if (keycode == kEscKey || keycode == kBackspaceKey) {
        clearAll(composingText, candidateService);
        changeState(WaitKey1);
        return RetDone;
    }

    if (keycode == kDownKey || keycode == kRightKey ||
        (!candidateListHasOnlyOnePage() && keycode == kSpaceKey)) {
        pageDown(candidateService, loaderService);
        return RetDone;
    }

    if (keycode == kUpKey || keycode == kLeftKey) {
        pageUp(candidateService, loaderService);
        return RetDone;
    }

    // enter == first candidate
    // space (when the candidate list has only one page) == first candidate
    char c = keycode;
    bool notSelkey = m_selKeys.find(keycode) == string::npos;

    bool defaultSelKey = (keycode == kReturnKey || (candidateListHasOnlyOnePage() && keycode == kSpaceKey));
    if (defaultSelKey || notSelkey) {
        c = m_selKeys[0];
    }

    string output;

    if (selectCandidateBySelKey(c, output)) {
        sendAndReset(output, composingText, candidateService, loaderService);
        if (notSelkey && !defaultSelKey) {
            return RetContinue;
        }
        return RetDone;
    }

    return RetPass;
}

void OpenVanilla::OVIMArrayContext::dispatchStateHandler(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    switch (m_state) {
        case WaitKey1:
            waitKey1(key, composingText, candidateService, loaderService);
            break;
        case WaitKey2:
            waitKey2(key, composingText, candidateService, loaderService);
            break;
        case WaitKey3:
            waitKey3(key, composingText, candidateService, loaderService);
            break;
        default:
            break;
    }
}

void OpenVanilla::OVIMArrayContext::changeBackState()
{
    switch (m_state) {
        case WaitCandidate:
        case WaitKey2:
            changeState(WaitKey1);
            break;
        case WaitKey3:
            if (m_components.size() == 2) {
                changeState(WaitKey2);
            }
            else if (m_components.size() == 1) {
                changeState(WaitKey1);
            }
            break;
        default:
            break;
    }
}

void OpenVanilla::OVIMArrayContext::changeState(State s)
{
    m_state = s;
}

void OpenVanilla::OVIMArrayContext::clearAll(OVTextBuffer* composingText, OVCandidateService* candidateService)
{
    clearCandidate(candidateService);
    composingText->setText("");
    composingText->updateDisplay();
    m_components.clear();
}

// =========================================================================
// Composition and commit
// =========================================================================

void OpenVanilla::OVIMArrayContext::commitKeySeq(int tableIndex, const string& errorMessage, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    OVCINDataTable* table = 0;
    switch (tableIndex) {
        case MainTable: table = m_module->mainTable(); break;
        case ShortcodeTable: table = m_module->shortcodeTable(); break;
        case SpecialTable: table = m_module->specialTable(); break;
        case PhraseTable: table = m_module->phraseTable(); break;
    }

    if (hasOnlyWildcard()) {
        clearAll(composingText, candidateService);
        changeState(WaitKey1);
        return;
    }

    if (!table) {
        clearAll(composingText, candidateService);
        changeState(WaitKey1);
        composingText->showToolTip(errorMessage);
        return;
    }

    m_candidates.clear();
    vector<pair<string, string> > pairs = table->findChardefWithWildcard(OVWildcard(queryKey()));
    for (vector<pair<string, string> >::iterator i = pairs.begin(); i != pairs.end(); ++i) {
        m_candidates.push_back((*i).second);
    }

    if (m_candidates.size() == 1) {
        m_candidatePage = 0;
        string c;
        if (m_selKeys.length() && selectCandidateBySelKey(m_selKeys[0], c)) {
            sendAndReset(c, composingText, candidateService, loaderService);
        }
    }
    else if (m_candidates.size() > 1) {
        updateCandidate(table, composingText, candidateService, loaderService);
        string c;
        if (m_selKeys.length() && selectCandidateBySelKey(m_selKeys[0], c)) {
            composingText->setText(c);
            composingText->updateDisplay();
        }
        changeState(WaitCandidate);
    }
    else {
        clearAll(composingText, candidateService);
        changeState(WaitKey1);
        composingText->showToolTip(errorMessage);
    }
}

void OpenVanilla::OVIMArrayContext::sendAndReset(const string& text, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    // If ⎔, prompt and reset state.
    if (text == "\xE2\x8E\x94") {    // ⎔
        clearAll(composingText, candidateService);
        changeState(WaitKey1);
        composingText->showToolTip("無此字");
        return;
    }

    bool notifySP = false;
    string notifyText;

    // lookup special code
    if (m_module->isForceSP() || m_module->isAutoSP()) {

        // The special table is short enough to allow sequential search.
        OVFastKeyValuePairMap* kvm = m_module->specialTable()->chardefMap();
        string matchKey;
        for (size_t i = 0, len = kvm->size(); i < len; i++) {
            const auto& kv = kvm->keyValuePairAtIndex(i);
            if (text == kv.second) {
                matchKey = kv.first;
                break;
            }
        }

        if (!matchKey.empty()) {
            if (queryKey() != matchKey) {
                string keynames = queryKeyName(matchKey);
                notifyText = text + ": " + keynames;
                notifySP = true;
            }
        }
    }

    if (!(m_module->isForceSP() && notifySP)) {
        composingText->setText(text);
        composingText->commit();
    }
    else {
        composingText->setText("");
        composingText->updateDisplay();
        loaderService->beep();
    }

    if (notifySP) {
        composingText->showToolTip(notifyText);
    }

    clearCandidate(candidateService);
    m_components.clear();
    changeState(WaitKey1);
}

// =========================================================================
// Key event entry point
// =========================================================================

bool OpenVanilla::OVIMArrayContext::handleKey(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    composingText->appendText(readingText->composedText());

    int ret = 0;
    const char keycode = (char)key->keyCode();
    const bool validkey = isKeynameKey(keycode) ||
        (isdigit((unsigned char)keycode) && isSymbolPrefix(queryKey()));

    if (!m_components.size() && !isprint((unsigned char)keycode)) {
        return false;
    }
    if (!m_components.size() && (key->isCombinedFunctionKey() || key->isNumLockOn())) {
        return false;
    }

    if (isprint((unsigned char)keycode) && key->isCapsLockOn() && m_components.size() == 0) {
        char c = key->isShiftPressed() ? toupper(keycode) : tolower(keycode);
        composingText->setText(string(1, c));
        composingText->commit();
        return true;
    }

    if (key->isShiftPressed() && isprint((unsigned char)keycode) && m_components.size() == 0 && keycode != '*' && keycode != '?') {
        composingText->setText(string(1, tolower(keycode)));
        composingText->commit();
        return true;
    }

    if (!m_components.size() && !validkey) {
        composingText->setText(string(1, tolower(keycode)));
        composingText->commit();
        return true;
    }

    if (keycode == kEscKey) {
        clearAll(composingText, candidateService);
        changeState(WaitKey1);
        return true;
    }

    // cancels candidate window if a wildcard character is entered
    if (!hasWildcard() && (keycode == '?' || keycode == '*')) {
        clearCandidate(candidateService);
        changeState(WaitKey3);
    }

    if (m_state == WaitCandidate) {
        int r = waitCandidate(key, composingText, candidateService, loaderService);
        if (r == RetDone) {
            ret = 1;
            goto syncAndReturn;
        }
        if (r == RetPass) {
            ret = 0;
            goto syncAndReturn;
        }
        // RetContinue: the first candidate was committed and the current key
        // starts a new composition; keep processing the key below.
        ret = 1;
    }

    if (m_candidatesOnDuty && isdigit((unsigned char)keycode) &&
        !isSymbolPrefix(queryKey())) {
        string c;
        if (selectCandidateBySelKey(keycode, c)) {
            if (c != "?") {
                sendAndReset(c, composingText, candidateService, loaderService);
            }
            else {
                loaderService->beep();
                if (m_state <= WaitKey3) {
                    // dirty hack to keep the candidate window on duty
                    updateCandidate(m_module->shortcodeTable(), composingText, candidateService, loaderService);
                }
            }
        }
        ret = 1;
        goto syncAndReturn;
    }

    if (m_components.size() && keycode == '\'') {
        commitKeySeq(PhraseTable, "無此詞彙", composingText, candidateService, loaderService);
        ret = 1;
        goto syncAndReturn;
    }

    if (m_components.size() && keycode == kSpaceKey) {
        // Detect ",,sp" to toggle the quick (force-SP) mode.
        if (isForceSPSeq()) {
            bool newState = !m_module->isForceSP();
            m_module->setForceSP(newState);
            stringstream s;
            s << "快打模式：" << (newState ? "啟用" : "關閉");
            composingText->showToolTip(s.str());
            clearAll(composingText, candidateService);
            changeState(WaitKey1);
            ret = 1;
            goto syncAndReturn;
        }
        commitKeySeq(MainTable, "無此字", composingText, candidateService, loaderService);
        ret = 1;
        goto syncAndReturn;
    }

    if (isprint((unsigned char)keycode) && validkey) {
        if (m_components.size() >= 5 ||
            (m_components.size() == 4 && keycode != 'i')) {
            ret = 1;
            goto syncAndReturn;
        }
        m_components.push_back(string(1, keycode));
        updateDisplay(composingText);
        ret = 1;
    }
    else if (keycode == kDeleteKey || keycode == kBackspaceKey) {
        if (m_components.size()) {
            m_components.pop_back();
        }
        updateDisplay(composingText);
        if (m_components.size() == 0 && m_candidatesOnDuty) {
            clearCandidate(candidateService);
        }
        changeBackState();
        ret = 1;
    }
    else {
        loaderService->beep();
        updateDisplay(composingText);
        ret = 1;
        goto syncAndReturn;
    }

    dispatchStateHandler(key, readingText, composingText, candidateService, loaderService);

syncAndReturn:
    // Mirror the original modern wrapper: when not composing, the reading
    // carries the key names and the composing text is committed; while
    // composing (candidate window up), the composing text carries the
    // pre-committed first candidate.
    if (isComposing()) {
        readingText->clear();
    }
    else {
        readingText->setText(composingText->composedText());
        composingText->setText("");
    }
    return ret;
}
