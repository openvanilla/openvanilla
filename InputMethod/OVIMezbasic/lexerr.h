
// lexerr.h: the error processor
//           modify the database for your demand

#ifndef __lexerr_h
    #define __lexerr_h

#ifndef __platform_h
    #include "platform.h"
#endif

#include <iostream>
using namespace std;

typedef uint   LEType;

const LEType    LEnull = 0,                 // not error
                LEsyntax = 1,               // syntax error (generic)
                LEfatal = 2,                // fatal error
                LEmem = 3,                  // out of memory
                LEunrecognizable = 100,     // unrecognizable char
                LEnotimplemented = 101,     // not exist or not implemented
                LEassignment = 102,         // assignment error
                LEend = 65535               // the last error
                ;

class LexException {
    public:
        LexException (LEType tt, size_t l) : t(tt), line(l) {}

        LEType t;
        size_t line;
    };


cchar* LexErrMsg (LEType);

inline void LexError (LEType t=LEfatal, size_t l=0, cchar* extmsg=NULL,
                      BOOL toabort=FALSE)
{
    cerr << flush << "Error (line " << l << "): " << LexErrMsg(t);

    if (extmsg) cerr << ", " << extmsg << endl;
    else cerr << "." << endl;

    #ifdef PF_EXCEPTION
        throw(LexException(t, l));
    #else
        if (toabort) abort();
    #endif
}

inline cchar* LexErrMsg (LEType t)
{
    switch (t)
    {
        case LEsyntax:
            return "syntax error";

        case LEfatal:
            return "fatal error";

        case LEmem:
            return "out of memory";

        case LEunrecognizable:
            return "unrecognizable character encountered";

        case LEnotimplemented:
            return "call to a function doesn't exist or isn't implemented";

        case LEassignment:
            return "assignment syntax error";

        default:
            return "nothing happens";
    }
}

#endif

