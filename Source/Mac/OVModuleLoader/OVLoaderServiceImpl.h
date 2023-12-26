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

#ifndef OVLoaderServiceImpl_h
#define OVLoaderServiceImpl_h

#include "OVLoaderService.h"

namespace OpenVanilla {
    class OVLoaderServiceImpl : public OVLoaderService {
    public:
        virtual void beep();
        virtual void notify(const string& message) {}
        virtual void HTMLNotify(const string& content) {}

        virtual const string locale() const;
        virtual const OVKey makeOVKey(int characterCode, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false);
        virtual const OVKey makeOVKey(const string& receivedString, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false);

        virtual ostream& logger(const string& sectionName = "");

        virtual OVDatabaseService* defaultDatabaseService() { return 0; }
        virtual OVDatabaseService* CINDatabaseService() { return 0; }
        virtual OVDatabaseService* SQLiteDatabaseService() { return 0; }

        virtual OVEncodingService* encodingService() { return 0; }

    	virtual void __reserved1(const string&) {}
        virtual void __reserved2(const string&) {}
        virtual void __reserved3(const string&) {}
    	virtual void __reserved4(const string&) {}
        virtual const string __reserved5() const { return ""; }
    	virtual void __reserved6(const string&) {}
    	virtual void __reserved7(const string&, const string &) {}
        virtual void* __reserved8(const string&) { return 0; }

        void setCurrentLocale(string locale);
    };
};

#endif
