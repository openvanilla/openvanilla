//
// OVCandidateServiceImpl.mm
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

#import "OVCandidateServiceImpl.h"

using namespace OpenVanilla;

@import CandidateUI;

OVCandidateServiceImpl::OVCandidateServiceImpl(OVLoaderService* loaderService)
    : m_horizontalCandidatePanel([VTHorizontalCandidateController class], loaderService)
    , m_verticalCandidatePanel([VTVerticalCandidateController class], loaderService)
{    
}

OVOneDimensionalCandidatePanel* OVCandidateServiceImpl::useOneDimensionalCandidatePanel()
{
    if (!m_currentCandidatePanel) {
        m_currentCandidatePanel = &m_verticalCandidatePanel;
    }

    return m_currentCandidatePanel;
}

OVOneDimensionalCandidatePanel* OVCandidateServiceImpl::useHorizontalCandidatePanel()
{
    m_currentCandidatePanel = &m_horizontalCandidatePanel;
    return m_currentCandidatePanel;
}

OVOneDimensionalCandidatePanel* OVCandidateServiceImpl::useVerticalCandidatePanel()
{
    m_currentCandidatePanel = &m_verticalCandidatePanel;
    return m_currentCandidatePanel;
}

OVOneDimensionalCandidatePanelImpl* OVCandidateServiceImpl::currentCandidatePanel()
{
    return m_currentCandidatePanel;
}

void OVCandidateServiceImpl::resetAll()
{
    m_horizontalCandidatePanel.reset();
    m_verticalCandidatePanel.reset();
}

void OVCandidateServiceImpl::applyFontSettings(NSString *fontName, NSUInteger fontSize)
{
    m_horizontalCandidatePanel.applyFontSettings(fontName, fontSize);
    m_verticalCandidatePanel.applyFontSettings(fontName, fontSize);
}

