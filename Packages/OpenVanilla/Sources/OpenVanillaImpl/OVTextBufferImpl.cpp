//
// OVTextBufferImpl.cpp
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

#include "OVTextBufferImpl.h"

using namespace OpenVanilla;

OVTextBufferImpl::OVTextBufferImpl()
    : m_committed(false)
    , m_needsUpdate(false)
    , m_readingTextStyle(Horizontal)
    , m_cursorIndex(0)
    , m_highlightRange(OVTextBuffer::RangePair(0, 0))
{
}

void OVTextBufferImpl::clear()
{
    m_committed = false;
    m_needsUpdate = false;
    m_cursorIndex = 0;
    m_highlightRange = OVTextBuffer::RangePair(0, 0);
    m_codePoints.clear();
    m_textSegments.clear();
    m_readingTextStyle = Horizontal;
    clearToolTip();
}

void OVTextBufferImpl::setText(const string& text)
{        
    if (text != composedText()) {
        m_codePoints = OVUTF8Helper::SplitStringByCodePoint(text);
        m_cursorIndex = m_codePoints.size();
        m_needsUpdate = true;
    }
}

void OVTextBufferImpl::appendText(const string& text, bool moveCursor)
{
    if (text.length()) {
        vector<string> insertedCodePoints = OVUTF8Helper::SplitStringByCodePoint(text);
        m_codePoints.insert(m_codePoints.end(), insertedCodePoints.begin(), insertedCodePoints.end());
        
        if (moveCursor) {
            m_cursorIndex = m_codePoints.size();
        }
        
        m_needsUpdate = true;
    }
}

void OVTextBufferImpl::commit()
{
    string composedTextString = composedText();
    m_composedCommittedText += composedTextString;
    
    if (m_composedCommittedTextSegments.size()) {
        m_composedCommittedTextSegments.back() += composedTextString;
    }
    else {
        m_composedCommittedTextSegments.push_back(composedTextString);
    }
    
    clear();
    m_committed = true;
    m_needsUpdate = true;
}

void OVTextBufferImpl::commitAsTextSegment()
{
    string composedTextString = composedText();
    m_composedCommittedText += composedTextString;
    m_composedCommittedTextSegments.push_back(composedTextString);        
    clear();
    m_committed = true;
    m_needsUpdate = true;
}

void OVTextBufferImpl::commit(const string& text)
{
    m_composedCommittedText += text;
    
    if (m_composedCommittedTextSegments.size()) {
        m_composedCommittedTextSegments.back() += text;
    }
    else {
        m_composedCommittedTextSegments.push_back(text);
    }
    
    m_needsUpdate = true;
    m_committed = true;                        
}

void OVTextBufferImpl::commitAsTextSegment(const string& text)
{
    m_composedCommittedText += text;
    m_composedCommittedTextSegments.push_back(text);        
    m_committed = true;    
    m_needsUpdate = true;
}

void OVTextBufferImpl::updateDisplay()
{
    m_needsUpdate = true;
}

bool OVTextBufferImpl::isEmpty() const
{
    return m_codePoints.size() == 0;
}

size_t OVTextBufferImpl::codePointCount() const
{
    return m_codePoints.size();
}

const string OVTextBufferImpl::codePointAt(size_t index) const
{
    return (index >= m_codePoints.size()) ? string() : m_codePoints[index];
}

const string OVTextBufferImpl::composedText() const
{
    return OVUTF8Helper::CombineCodePoints(m_codePoints);
}

const string OVTextBufferImpl::composedCommittedText() const
{
    return m_composedCommittedText;
}

const vector<string> OVTextBufferImpl::composedCommittedTextSegments() const
{
    return m_composedCommittedTextSegments;
}

void OVTextBufferImpl::setCursorPosition(size_t position)
{
    if (position <= m_codePoints.size()) {
        m_cursorIndex = position;
        m_needsUpdate = true;
    }
}

size_t OVTextBufferImpl::cursorPosition() const
{
    return m_cursorIndex;
}

void OVTextBufferImpl::showToolTip(const string& text)
{
    m_toolTipText = text;
}

void OVTextBufferImpl::clearToolTip()
{
    m_toolTipText = "";
}

void OVTextBufferImpl::setHighlightMark(const OVTextBuffer::RangePair& range)
{
    OVTextBuffer::RangePair clamped = clampedRangePair(range);
    if (clamped != m_highlightRange) {
        m_highlightRange = range;
        m_needsUpdate = true;
    }
}

void OVTextBufferImpl::setWordSegments(const vector<OVTextBuffer::RangePair>& segments)
{
    vector<OVTextBuffer::RangePair> result;
    for (vector<OVTextBuffer::RangePair>::const_iterator i = segments.begin(), e = segments.end() ; i != e; ++i) {
        result.push_back(clampedRangePair(*i));
    }
        
    if (result != m_textSegments) {
        m_textSegments = result;
        m_needsUpdate = true;
    }
}

void OVTextBufferImpl::setSuggestedReadingTextStyle(OVTextBuffer::ReadingTextStyle style)
{
    m_readingTextStyle = style;
}

OVTextBuffer::ReadingTextStyle OVTextBufferImpl::defaultReadingTextStyle() const
{
    return m_readingTextStyle;
}

bool OVTextBufferImpl::shouldUpdate()
{
    return m_needsUpdate;
}

bool OVTextBufferImpl::isCommitted()
{
    return m_committed;
}

void OVTextBufferImpl::finishUpdate()
{
    m_needsUpdate = false;
}

void OVTextBufferImpl::finishCommit()
{
    m_committed = false;
    m_composedCommittedText = "";
    m_composedCommittedTextSegments.clear();
}

const OVTextBuffer::RangePair& OVTextBufferImpl::highlightMark() const
{
    return m_highlightRange;
}

const vector<OVTextBuffer::RangePair>& OVTextBufferImpl::wordSegments() const
{
    return m_textSegments;
}

const pair<string, string> OVTextBufferImpl::composedTextsSeparatedByCursor() const
{
    vector<string> front;
    vector<string> back;        
    front.insert(front.begin(), m_codePoints.begin(), m_codePoints.begin() + m_cursorIndex);
    back.insert(back.begin(), m_codePoints.begin() + m_cursorIndex, m_codePoints.end());
    return pair<string, string>(OVUTF8Helper::CombineCodePoints(front), OVUTF8Helper::CombineCodePoints(back));
}

const vector<string>& OVTextBufferImpl::codePoints() const
{
    return m_codePoints;
}

void OVTextBufferImpl::setComposedCommittedText(const string& text)
{
    m_composedCommittedText = text;
    m_composedCommittedTextSegments.clear();
    m_composedCommittedTextSegments.push_back(text);
    m_committed = true;
}

const string OVTextBufferImpl::toolTipText() const
{
    return m_toolTipText;
}

OVTextBuffer::RangePair OVTextBufferImpl::clampedRangePair(const OVTextBuffer::RangePair& range)
{
    OVTextBuffer::RangePair result = range;
    size_t size = m_codePoints.size();
    if (result.first > size) {
        result.first = size;
    }
        
    if (result.first + result.second > size) {
        result.second = size - result.first;
    }

    return result;
}

