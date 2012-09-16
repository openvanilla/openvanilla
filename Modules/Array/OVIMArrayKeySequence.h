// OVIMArrayKeySequence.h

#ifndef OVIMARRAYKEYSEQUENCE_H
#define OVIMARRAYKEYSEQUENCE_H

#include "OpenVanilla.h"

namespace OpenVanilla {
    class OVIMArrayKeySequence {
    protected:
        string m_sequence;
        OVCINDataTable *m_table;

    public:
        OVIMArrayKeySequence(OVCINDataTable* table) : m_table(table) {}

        bool add(char c)
        {
            m_sequence += string(1, c);
            return true;
        }
        
        void remove()
        {
            if (m_sequence.length()) {
                m_sequence = m_sequence.substr(0, m_sequence.length() - 1);
            }
        }

        void clear()
        {
            m_sequence.clear();
        }

        size_t length() const
        {
            return m_sequence.length();
        }

        string content() const
        {
            return m_sequence;
        }
        
        bool valid(char c)
        {
            string keyName = m_table->findKeyname(string(1, c));
            return keyName.length() != 0;
        }

        string compose() const
        {
            string result;
            for (string::const_iterator i = m_sequence.begin(), e = m_sequence.end(); i != e; ++i) {
                string keyName = m_table->findKeyname(string(1, *i));
                result += keyName;
            }

            return result;
        }

        bool hasWildcardCharacter() const
    	{
            return m_sequence.find("?") != string::npos || m_sequence.find("*") != string::npos;
    	}
    };
};

#endif
