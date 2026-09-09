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

#include "ArrayIMTestSupport.h"

#include "LegacyOVIMArray.h"

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
// Test doubles for the legacy (OV_Array) interfaces
// =========================================================================

class TestKey : public ::OVKeyCode {
    char c;
    bool shiftPressed = false;
    bool capsLockOn = false;
public:
    explicit TestKey(char ch) : c(ch) {}
    TestKey(char ch, bool shift, bool capslock) : c(ch), shiftPressed(shift), capsLockOn(capslock) {}
    int code() override { return c; }
    bool isShift() override { return shiftPressed; }
    bool isCapslock() override { return capsLockOn; }
    bool isCtrl() override { return false; }
    bool isAlt() override { return false; }
    bool isNum() override { return false; }
};

class TestBuffer : public OVBuffer {
public:
    string content;
    string committed;
    OVBuffer* clear() override { content.clear(); return this; }
    OVBuffer* append(const char* s) override { if (s) content += s; return this; }
    OVBuffer* send() override { committed += content; content.clear(); return this; }
    OVBuffer* update() override { return this; }
    OVBuffer* update(int, int = -1, int = -1) override { return this; }
    bool isEmpty() override { return content.empty(); }
};

class TestCandidate : public OVCandidate {
public:
    string content;
    bool shown = false;
    OVCandidate* clear() override { content.clear(); return this; }
    OVCandidate* append(const char* s) override { if (s) content += s; return this; }
    OVCandidate* hide() override { shown = false; return this; }
    OVCandidate* show() override { shown = true; return this; }
    OVCandidate* update() override { return this; }
    bool onScreen() override { return shown; }
};

class TestDictionary : public OVDictionary {
public:
    bool keyExist(const char*) override { return false; }
    int getInteger(const char*) override { return 0; }
    int setInteger(const char*, int) override { return 1; }
    const char* getString(const char*) override { return ""; }
    const char* setString(const char*, const char* v) override { return v; }
};

class TestService : public OVService {
public:
    int beeps = 0;
    string lastNotify;
    void beep() override { beeps++; }
    void notify(const char* msg) override { lastNotify = msg ? msg : ""; }
    const char* locale() override { return "zh_TW"; }
    const char* userSpacePath(const char*) override { return "/tmp"; }
    const char* pathSeparator() override { return "/"; }
    const char* toUTF8(const char*, const char* s) override { return s; }
    const char* fromUTF8(const char*, const char* s) override { return s; }
    const char* UTF16ToUTF8(unsigned short*, int) override { return ""; }
    int UTF8ToUTF16(const char*, unsigned short**) override { return 0; }
};

// =========================================================================
// Session
// =========================================================================

struct OVArrayLegacySession {
    OVIMArray module;
    TestDictionary dict;
    TestService srv;
    OVIMArrayContext* ctx = 0;
    TestBuffer buf;
    TestCandidate candi;
    vector<string> parsedCandidates;
    string tmpRoot;   // owns the copied tables; removed on destroy
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

// Parses the legacy candidate bar text ("1.甲 2.乙 ...") into plain values.
static vector<string> parseLegacyCandidates(const string& text)
{
    vector<string> result;
    size_t pos = 0;
    while (pos < text.size()) {
        size_t end = text.find(' ', pos);
        if (end == string::npos) end = text.size();
        string token = text.substr(pos, end - pos);
        pos = end + 1;
        if (token.size() >= 3 && token[1] == '.') {
            result.push_back(token.substr(2));
        }
    }
    return result;
}

static void refreshLegacyCandidates(OVArrayLegacySession* s)
{
    s->parsedCandidates = parseLegacyCandidates(s->candi.content);
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

    if (customMainTablePath && strlen(customMainTablePath)) {
        s->module.setCustomMainTablePath(customMainTablePath);
    }

    string root = s->tmpRoot + "/";
    if (!s->module.initialize(&s->dict, &s->srv, root.c_str())) {
        ov_array_session_destroy(s);
        return 0;
    }

    s->ctx = static_cast<OVIMArrayContext*>(s->module.newContext());
    if (!s->ctx) {
        ov_array_session_destroy(s);
        return 0;
    }

    refreshLegacyCandidates(s);
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
    if (!s->tmpRoot.empty()) {
        string cmd = "rm -rf \"" + s->tmpRoot + "\"";
        system(cmd.c_str());
    }
    delete s;
}

void ov_array_session_reset(OVArrayLegacySession* s)
{
    if (!s) return;
    s->ctx->clear();
    s->buf = TestBuffer();
    s->candi = TestCandidate();
    s->srv.beeps = 0;
    s->srv.lastNotify.clear();
    s->module.setAutoSP(true);
    s->module.setForceSP(false);
    refreshLegacyCandidates(s);
}

int ov_array_session_key(OVArrayLegacySession* s, char key)
{
    if (!s || !s->ctx) return 0;
    TestKey k(key);
    int handled = s->ctx->keyEvent(&k, &s->buf, &s->candi, &s->srv);
    refreshLegacyCandidates(s);
    return handled;
}

int ov_array_session_key_with_modifiers(OVArrayLegacySession* s, char key, int shift, int capslock)
{
    if (!s || !s->ctx) return 0;
    TestKey k(key, shift != 0, capslock != 0);
    int handled = s->ctx->keyEvent(&k, &s->buf, &s->candi, &s->srv);
    refreshLegacyCandidates(s);
    return handled;
}

int ov_array_session_beep_count(OVArrayLegacySession* s)
{
    return s ? s->srv.beeps : 0;
}

void ov_array_session_set_force_sp(OVArrayLegacySession* s, int on)
{
    if (s) s->module.setForceSP(on != 0);
}

void ov_array_session_set_auto_sp(OVArrayLegacySession* s, int on)
{
    if (s) s->module.setAutoSP(on != 0);
}

int ov_array_session_is_force_sp(OVArrayLegacySession* s)
{
    return s ? (s->module.isForceSP() ? 1 : 0) : 0;
}

int ov_array_session_is_composing(OVArrayLegacySession* s)
{
    return s && s->ctx ? (s->ctx->isComposing() ? 1 : 0) : 0;
}

const char* ov_array_session_buffer(OVArrayLegacySession* s)
{
    return s ? s->buf.content.c_str() : "";
}

const char* ov_array_session_committed(OVArrayLegacySession* s)
{
    return s ? s->buf.committed.c_str() : "";
}

int ov_array_session_candidates_shown(OVArrayLegacySession* s)
{
    return s ? (s->candi.shown ? 1 : 0) : 0;
}

int ov_array_session_candidate_count(OVArrayLegacySession* s)
{
    return s ? (int)s->parsedCandidates.size() : 0;
}

const char* ov_array_session_candidate_at(OVArrayLegacySession* s, int index)
{
    if (!s || index < 0 || (size_t)index >= s->parsedCandidates.size()) return "";
    return s->parsedCandidates[(size_t)index].c_str();
}

const char* ov_array_session_last_notify(OVArrayLegacySession* s)
{
    return s ? s->srv.lastNotify.c_str() : "";
}
