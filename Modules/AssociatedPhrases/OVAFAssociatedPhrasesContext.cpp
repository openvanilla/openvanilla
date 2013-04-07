//
// OVAFAssociatedPhrasesContext.cpp
//
// Copyright (c) 2004-2013 Lukhnos Liu (lukhnos at openvanilla dot org)
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


#include "OVAFAssociatedPhrasesContext.h"

static const char *const kSelectionKeys = "!@#$%^&*()";

using namespace OpenVanilla;

OVAFAssociatedPhrasesContext::OVAFAssociatedPhrasesContext(OVAFAssociatedPhrases* module)
	: m_module(module)
{
}

void OVAFAssociatedPhrasesContext::startSession(OVLoaderService* loaderService)
{
	m_module->checkTable();
}

bool OVAFAssociatedPhrasesContext::handleKey(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
	if (!m_module->m_table) {
		return false;
	}

     OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();

    if (key->keyCode() == OVKeyCode::PageUp) {
        panel->goToPreviousPage();
        panel->updateDisplay();
        return true;
    }
    
    if (key->keyCode() == OVKeyCode::PageDown || key->keyCode() == OVKeyCode::Space) {
        panel->goToNextPage();
        panel->updateDisplay();
        return true;
    }
		  
    bool keyHandled = false;
    size_t keyIndex;
    size_t selKeyLength = strlen(kSelectionKeys);

    for (keyIndex = 0; keyIndex < selKeyLength; key++) {
        if (key->keyCode() == kSelectionKeys[keyIndex]) break;
    }

    if (keyIndex < selKeyLength && key->isShiftPressed()) {
        if (keyIndex >= panel->currentPageCandidateCount()) {
            loaderService->beep();
        }
        else {
            panel->setHighlightIndex(keyIndex);
            size_t candidateIndex = panel->currentHightlightIndexInCandidateList();
            string candidate = panel->candidateList()->candidateAtIndex(candidateIndex);
            composingText->setText(candidate);
            composingText->commit();
        }
        
        keyHandled = true;
    }

    panel->hide();
    panel->reset();
    panel->updateDisplay();

    // Absorb the Return key
    if (key->keyCode() == OVKeyCode::Return) {
        keyHandled = true;
    }

    // Absorb the Esc key
	if (key->keyCode() == OVKeyCode::Esc) {
        keyHandled = true;
    }

    return keyHandled;
}

bool OVAFAssociatedPhrasesContext::handleDirectText(const string& text, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
	if (!m_module->m_table) {
        cerr << "1\n";
		return false;
	}

    if (!(readingText->isEmpty() && composingText->isEmpty())) {
        cerr << "2\n";
        return false;
    }
    
    vector<string> codepoints = OVUTF8Helper::SplitStringByCodePoint(text);
    if (codepoints.size() != 1) {
        cerr << "3\n";
        return false;
    }

    composingText->setText(text);
    composingText->commit();

    vector<string> candidates = m_module->m_table->findChardef(text);
    cerr << "text: " << text << ", c: " << candidates.size() << endl;

    if (candidates.size()) {
        OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
        OVCandidateList* list = panel->candidateList();
        list->setCandidates(candidates);
        panel->setCandidateKeys(kSelectionKeys, loaderService);
        panel->setCandidatesPerPage(9);
        panel->updateDisplay();
        panel->show();
        return true;
    }
        
    return false;
}

bool OVAFAssociatedPhrasesContext::candidateSelected(OVCandidateService* candidateService, const string& text, size_t index, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
	if (!m_module->m_table) {
		return false;
	}

    composingText->setText(text);
    composingText->commit();
    return true;
}
