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
