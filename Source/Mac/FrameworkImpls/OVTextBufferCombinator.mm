//
// OVTextBufferCombinator.mm
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

#import "OVTextBufferCombinator.h"

using namespace OpenVanilla;

OVTextBufferCombinator::OVTextBufferCombinator(const OVTextBufferImpl* composingText, const OVTextBufferImpl* readingText)
    : m_cursorIndex(0)
    , m_attrText(nil)
{            
    const vector<string>& composingCodePoints = composingText->codePoints();
    const OVTextBuffer::RangePair& highlight = composingText->highlightMark();

    const vector<string>& readingCodePoints = readingText->codePoints();
    NSMutableString *text = [NSMutableString string];

    size_t accuLength = 0;
    bool hasHighlight = (highlight.second > 0);
    bool highlightStarted = false;
    NSUInteger highlightStart = 0;
    size_t highlightAccumulator = 0;
    NSUInteger highlightLength = 0;

    for (size_t ci = 0, ce = composingText->cursorPosition(); ci != ce; ++ci) {
        NSString *segment = [NSString stringWithUTF8String:composingCodePoints[ci].c_str()];
        [text appendString:segment];

        if (hasHighlight && (ci == highlight.first)) {
            highlightStart = accuLength;
            highlightStarted = true;
        }

        NSUInteger segLen = [segment length];
        m_cursorIndex += segLen;
        accuLength += (size_t)segLen;

        if (highlightStarted) {
            if (highlightAccumulator < highlight.second) {
                highlightLength += (size_t)segLen;                
            }
            highlightAccumulator++;
        }
    }

    for (size_t ri = 0, re = readingCodePoints.size(); ri != re; ++ri) {
        NSString *segment = [NSString stringWithUTF8String:readingCodePoints[ri].c_str()];
        [text appendString:segment];
        NSUInteger segLen = [segment length];
        accuLength += (size_t)segLen;

        if (ri < readingText->cursorPosition()) {
            m_cursorIndex += segLen;
        }

        if (highlightStarted) {
            highlightLength += (size_t)segLen;
        }
    }

    for (size_t ci = composingText->cursorPosition(), ce = composingCodePoints.size(); ci != ce; ++ci) {
        NSString *segment = [NSString stringWithUTF8String:composingCodePoints[ci].c_str()];
        [text appendString:segment];

        if (hasHighlight && (ci == highlight.first)) {
            highlightStart = accuLength;
            highlightStarted = true;
        }

        NSUInteger segLen = [segment length];
        accuLength += segLen;
        if (highlightStarted) {
            if (highlightAccumulator < highlight.second) {
                highlightLength += (size_t)segLen;                
            }
            highlightAccumulator++;
        }        
    }

    m_attrText = [[NSMutableAttributedString alloc] initWithString:text];

    // segment 1
    NSInteger section = 0;
    if (highlightStart) {
        NSRange markerRange = NSMakeRange(0, highlightStart);
        NSDictionary *attrDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInteger:NSUnderlineStyleSingle], NSUnderlineStyleAttributeName,
                                  [NSNumber numberWithInteger:section], NSMarkedClauseSegmentAttributeName, nil];
        [m_attrText setAttributes:attrDict range:markerRange];
        section++;
    }

    // segment 2
    if (highlightLength) {
        NSRange markerRange = NSMakeRange(highlightStart, highlightLength);
        NSDictionary *attrDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInteger:NSUnderlineStyleThick], NSUnderlineStyleAttributeName,
                                  [NSNumber numberWithInteger:section], NSMarkedClauseSegmentAttributeName, nil];
        [m_attrText setAttributes:attrDict range:markerRange];
        section++;
    }

    // segment 3
    NSUInteger endSegmentStart = highlightStart + highlightLength;
    NSUInteger endSegmentLength = accuLength - endSegmentStart;
    if (endSegmentLength) {
        NSRange markerRange = NSMakeRange(endSegmentStart, endSegmentLength);
        NSDictionary *attrDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInteger:NSUnderlineStyleSingle], NSUnderlineStyleAttributeName,
                                  [NSNumber numberWithInteger:section], NSMarkedClauseSegmentAttributeName, nil];
        [m_attrText setAttributes:attrDict range:markerRange];
    }
}

OVTextBufferCombinator::~OVTextBufferCombinator()
{
}

NSAttributedString *OVTextBufferCombinator::combinedAttributedString()
{
    return m_attrText;
}

NSRange OVTextBufferCombinator::selectionRange()
{
    return NSMakeRange(m_cursorIndex, 0);
}
