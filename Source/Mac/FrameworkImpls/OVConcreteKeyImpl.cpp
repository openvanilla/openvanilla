#include "OVConcreteKeyImpl.h"

using namespace OpenVanilla;

#import "OVKey.h"

OVConcreteKeyImpl::OVConcreteKeyImpl()
  : m_characterCode(0)
  , m_opt(false)
  , m_ctrl(false)
  , m_shift(false)
  , m_command(false)
  , m_numLock(false)
  , m_capsLock(false)
{
}

OVConcreteKeyImpl::OVConcreteKeyImpl(int characterCode, bool alt, bool opt, bool ctrl, bool shift, bool command, bool capsLock, bool numLock)
  : m_characterCode(characterCode)
  , m_opt(alt || opt)
  , m_ctrl(ctrl)
  , m_shift(shift)
  , m_command(command)
  , m_numLock(numLock)
  , m_capsLock(capsLock)
{
}

OVConcreteKeyImpl::OVConcreteKeyImpl(const std::string& receivedString, bool alt, bool opt, bool ctrl, bool shift, bool command, bool capsLock, bool numLock)
  : m_receivedString(receivedString)
  , m_opt(alt || opt)
  , m_ctrl(ctrl)
  , m_shift(shift)
  , m_command(command)
  , m_numLock(numLock)
  , m_capsLock(capsLock)
{
    uint8_t c = 0;
    if (receivedString.size()) {
        c = (uint8_t)receivedString[0];
        if (c >= 0x80) {
            c = 0;
        }
    }
    
    m_characterCode = c;
}

const std::string OVConcreteKeyImpl::receivedString() const
{
    return m_receivedString;
}

unsigned int OVConcreteKeyImpl::keyCode() const
{
    return m_characterCode;
}

bool OVConcreteKeyImpl::isAltPressed() const
{
    return m_opt;
}

bool OVConcreteKeyImpl::isOptPressed() const
{
    return m_opt;
}

bool OVConcreteKeyImpl::isCtrlPressed() const
{
    return m_ctrl;
}

bool OVConcreteKeyImpl::isShiftPressed() const
{
    return m_shift;
}

bool OVConcreteKeyImpl::isCommandPressed() const
{
    return m_command;
}

bool OVConcreteKeyImpl::isNumLockOn() const
{
    return m_numLock;
}

bool OVConcreteKeyImpl::isCapsLockOn() const
{
    return m_capsLock;
}

bool OVConcreteKeyImpl::isDirectTextKey() const
{
    return false;
}

bool OVConcreteKeyImpl::shouldDelete() const
{
    return true;
}

OVKeyImpl* OVConcreteKeyImpl::copy()
{
    OVConcreteKeyImpl* newInstance = new OVConcreteKeyImpl;
    *newInstance = *this;
    return newInstance;
}
