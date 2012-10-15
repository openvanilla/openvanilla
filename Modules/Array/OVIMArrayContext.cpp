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
#include <cctype>

using namespace OpenVanilla;

OVIMArrayContext::OVIMArrayContext(OVIMArray* module)
	: parent(module)
    , keyseq(module->m_mainTable)
    , state(STATE_WAIT_KEY1)
{
}

void OVIMArrayContext::startSession(OVLoaderService* loaderService)
{
    parent->checkTables();
}

bool OVIMArrayContext::handleKey(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
    int result = keyEvent(key, composingText, candidateService, loaderService);
	return result != 0;
}

void OVIMArrayContext::changeBackState(STATE s)
{
    switch(s){
        case STATE_WAIT_CANDIDATE:
        case STATE_WAIT_KEY2: 
            changeState(STATE_WAIT_KEY1); 
            break;
        case STATE_WAIT_KEY3: 
            if (keyseq.length() == 2)
                changeState(STATE_WAIT_KEY2); 
            else if (keyseq.length() == 1)
                changeState(STATE_WAIT_KEY1);
            break;
        default: break;
    }
}

void OVIMArrayContext::changeState(STATE s)
{
    state = s;  
}

bool OVIMArrayContext::isWSeq(char a, char b) const
{ 
	return a == 'w' && isdigit(b);
}

bool OVIMArrayContext::isForceSPSeq()
{
	return keyseq.length() == 4 && (keyseq.content() == ",,sp");
}

void OVIMArrayContext::clear()
{
    keyseq.clear();
    changeState(STATE_WAIT_KEY1);
}

void OVIMArrayContext::clearAll(OVTextBuffer* buf, OVCandidateService* candi_bar)
{
    clearCandidate(candi_bar);
    buf->clear();
    buf->updateDisplay();
    keyseq.clear();
}

void OVIMArrayContext::clearCandidate(OVCandidateService *candi_bar)
{
    OVOneDimensionalCandidatePanel* panel = candi_bar->useOneDimensionalCandidatePanel();
    OVCandidateList* list = panel->candidateList();
    panel->hide();
    list->clear();
    panel->updateDisplay();
}

void OVIMArrayContext::updateDisplay(OVTextBuffer* buf)
{
    buf->clear();
    if (keyseq.length()) {
        string str = keyseq.compose();
        buf->appendText(str);
    }
    buf->updateDisplay();
}

int OVIMArrayContext::updateCandidate(OVCINDataTable *tab, OVTextBuffer *buf, OVCandidateService *candibar, OVLoaderService *loaderService)
{
    vector<string> candidates;

    if (!keyseq.hasWildcardCharacter()) {
        candidates = tab->findChardef(keyseq.content());
    }
    else {
        vector<pair<string, string> > pairs = tab->findChardefWithWildcard(OVWildcard(keyseq.content()));
        for (vector<pair<string, string> >::const_iterator i = pairs.begin(), e = pairs.end(); i != e; ++i) {
            candidates.push_back((*i).second);
        }
    }

    if (candidates.size() == 0) {
        clearCandidate(candibar);
    }
    else {
        OVOneDimensionalCandidatePanel* panel = candibar->useOneDimensionalCandidatePanel();
        OVCandidateList* list = panel->candidateList();

        string selkey = tab->findProperty("selkey");
        if (!selkey.length()) {
            selkey = "1234567890";
        }
        panel->setCandidateKeys(selkey, loaderService);
        list->setCandidates(candidates);
        panel->updateDisplay();
        panel->show();
    }

    return 1;
}


void OVIMArrayContext::sendAndReset(const char *ch, OVTextBuffer* buf, 
                                    OVCandidateService* candibar, OVLoaderService* srv)
{
    bool notifySP = false;
    
    // lookup special code
    if (parent->m_cfgAutoSP || parent->m_cfgForceSP) {
        vector<string> specialCodeVector;

        if (!strchr(ch, '*') && !strchr(ch, '?')) {
            specialCodeVector = parent->m_specialCodeTable->findChardef(ch);
        }
        else {
            vector<pair<string, string> > pairs = parent->m_specialCodeTable->findChardefWithWildcard(OVWildcard(ch));
            for (vector<pair<string, string> >::const_iterator i = pairs.begin(), e = pairs.end(); i != e; ++i) {
                specialCodeVector.push_back((*i).second);
            }
        }

        if (specialCodeVector.size() > 0) {
            size_t splen = specialCodeVector[0].length();
            const char *spcode = specialCodeVector[0].c_str();
            if (!(splen == keyseq.length() && equal(spcode, spcode+splen, keyseq.content().c_str()))) {
                char buf[16];
                string keynames;
                queryKeyName(specialCodeVector[0].c_str(), keynames);
                sprintf(buf, "%s: %s", ch, keynames.c_str());
                srv->notify(buf);
                notifySP = true;
            }
        }
    }

    bool committed = false;

    if (parent->m_cfgForceSP) {
        parent->m_cfgForceSP = !parent->m_cfgForceSP;
    }
    else {
        if (!(parent->m_cfgForceSP && notifySP)) {
            buf->clear();
            buf->appendText(ch);
            buf->commit();
            committed = true;
        }
        else {
            srv->beep();
        }
    }
    
    clearCandidate(candibar);
    if (!committed) {
        buf->clear();
        buf->updateDisplay();
    }

    keyseq.clear();
    changeState(STATE_WAIT_KEY1);
}


void OVIMArrayContext::queryKeyName(const char *keys, std::string& outKeyNames)
{
    size_t len = strlen(keys);
    for (size_t i = 0; i < len; i++) {
        string inKey(keys, i, 1);
        vector<string> keyName;
        keyName = parent->m_mainTable->findChardef(inKey);
        if (keyName.size() > 0) {
            outKeyNames.append(keyName[0]);
        }
    }
}

int OVIMArrayContext::keyEvent(OVKey* key, OVTextBuffer* buf, OVCandidateService* candi_bar, OVLoaderService* srv)
{
    int ret = 0;
    int keycode = key->keyCode();
    const bool validkey = keyseq.valid(keycode) || 
      ( keyseq.content()[0] == 'w' && isdigit(keycode) );

    if (!keyseq.length() && !isprint(keycode)) {
        return 0;
    }

    if (!keyseq.length() && key->isCombinedFunctionKey())
        return 0;
    
    if (isprint(key->keyCode()) && key->isCapsLockOn() && keyseq.length() == 0) {
        char cbuf[2];
        if (key->isShiftPressed())
            sprintf(cbuf, "%c", toupper(key->keyCode()));
        else
            sprintf(cbuf, "%c", tolower(key->keyCode()));

        buf->appendText(cbuf);
        buf->commit();
        return 1;
    }

    if (key->isShiftPressed() && isprint(keycode) && keyseq.length() == 0 && keycode != '*' && keycode != '?') {
        char k[2];
        k[0] = ::tolower(keycode);
        k[1] = 0;
        buf->appendText(k);
        buf->commit();
        return 1;
    }

    if (!keyseq.length() && !validkey) {
        char k[2];
        k[0] = ::tolower(keycode);
        k[1] = 0;
        buf->appendText(k);
        buf->commit();
        return 1;
    }

    if(keycode==OVKeyCode::Esc){
        clearAll(buf, candi_bar);
        changeState(STATE_WAIT_KEY1);
        return 1;
    }

    // cancels candidate window if wildcard character is entered
    if (!keyseq.hasWildcardCharacter() && (keycode == '?' || keycode== '*')) {
        clearCandidate(candi_bar);
        state = STATE_WAIT_KEY3;
    }

    if( state == STATE_WAIT_CANDIDATE ){
        int r = WaitCandidate(key, buf, candi_bar, srv);
        if (r != RET_CONTINUE) 
            return r;
    }

    OVOneDimensionalCandidatePanel* panel = candi_bar->useOneDimensionalCandidatePanel();
    OVCandidateList* list = panel->candidateList();

    string selkey = parent->m_mainTable->findProperty("selkey");
    if (!selkey.length()) {
        selkey = "1234567890";
    }

    if (panel->isVisible() && isdigit(keycode) &&
        !(keyseq.length() == 1 && isWSeq(keyseq.content()[0],keycode))) {
        string c;

        size_t index = 0;
        if (selkey.find(string(1, keycode)) != string::npos) {
            index = selkey.find(string(1, keycode));
            panel->setHighlightIndex(index);
            c = list->candidateAtIndex(panel->currentHightlightIndexInCandidateList());
            if (c != "?" ) {
                sendAndReset(c.c_str(), buf, candi_bar, srv);
            }
            else {
                srv->beep();
                if (state <= STATE_WAIT_KEY3 ) //dirty hack to set duty=1
                    updateCandidate(parent->m_shortCodeTable, buf, candi_bar, srv);
            }
        }
        return 1;
    }

    if (keyseq.length() && keycode == OVKeyCode::Space) {
        candidateStringVector.clear();
        if (!keyseq.hasWildcardCharacter()) {
            candidateStringVector = parent->m_mainTable->findChardef(keyseq.content());
        }
        else {
            vector<pair<string, string> > pairs = parent->m_mainTable->findChardefWithWildcard(OVWildcard(keyseq.content()));
            for (vector<pair<string, string> >::const_iterator i = pairs.begin(), e = pairs.end(); i != e; ++i) {
                candidateStringVector.push_back((*i).second);
            }
        }

        string c;
        if (candidateStringVector.size() == 1) {
            sendAndReset(candidateStringVector[0].c_str(), buf, candi_bar, srv);
        }
        else if (candidateStringVector.size() > 1) {
            updateCandidate(parent->m_mainTable, buf, candi_bar, srv);
            c = candidateStringVector[0];
            buf->clear();
            buf->appendText(c);
            buf->updateDisplay();
            changeState(STATE_WAIT_CANDIDATE);
        }
        return 1;
    }

    if (isprint(keycode) && validkey) {
        if( keyseq.length() >= 5 ||
            (keyseq.length() == 4 && keycode != 'i') )
            return 1;
        keyseq.add(keycode);
        updateDisplay(buf);
        ret = 1;
    }
    else if (key->keyCode() == OVKeyCode::Delete || key->keyCode() == OVKeyCode::Backspace) {
        keyseq.remove();
        updateDisplay(buf);
        if(keyseq.length() == 0 && panel->isVisible())
            clearCandidate(candi_bar);
        changeBackState(state);
        ret = 1;
    }
    ret = dispatchStateHandler(key, buf, candi_bar, srv);
    return ret;
}

int OVIMArrayContext::dispatchStateHandler(OVKey* key, OVTextBuffer* buf, 
                                            OVCandidateService* candi_bar, OVLoaderService* srv)
{
    switch(state){
        case STATE_WAIT_KEY1:
            return WaitKey1(key, buf, candi_bar, srv);
            break;
        case STATE_WAIT_KEY2:
            return WaitKey2(key, buf, candi_bar, srv);
            break;
        case STATE_WAIT_KEY3:
            return WaitKey3(key, buf, candi_bar, srv);
            break;
        default:
            break;
    }

    return 0;
}

int OVIMArrayContext::WaitKey1(OVKey* key, OVTextBuffer* buf, 
                               OVCandidateService* candibar, OVLoaderService* srv)
{
    if (keyseq.length() != 1)
        return 0;

    if (!keyseq.hasWildcardCharacter())
        updateCandidate(parent->m_shortCodeTable, buf, candibar, srv);

    char keycode = keyseq.content()[0];

    if (keycode == 't') {
        buf->clear();
        buf->appendText("\xe7\x9a\x84");
        buf->updateDisplay();
    }

    if (isprint(keycode) && keyseq.valid(keycode))
        changeState(STATE_WAIT_KEY2);

    return 1;
}

int OVIMArrayContext::WaitKey2(OVKey* key, OVTextBuffer* buf, 
                               OVCandidateService* candibar, OVLoaderService* srv)
{
    if (keyseq.length() != 2)
        return 0;

    char keycode = keyseq.content()[1];

    if (isWSeq(keyseq.content()[0], keyseq.content()[1])) {
        updateCandidate(parent->m_mainTable, buf, candibar, srv);
        buf->clear();
        buf->appendText(candidateStringVector[0]);
        buf->updateDisplay();
        changeState(STATE_WAIT_CANDIDATE);
    }
    else {
        if (!keyseq.hasWildcardCharacter())
            updateCandidate(parent->m_shortCodeTable, buf, candibar, srv);

        if (isprint(keycode) && keyseq.valid(keycode))
            changeState(STATE_WAIT_KEY3);
    }
    return 1;    
}

int OVIMArrayContext::WaitKey3(OVKey* key, OVTextBuffer* buf, 
                               OVCandidateService* candibar, OVLoaderService* srv)
{
    if (keyseq.length() >= 3) {
        if (!keyseq.hasWildcardCharacter()) {
            updateCandidate(parent->m_mainTable, buf, candibar, srv);
        }

    }
    updateDisplay(buf);
    return 1;    
}


int OVIMArrayContext::WaitCandidate(OVKey* key, OVTextBuffer* buf, 
                                    OVCandidateService* candibar, OVLoaderService* srv)
{
    unsigned int keycode = key->keyCode();

    if (keycode == OVKeyCode::Esc || keycode == OVKeyCode::Backspace) {
        clearAll(buf, candibar);
        changeState(STATE_WAIT_KEY1);
        return RET_DONE;
    }

    OVOneDimensionalCandidatePanel* panel = candibar->useOneDimensionalCandidatePanel();
    OVCandidateList* list = panel->candidateList();


    if (keycode == OVKeyCode::Down || keycode == OVKeyCode::Right ||
        (panel->pageCount() > 1 && keycode == OVKeyCode::Space)) {
        panel->goToNextPage();
        panel->updateDisplay();
        updateDisplay(buf);
        return RET_DONE;
    }

    if (keycode == OVKeyCode::Up || keycode == OVKeyCode::Left) {
        panel->goToPreviousPage();
        panel->updateDisplay();
        updateDisplay(buf);
        return RET_DONE;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    bool notSelkey = parent->m_mainTable->findProperty("selkey").find(keycode) == string::npos;
    bool defaultSelKey = (keycode == OVKeyCode::Return || (panel->pageCount() == 1 && keycode == OVKeyCode::Space));
    if (defaultSelKey || notSelkey) {
        keycode = parent->m_mainTable->findProperty("selkey")[0];
    }

    size_t index = parent->m_mainTable->findProperty("selkey").find(keycode);
    if (index != string::npos) {
        panel->setHighlightIndex(index);
        size_t candidateIndex = panel->currentHightlightIndexInCandidateList();
        string output = list->candidateAtIndex(candidateIndex);
        sendAndReset(output.c_str(), buf, candibar, srv);
        if (notSelkey && !defaultSelKey) {
            updateDisplay(buf);
            return RET_CONTINUE;
        }
        return RET_DONE;
    }
    
    updateDisplay(buf);
    srv->beep();
    return RET_PASS;
}

