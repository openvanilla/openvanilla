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

using namespace OpenVanilla;

static const char *const kSelectionKeys = "!@#$%^&*()";
static const char *const kSelectionKeyLabels = "1234567890";
static const char *const kSelectionKeyLabelPrefix = "⇧";

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

    if (m_candidates.empty()) {
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

    for (keyIndex = 0; keyIndex < selKeyLength; keyIndex++) {
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

    m_candidates.clear();
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
		return false;
	}

    if (!(readingText->isEmpty() && composingText->isEmpty())) {
        return false;
    }
    
    vector<string> codepoints = OVUTF8Helper::SplitStringByCodePoint(text);
    if (codepoints.size() != 1) {
        return false;
    }

    composingText->setText(text);
    composingText->commit();

    m_candidates = m_module->m_table->findChardef(text);
    if (m_candidates.size()) {
        OVOneDimensionalCandidatePanel* panel = candidateService->useOneDimensionalCandidatePanel();
        OVCandidateList* list = panel->candidateList();
        list->setCandidates(m_candidates);

        vector<pair<OVKey, string>> keyLabelPairs;
        for (size_t i = 0, len = strlen(kSelectionKeys); i < len; i++) {
            keyLabelPairs.push_back(make_pair(loaderService->makeOVKey(kSelectionKeys[i]), string(kSelectionKeyLabelPrefix) + string(1, kSelectionKeyLabels[i])));
        }
        panel->setCandidateKeysAndLabels(keyLabelPairs);
        panel->setCandidatesPerPage(10);
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
