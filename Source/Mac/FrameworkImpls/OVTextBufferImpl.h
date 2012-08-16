//
// OVTextBufferImpl.h
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

#ifndef OVTextBufferImpl_h
#define OVTextBufferImpl_h

#include "OpenVanilla.h"

namespace OpenVanilla {
    using namespace std;

    class OVTextBufferImpl : public OVTextBuffer {
    public:
        OVTextBufferImpl();
        virtual void clear();
        virtual void setText(const string& text);
        virtual void appendText(const string& text, bool moveCursor = true);
        virtual void commit();
        virtual void commitAsTextSegment();        
        virtual void commit(const string& text);
        virtual void commitAsTextSegment(const string& text);		
        virtual void updateDisplay();
        virtual bool isEmpty() const;
        virtual size_t codePointCount() const;
        virtual const string codePointAt(size_t index) const;
        virtual const string composedText() const;
        virtual const string composedCommittedText() const;
        virtual const vector<string> composedCommittedTextSegments() const;
        virtual void setCursorPosition(size_t position);
        virtual size_t cursorPosition() const;
        virtual void showToolTip(const string& text);
        virtual void clearToolTip();
        virtual void setHighlightMark(const OVTextBuffer::RangePair& range);
        virtual void setWordSegments(const vector<OVTextBuffer::RangePair>& segments);
        virtual void setSuggestedReadingTextStyle(ReadingTextStyle style);
        virtual ReadingTextStyle defaultReadingTextStyle() const;

    public:
        virtual bool shouldUpdate();
        virtual bool isCommitted();
        virtual void finishUpdate();
        virtual void finishCommit();
        virtual const OVTextBuffer::RangePair& highlightMark() const;
        virtual const vector<OVTextBuffer::RangePair>& wordSegments() const;
        virtual const pair<string, string> composedTextsSeparatedByCursor() const;
        virtual const vector<string>& codePoints() const;
        virtual void setComposedCommittedText(const string& text);
		virtual const string toolTipText() const;

    protected:
        virtual RangePair clampedRangePair(const RangePair& range);

        bool m_committed;        
        bool m_needsUpdate;
        OVTextBuffer::ReadingTextStyle m_readingTextStyle;

        size_t m_cursorIndex;
        vector<string> m_codePoints;
        OVTextBuffer::RangePair m_highlightRange;
        string m_composedCommittedText;
        vector<OVTextBuffer::RangePair> m_textSegments;
        vector<string> m_composedCommittedTextSegments;
    };
};

#endif