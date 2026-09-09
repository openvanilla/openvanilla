// Copyright (c) 2022 and onwards The OpenVanilla Authors.
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
// Test doubles and session plumbing that let the Swift unit tests drive the
// modern-API Array input method (OpenVanilla::OVIMArray / OVIMArrayContext)
// without any AppKit dependency, mirroring the app's key routing.
//

#include "ArrayIMTestSupport.h"

#include "OVIMArray.h"
#include "OVIMArrayContext.h"
#include "OVTextBufferImpl.h"
#include "OVConcreteKeyImpl.h"

#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#if defined(__APPLE__)
#include <unistd.h>
#endif

using namespace std;
using namespace OpenVanilla;

// =========================================================================
// Test doubles for the modern interfaces
// =========================================================================

class StubCandidateList : public OpenVanilla::OVCandidateList {
public:
    vector<string> list;
    void clear() override { list.clear(); }
    size_t size() const override { return list.size(); }
    string candidateAtIndex(size_t i) const override { return i < list.size() ? list[i] : string(); }
    void setCandidate(size_t i, const string& c) override { if (i < list.size()) list[i] = c; }
    void setCandidates(const vector<string>& c) override { list = c; }
    void addCandidate(const string& c) override { list.push_back(c); }
    void addCandidates(const vector<string>& c) override { for (auto& s : c) list.push_back(s); }
};

class StubPanel : public OVOneDimensionalCandidatePanel {
public:
    StubCandidateList candList;
    bool visible = false;
    vector<OVKey> candidateKeys;
    size_t perPage = 10;

    void hide() override { visible = false; }
    void show() override { visible = true; }
    void updateDisplay() override {}
    bool isVisible() override { return visible; }
    void setPrompt(const string&) override {}
    string prompt() override { return ""; }
    bool yieldToCandidateEventHandler() override { return false; }
    void cancelEventHandler() override {}
    void reset() override { visible = false; candList.clear(); }
    bool isHorizontal() const override { return false; }
    bool isVertical() const override { return true; }
    OpenVanilla::OVCandidateList* candidateList() override { return &candList; }
    size_t candidatesPerPage() const override { return perPage; }
    void setCandidatesPerPage(size_t n) override { perPage = n; }
    size_t pageCount() const override { return 0; }
    size_t currentPage() const override { return 0; }
    size_t currentPageCandidateCount() const override { return candList.size(); }
    bool allowsPageWrapping() const override { return false; }
    void setAllowsPageWrapping(bool) override {}
    size_t currentHighlightIndex() const override { return 0; }
    void setHighlightIndex(size_t) override {}
    size_t currentHighlightIndexInCandidateList() const override { return 0; }
    size_t goToNextPage() override { return 0; }
    size_t goToPreviousPage() override { return 0; }
    size_t goToPage(size_t) override { return 0; }
    const OVKey candidateKeyAtIndex(size_t i) override { return candidateKeys[i]; }
    void setCandidateKeys(const OVKeyVector& keys) override { candidateKeys = keys; }
    void setNextPageKeys(const OVKeyVector&) override {}
    void setPreviousPageKeys(const OVKeyVector&) override {}
    void setNextCandidateKeys(const OVKeyVector&) override {}
    void setPreviousCandidateKeys(const OVKeyVector&) override {}
    void setCancelKeys(const OVKeyVector&) override {}
    void setChooseHighlightedCandidateKeys(const OVKeyVector&) override {}
    const OVKeyVector defaultCandidateKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultNextPageKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultNextCandidateKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultPreviousPageKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultPreviousCandidateKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultCancelKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultChooseHighlightedCandidateKeys() const override { return OVKeyVector(); }
    void setCandidateKeysAndLabels(const vector<pair<OVKey, string> >& pairs) override {
        candidateKeys.clear();
        for (auto& p : pairs) candidateKeys.push_back(p.first);
    }
};

class StubCandidateService : public OVCandidateService {
public:
    StubPanel panel;
    OVOneDimensionalCandidatePanel* useOneDimensionalCandidatePanel() override { return &panel; }
};

class StubLoaderService : public OVLoaderService {
public:
    int beepCount = 0;
    string lastNotify;
    void beep() override { beepCount++; }
    void notify(const string& message) override { lastNotify = message; }
    void HTMLNotify(const string&) override {}
    const string locale() const override { return "zh_TW"; }
    const OVKey makeOVKey(int characterCode, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false) override {
        return OVKey(new OVConcreteKeyImpl(characterCode, alt, opt, ctrl, shift, command, capsLock, numLock));
    }
    const OVKey makeOVKey(const string& receivedString, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false) override {
        return OVKey(new OVConcreteKeyImpl(receivedString, alt, opt, ctrl, shift, command, capsLock, numLock));
    }
    ostream& logger(const string& = "") override {
        static ostringstream buf;
        return buf;
    }
    OVDatabaseService* defaultDatabaseService() override { return 0; }
    OVDatabaseService* CINDatabaseService() override { return 0; }
    OVDatabaseService* SQLiteDatabaseService() override { return 0; }
    OVEncodingService* encodingService() override { return 0; }
    void __reserved1(const string&) override {}
    void __reserved2(const string&) override {}
    void __reserved3(const string&) override {}
    void __reserved4(const string&) override {}
    const string __reserved5() const override { return ""; }
    void __reserved6(const string&) override {}
    void __reserved7(const string&, const string&) override {}
    void* __reserved8(const string&) override { return 0; }
};

// =========================================================================
// Session
// =========================================================================

struct OVArrayLegacySession {
    OpenVanilla::OVIMArray* module = 0;
    OpenVanilla::OVIMArrayContext* ctx = 0;
    StubLoaderService lsvc;
    StubCandidateService csvc;
    OVTextBufferImpl reading;
    OVTextBufferImpl composing;
    string committedAccumulator;
    string tmpRoot;   // owns the copied tables; removed on destroy
    string cachedBuffer;
    string cachedNotify;
};

static bool copyFile(const string& src, const string& dst)
{
    FILE* in = fopen(src.c_str(), "rb");
    if (!in) return false;
    FILE* out = fopen(dst.c_str(), "wb");
    if (!out) {
        fclose(in);
        return false;
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        fwrite(buf, 1, n, out);
    }
    fclose(in);
    fclose(out);
    return true;
}

// =========================================================================
// Session API
// =========================================================================

static OVArrayLegacySession* createSession(const char* dataTablesRoot, const char* customMainTablePath)
{
    if (!dataTablesRoot || !strlen(dataTablesRoot)) {
        return 0;
    }

    OVArrayLegacySession* s = new OVArrayLegacySession;

#if defined(__APPLE__)
    char tmpl[] = "/tmp/ovimarray.XXXXXX";
    char* dir = mkdtemp(tmpl);
    if (!dir) {
        delete s;
        return 0;
    }
    s->tmpRoot = dir;
#else
    s->tmpRoot = "ovimarray-tmp";
#endif

    string arrayDir = s->tmpRoot + "/Array";
    string mkdirCmd = "mkdir -p \"" + arrayDir + "\"";
    if (system(mkdirCmd.c_str()) != 0) {
        ov_array_session_destroy(s);
        return 0;
    }

    const char* files[] = { "array30.cin", "array-shortcode.cin", "array-special.cin", "array-phrase.cin" };
    for (const char* f : files) {
        string src = string(dataTablesRoot) + "/" + f;
        string dst = arrayDir + "/" + f;
        if (!copyFile(src, dst)) {
            ov_array_session_destroy(s);
            return 0;
        }
    }

    s->module = new OpenVanilla::OVIMArray(s->tmpRoot);
    if (customMainTablePath && strlen(customMainTablePath)) {
        s->module->setCustomMainTablePath(customMainTablePath);
    }

    s->ctx = static_cast<OpenVanilla::OVIMArrayContext*>(s->module->createContext());
    if (!s->ctx) {
        ov_array_session_destroy(s);
        return 0;
    }

    s->ctx->startSession(&s->lsvc);
    return s;
}

OVArrayLegacySession* ov_array_session_create(const char* dataTablesRoot)
{
    return createSession(dataTablesRoot, 0);
}

OVArrayLegacySession* ov_array_session_create_with_table(const char* dataTablesRoot, const char* customMainTablePath)
{
    return createSession(dataTablesRoot, customMainTablePath);
}

void ov_array_session_destroy(OVArrayLegacySession* s)
{
    if (!s) return;
    if (s->ctx) {
        delete s->ctx;
        s->ctx = 0;
    }
    delete s->module;
    s->module = 0;
    if (!s->tmpRoot.empty()) {
        string cmd = "rm -rf \"" + s->tmpRoot + "\"";
        system(cmd.c_str());
    }
    delete s;
}

void ov_array_session_reset(OVArrayLegacySession* s)
{
    if (!s) return;
    s->reading = OVTextBufferImpl();
    s->composing = OVTextBufferImpl();
    s->committedAccumulator.clear();
    s->cachedBuffer.clear();
    s->cachedNotify.clear();
    s->lsvc.beepCount = 0;
    s->lsvc.lastNotify.clear();
    s->csvc.panel.reset();
    s->ctx->startSession(&s->lsvc);
    s->module->setAutoSP(true);
    s->module->setForceSP(false);
}

static int sendKey(OVArrayLegacySession* s, char key, bool shift, bool capslock)
{
    if (!s || !s->ctx) return 0;

    // Mirror OVInputMethodController: a tooltip is dismissed by the next key.
    if (s->reading.toolTipText().length() || s->composing.toolTipText().length()) {
        s->reading.clearToolTip();
        s->composing.clearToolTip();
    }

    OVKey k = s->lsvc.makeOVKey((int)key, false, false, false, shift, false, capslock, false);
    bool handled = s->ctx->handleKey(&k, &s->reading, &s->composing, &s->csvc, &s->lsvc);

    if (s->composing.isCommitted()) {
        s->committedAccumulator += s->composing.composedCommittedText();
        s->composing.finishCommit();
    }

    // Cache the observable strings: composedText()/toolTipText() return
    // values by value.
    s->cachedBuffer = (s->ctx->isComposing()) ? s->composing.composedText() : s->reading.composedText();
    s->cachedNotify = s->composing.toolTipText();

    return handled ? 1 : 0;
}

int ov_array_session_key(OVArrayLegacySession* s, char key)
{
    return sendKey(s, key, false, false);
}

int ov_array_session_key_with_modifiers(OVArrayLegacySession* s, char key, int shift, int capslock)
{
    return sendKey(s, key, shift != 0, capslock != 0);
}

int ov_array_session_is_composing(OVArrayLegacySession* s)
{
    return s && s->ctx ? (s->ctx->isComposing() ? 1 : 0) : 0;
}

const char* ov_array_session_buffer(OVArrayLegacySession* s)
{
    return s ? s->cachedBuffer.c_str() : "";
}

const char* ov_array_session_committed(OVArrayLegacySession* s)
{
    return s ? s->committedAccumulator.c_str() : "";
}

int ov_array_session_candidates_shown(OVArrayLegacySession* s)
{
    return s ? (s->csvc.panel.visible ? 1 : 0) : 0;
}

int ov_array_session_candidate_count(OVArrayLegacySession* s)
{
    return s ? (int)s->csvc.panel.candList.size() : 0;
}

const char* ov_array_session_candidate_at(OVArrayLegacySession* s, int index)
{
    if (!s || index < 0 || (size_t)index >= s->csvc.panel.candList.list.size()) return "";
    return s->csvc.panel.candList.list[(size_t)index].c_str();
}

const char* ov_array_session_last_notify(OVArrayLegacySession* s)
{
    return s ? s->cachedNotify.c_str() : "";
}

int ov_array_session_beep_count(OVArrayLegacySession* s)
{
    return s ? s->lsvc.beepCount : 0;
}

void ov_array_session_set_auto_sp(OVArrayLegacySession* s, int on)
{
    if (s && s->module) s->module->setAutoSP(on != 0);
}

void ov_array_session_set_force_sp(OVArrayLegacySession* s, int on)
{
    if (s && s->module) s->module->setForceSP(on != 0);
}

int ov_array_session_is_force_sp(OVArrayLegacySession* s)
{
    return s && s->module ? (s->module->isForceSP() ? 1 : 0) : 0;
}
