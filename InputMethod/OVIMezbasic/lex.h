
// lex.h: the lexical analyzer (class Lex) and its input (class LexInput)
//        with the extention required by parsing standard BASIC

#ifndef __lex_h
    #define __lex_h

#include "platform.h"
#include "cchar.h"
#include "lexerr.h"

typedef ucchar      LCType;
typedef LCType      (*LCFunc)(cchar);
typedef WORD        Token;

const LCType    LCnull  = 0, LCalpha = 1, LCdigit = 2,
                LCdilim = 3, LCendl  = 4, LCquote = 5,
                LCend   = 127;

const Token     TKliteral = 0x0100,     TKkeyword   = 0x0200,
                TKnumber  = 0x0400,     TKdelimiter = 0x0800,
                TKstring  = 0x1000,     TKnull      = 0,
                TKerror   = 0x00ff,
                TKends    = 0xff00,     TKend     = 0xffff;
                // TKends: end of section (LF or block, etc.)
                
// ------------------------------------------------------------------------

class LexInput;

LCType L_DefLookup(cchar);
const size_t L_DefBufSize = 129;

class Lex {
    public:
        Lex (LexInput*, LCFunc f = &L_DefLookup, size_t s = L_DefBufSize);
        ~Lex();

        Token   ahead();
        Token   skipbeyond(Token, cchar* cmp=NULL);
                                    // skip till token t & return the next
        Token   skipbeyond(cchar);
                                    // skip till char c encounterd & beyond
        cchar*  consume();          // consume the token & get the buffer
        size_t  line();             // dependent on LexInput::line()

    protected:
        cchar*  nextstring(cchar*, cchar*);
                                    // facilities function for string
                                    // (w/ escape char procs. like C++ !)
        cchar   escchar();

        cchar*  buf;
        size_t  bufsize;
        Token   lasttoken;
        LCFunc  lexilook;
        LexInput* input;
    };

// ------------------------------------------------------------------------

class LexInput {        // return 0 if end encountered in next() or peek()
    public:
        LexInput() : linecntr(1), last(0) { ; }

        cchar   peek()      { if (!last) { last = nextchar();
                                           if (last=='\n') linecntr++; }
                              return (last); }
        cchar   next()      { cchar c = peek(); last = 0; return (c); }
        size_t  line()      { return (linecntr); }

    protected:
        virtual cchar   nextchar() = 0;

        size_t  linecntr;
        cchar   last;
    };

// ------------------------------------------------------------------------

inline Lex::Lex (LexInput* i, LCFunc f, size_t s)
           : input(i), lexilook(f), bufsize(s), lasttoken(TKnull)
{
    buf = new cchar[bufsize];

    if (buf == NULL)
        LexError (LEmem);

    if (i == NULL)
        LexError (LEfatal, 0, "fake LexInput object is given.");

    if (s < 2)      // at least one char '\0' is required
        LexError (LEfatal, 0, "given bufsize of Lex::Lex() too small.");
}

inline Lex::~Lex ()
{
    if (buf) delete buf;
}

inline cchar* Lex::consume()
{
    lasttoken = TKnull;
    return (buf);
}

inline size_t Lex::line()
{
    return (input->line());
}

inline Token Lex::ahead()
{
    if (lasttoken != TKnull) return (lasttoken);

    // skip white space and judge if end encountered
    LCType nextlct;
    while ((nextlct = (*lexilook)(input->peek())) == LCnull) input->next();

    if (nextlct == LCend)
    {
//      input->next();
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
            lasttoken = TKliteral;
            while (bufp < bufend)
            {
                LCType t = (*lexilook)(input->peek());
                if (t == LCalpha || t == LCdigit)
                    *bufp++ = input->next();
                else
                    break;
            }
            break;

        case LCdigit:
            lasttoken = TKnumber;
            while (bufp < bufend)
                if ((*lexilook)(input->peek()) == LCdigit)
                    *bufp++ = input->next();
                else
                    break;
            break;

        case LCdilim:
            lasttoken = TKdelimiter;
            *bufp++ = input->next();
            break;

        case LCquote:
            lasttoken = TKstring;
            bufp = nextstring(bufp, bufend);
            break;
    }

    *bufp = 0;
    return (lasttoken);
}

inline Token Lex::skipbeyond(Token t, cchar* cmp)
{
    lasttoken = TKnull;

    while (1)
    {
        Token nt = ahead();
        cchar* buf = consume();
        if (nt == TKend) break;
        if (nt == t)
        {
            if (cmp == NULL) break;
            else if (!cstrcmp(buf, cmp)) break;
        }
    }
    return(ahead());
}

inline Token Lex::skipbeyond(cchar c)
{
    lasttoken = TKnull;

    while (1)
    {
        cchar nc = input->next();
        if (!nc || (nc == c)) break;
    }

    return (ahead());
}

inline cchar* Lex::nextstring(cchar* bufp, cchar* bufend)
{
    // skip the first '"'
    input->next();

    while (bufp < bufend)
    {
        cchar c = input->peek();
        LCType t = (*lexilook)(c);
        if ((t == LCend || t == LCendl) && (c != ':')) break;

        input->next();
        if (c == '\\') c = escchar();
        if (c == '"') break;

        *bufp++ = c;
    }

    return (bufp);
}

inline cchar Lex::escchar()
{
    // the first '\' is already skipped

    cchar  c = input->peek();
    LCType t = (*lexilook)(c);

    if (t == LCend || t == LCnull)
    {
        LexError(LEsyntax, line(), "inproper terminated string.");
        return (' ');
    }

    c = input->next();

    int radix = 8;
    switch (c)
    {
        case '\"':  return ('\"');
        case '?':   return ('?');
        case 'a':   return ('\a');
        case 'b':   return ('\b');
        case 'f':   return ('\f');
        case 'n':   return ('\n');
        case 'r':   return ('\r');
        case 't':   return ('\t');
        case 'v':   return ('\v');
        case '\\':  return ('\\');
        case 'x':   radix = 16; break;
        default:
            if (!isoctc(c))
            {
                LexError(LEunrecognizable, line());
                return(' ');
            }
    }


    ucchar uc = 0;

    if (radix == 8)
    {
        uc = todigit(c);
        uint readchar = 1;

        while ((readchar <= 3) && isoctc(input->peek()))
        {
            uc = (uc * 8) + todigit(input->next());
            readchar++;
        }
    }
    else
    {
        if (ishexc(input->peek())) uc = tohex(input->next());
        if (ishexc(input->peek())) uc = uc * 0x10 + tohex(input->next());
    }

    return ((cchar)uc);
}

// ------------------------------------------------------------------------

inline LCType L_DefLookup(cchar c)
{
    static LCType table[128] = {
    // 0~   NUL,    SOH,    STX,    ETX,    EOT,    ENQ,    ACK,    BEL,
            LCend,  LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull,
    // 8~   BS,     HT,     LF,     VT,     FF,     CR,     SO,     SI,
            LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull, LCnull,
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
            LCdigit,LCdigit,LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,LCdilim,
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

