
// basiclex.h: the BASICLex class, that implements the lexical analyzer
//             we need to parse ANSI BASIC

#ifndef __basiclex_h
    #define __basiclex_h

#ifndef __lex_h
    #include "lex.h"
#endif

// the token family: see "basictkn.h," for it's tooooo long...

    #include "basictkn.h"

LCType BL_Lookup(cchar);
Token BL_KeywordLookup(cchar*);
Token BL_DelimiterLookup(cchar);

class BASICLex : public Lex {
    public:
        BASICLex (LexInput*, LCFunc f = &BL_Lookup, size_t s = L_DefBufSize);
        Token   ahead();

    protected:
        LCType  clearrem();
    };

inline BASICLex::BASICLex (LexInput* i, LCFunc f, size_t s)
                :Lex (i, f, s)
{
}

inline LCType BASICLex::clearrem()
{
    while (1)
    {
        cchar c = input->peek();
        if (!c) return (LCend);
        if (c == '\n') return (LCendl);
        input->next();
    }

    return (LCnull);
}

// a little different with its parent's
inline Token BASICLex::ahead()
{
    if (lasttoken != TKnull) return (lasttoken);

    // skip white space and judge if end encountered
    LCType nextlct;
    while ((nextlct = (*lexilook)(input->peek())) == LCnull) input->next();

    if (input->peek() == '\'') nextlct = clearrem();    // clear '\'' (REM)

    if (nextlct == LCend)
    {
        input->next();
        return (lasttoken = TKend);
    }

    if (nextlct == LCendl)
    {
        input->next();
        return (lasttoken = TKends);
    }

    cchar* bufp   = buf;
    cchar* bufend = buf + bufsize - 1;      // don't forget '\0' !

    switch (nextlct)
    {
        case LCalpha:
            lasttoken = BTliteral;
            while (bufp < bufend)
            {
                LCType t = (*lexilook)(input->peek());
                if (t == LCalpha || t == LCdigit)
                    *bufp++ = input->next();
                else
                    break;
            }

//          switch (input->peek())
//          {
//              case ':': input->next(); return (BTlabel);
//          }


            *bufp = 0;
            lasttoken = BL_KeywordLookup(buf, lasttoken);
            break;

        case LCdigit:
            lasttoken = BTint;
            while (bufp < bufend)
                if ((*lexilook)(input->peek()) == LCdigit)
                    *bufp++ = input->next();
                else
                    break;
            break;

        case LCdilim:
            *bufp++ = input->next();
            lasttoken = BL_DelimiterLookup (*buf);
            if (lasttoken == TKnull) LexError (LEunrecognizable, line());
            break;

        case LCquote:
            lasttoken = BTstring;
            bufp = nextstring(bufp, bufend);
            break;
    }

    *bufp = 0;
    return (lasttoken);
}


// not to mention the lookup table...
// ------------------------------------------------------------------------

inline LCType BL_Lookup(cchar c)
{
    static LCType table[128] = {
    // 0~   NUL,    SOH,    STX,    ETX,    EOT,    ENQ,    ACK,    BEL,
            LCend,  LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull,
    // 8~   BS,     HT,     LF,     VT,     FF,     CR,     SO,     SI,
            LCnull, LCnull, LCendl, LCnull, LCnull, LCnull, LCnull, LCnull,
    // 16~  DLE,    DC1,    DC2,    DC3,    DC4,    NAK,    SYN,    ETB,
            LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull,
    // 24~  CAN,    EM,     SUB,    ESC,    FS,     GS,     RS,     US,
            LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull,
    // 32~  SP,     '!',    '"',    '#',    '$',    '%',    '&',    ''',
            LCnull, LCdilim,LCquote,LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,
    // 40~  '(',    ')',    '*',    '+',    ',',    '-',    '.',    '/',
            LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,
    // 48~  '0',    '1',    '2',    '3',    '4',    '5',    '6',    '7',
            LCdigit,LCdigit,LCdigit,LCdigit,LCdigit,LCdigit,LCdigit,LCdigit,
    // 56~  '8',    '9',    ':',    ';',    '<',    '=',    '>',    '?',
            LCdigit,LCdigit,LCendl ,LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,
    // 64~  '@',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
            LCdilim,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,
    // 72~  'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
            LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,
    // 80~  'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
            LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,
    // 88~  'X',    'Y',    'Z',    '[',    '\',    ']',    '^',    '_'
            LCalpha,LCalpha,LCalpha,LCdilim,LCdilim,LCdilim,LCdilim,LCalpha,
    // 96~  '`',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
            LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,
    // 104~ 'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
            LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,
    // 112~ 'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
            LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,LCalpha,
    // 120~ 'x',    'y',    'z',    '{',    '|',    '}',    '~',    DEL
            LCalpha,LCalpha,LCalpha,LCdilim,LCdilim,LCdilim,LCdilim,LCnull
    };

    if ((ucchar)c & 0x80) return (LCalpha);
    return (table[c]);
}

#endif

