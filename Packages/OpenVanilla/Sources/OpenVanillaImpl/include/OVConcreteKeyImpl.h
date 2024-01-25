//
// OVConcreteKeyImpl.h
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

#import "OVKey.h"

namespace OpenVanilla {
    class OVConcreteKeyImpl : public OVKeyImpl {
    public:
        OVConcreteKeyImpl();
        OVConcreteKeyImpl(int characterCode, bool alt, bool opt, bool ctrl, bool shift, bool command, bool capsLock, bool numLock);
        OVConcreteKeyImpl(const std::string& receivedString, bool alt, bool opt, bool ctrl, bool shift, bool command, bool capsLock, bool numLock);

        virtual const std::string receivedString() const;
        virtual unsigned int keyCode() const;
        virtual bool isAltPressed() const;
        virtual bool isOptPressed() const;
        virtual bool isCtrlPressed() const;
        virtual bool isShiftPressed() const;
        virtual bool isCommandPressed() const;
        virtual bool isNumLockOn() const;
        virtual bool isCapsLockOn() const;

        // a direct text key carries a composed glyph (or a std::string) that semantically differs from the intended keystroke
        // (i.e. a half-width char stroke but with a composed, full-width char output)
        virtual bool isDirectTextKey() const;

        virtual bool shouldDelete() const;
        virtual OVKeyImpl* copy();

    protected:
        int m_characterCode;
        std::string m_receivedString;
        bool m_opt;
        bool m_ctrl;
        bool m_shift;
        bool m_command;
        bool m_numLock;
        bool m_capsLock;
    };
};
