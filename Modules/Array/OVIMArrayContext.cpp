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
#include "OVIMArrayKeySequence.h"

using namespace OpenVanilla;

OVIMArrayContext::OVIMArrayContext(OVIMArray* module)
	: m_module(module)
{
}

void OVIMArrayContext::startSession(OVLoaderService* loaderService)
{
}

bool OVIMArrayContext::handleKey(OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVCandidateService* candidateService, OVLoaderService* loaderService)
{
	return false;
}

void OVIMArrayContext::candidateCanceled(OVCandidateService* candidateService, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
}

bool OVIMArrayContext::candidateSelected(OVCandidateService* candidateService, const string& text, size_t index, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
	return false;
}

bool OVIMArrayContext::candidateNonPanelKeyReceived(OVCandidateService* candidateService, const OVKey* key, OVTextBuffer* readingText, OVTextBuffer* composingText, OVLoaderService* loaderService)
{
	return false;
}
