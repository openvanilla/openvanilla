
// ineval.cpp: this module demonstrates the inline evaluation,
//             which is the modification of FILEinput class,
//             with THAT simple!

#include <iostream>
#include <strstream>
#include "basiclex.h"

using namespace std;

// --------------------------------------------------------------------------

const size_t FI_MaxSize = 65534;   // this limit does virtually not exist
                                   // on machines other than REAL MODE...

class FILEinput : public LexInput {
    public:
        FILEinput (cchar* str) : posi(0)
        {
            bufsize = strlen(str);
            buf = new cchar [bufsize + 1];
            strcpy (buf, str);
        }

        FILEinput (FILE* fs) : posi(0)
        {
            if (fs == NULL) LexError (LEfatal, 0, "file can't be NULL!", TRUE);

            fseek (fs, 0L, SEEK_END);
            bufsize = ftell(fs);
            fseek (fs, 0L, SEEK_SET);

            if (bufsize > FI_MaxSize)
                LexError (LEfatal, 0, "source code too large", TRUE);

            buf = new cchar[bufsize];
            if (buf == NULL) LexError (LEmem, 0, NULL, TRUE);
            fread (buf, bufsize, 1, fs);
        }
        ~FILEinput()    { delete[] buf; }

        size_t pos()    { return (posi); }
        void set(size_t p, size_t line) { posi = p; linecntr = line; }


    protected:
        virtual cchar nextchar()
        {
            if (posi >= bufsize) return(0);
            return (buf[posi++]);
        }

        cchar* buf;
        size_t bufsize, posi;
    };

class VarTable {
    public:
        VarTable();
        int index(cchar*);
        double set(int idx, double val);
        double get(int idx);

    protected:
        double var[27];
    } vars;

// --------------------------------------------------------------------------

void    statement(BASICLex&, FILEinput&);
void    print(BASICLex&);
void    assignment(BASICLex&, int index);
void    inputstat(BASICLex&);
void    forstat(BASICLex&, FILEinput&);

double  expression(BASICLex&, BOOL equagn=FALSE);
double  subexp(BASICLex&);

char outbuf[8192];
strstream strout(outbuf, 8192);

extern "C" char *EvalEZBasic(char *inbuf)
{
    try
    {
    	FILEinput input(inbuf);
        BASICLex  lex (&input);

        try
        {
        	statement (lex, input);
        }
        catch(LexException)
        {
        	lex.consume();              // clear token buffer
            lex.skipbeyond('\n');       // clear error condition
        }
    }
    catch(LexException)
    {
        strout << "exception caught! warum?!" << endl;
        return (0);
    }
    catch(int)
    {
    }

    return(outbuf);
}

void statement (BASICLex& lex, FILEinput& input)
{
    while (1)
    {
        Token t = lex.ahead();
        cchar* buf = lex.consume();

        if (t == TKend) return;

        if (t == TKends) continue;

        switch (t)
        {
            case BTliteral:
                if (!cstricmp(buf, "system"))
                {
                    throw(1);
                    return;
                }
                assignment(lex, vars.index(buf));
                break;

            case BTprint:
                print (lex);
                break;

            case BTinput:
                inputstat (lex);
                break;

            case BTfor:
                forstat (lex, input);
                break;

            case BTnext:
                if (lex.ahead() == BTliteral) lex.consume();
                return;

            default:
                LexError (LEnotimplemented, lex.line(), buf);
        }
    }
}

void print (BASICLex& lex)
{
    int sendlf = 1;

    while (1)
    {
        Token t = lex.ahead();

        if (t == BTliteral || t == BTint || t == BTlp)
        {
            strout << expression(lex);
            sendlf =  1;
            continue;
        }


        cchar* buf = lex.consume();

        switch(t)
        {
            case BTstring:
                strout << buf;
                sendlf = 1;
                break;

            case TKends:
            case TKend:
                if (sendlf) strout << endl;
                return;

            case BTsmcln:
                sendlf = 0;
                break;

            case BTcomma:
                strout << "\t";
                break;

            default:
                LexError (LEsyntax, lex.line());
        }
    }
}

void inputstat (BASICLex& lex)
{
    while (1)
    {
        Token t = lex.ahead();
        cchar* buf = lex.consume();

        switch (t)
        {
//          case BTliteral: cin >> vars.get(vars.index(buf)); break;
            case BTstring:  strout << buf; break;
            case BTsmcln:   strout << "? "; break;
            case BTcomma:   strout << " "; break;
            case TKends:
            case TKend:     return;
            default:        LexError(LEsyntax, lex.line());
        }
    }
}

void forstat (BASICLex& lex, FILEinput& input)
{
    Token t = lex.ahead();
    if (t != BTliteral) LexError (LEsyntax, lex.line());
    cchar* buf = lex.consume();

    int index = vars.index(buf);

    if (lex.ahead() != BTequal)
        LexError (LEsyntax, lex.line(), "error on assignment.");
    lex.consume();

    vars.set(index, expression(lex));

    if (lex.ahead() != BTto)
        LexError (LEsyntax, lex.line());
    lex.consume();

    double toval = expression(lex);
    double stepval = 1.0;

    if (lex.ahead() == BTstep)
    {
        lex.consume();
        stepval = expression(lex);
    }

    size_t pos = input.pos(), line = input.line();

    for ( ; vars.get(index) <= toval ; vars.set(index, vars.get(index) + stepval))
    {
        input.set(pos, line);
        lex.consume();
        statement(lex, input);
    }
}

void assignment (BASICLex& lex, int index)
{
    if (index == 26)
        LexError (LEsyntax, lex.line(), "unidentified literal.");

    if (lex.ahead() != BTequal)
        LexError (LEsyntax, lex.line(), "error on assignment.");

    lex.consume();

    vars.set(index, expression(lex));
}

double expression (BASICLex& lex, BOOL)
{
    if (lex.ahead() == BTsub)
    {
        lex.consume();
        return (-1 * expression(lex));
    }

    double x = subexp(lex);

    Token t = lex.ahead();

    if (t == BTadd)
    {
        lex.consume();
        return (x + expression(lex));
    }

    if (t == BTsub)
    {
        lex.consume();
        return (x - expression(lex));
    }

    return (x);
}

double subexp (BASICLex& lex)
{
    Token t = lex.ahead();
    double x = 0;

    if (t == BTint)
    {
        cchar* buf = lex.consume();
        x = atof(buf);
    }

    if (t == BTliteral)
    {
        cchar* buf = lex.consume();
        x = vars.get(vars.index(buf));
    }

    if (t == BTlp)
    {
        lex.consume();

        x = expression(lex);

        t = lex.ahead();
        if (t != BTrp) LexError (LEsyntax, lex.line(), "parentheis not match.");
        else lex.consume();
    }

    t = lex.ahead();

    if (t == BTmul)
    {
        lex.consume();
        return (x * subexp(lex));
    }
    if (t == BTdiv)
    {
        lex.consume();
        return (x / subexp(lex));
    }

    return (x);
}

VarTable::VarTable()
{
    for (int i = 0 ; i < 27; i++) var[i] = 0;
}

int VarTable::index(cchar* c)
{
    char cc = toupperc(*c);
    if (cc >= 'A' && cc <='Z') return (cc - 'A');

    return (26);
}

double VarTable::set(int idx, double val)
{
    return (var[idx] = val);
}

double VarTable::get(int idx)
{
    return (var[idx]);
}

