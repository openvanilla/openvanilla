
// basictkn.h: token type used by BASIC, lookup function included

#ifndef __basictkn_h
    #define __basictkn_h

typedef Token BToken;

const BToken    BTliteral = TKliteral,
                    BTstrvar    = BTliteral + 0x01,
                    BTintvar    = BTliteral + 0x02,
                    BTdblvar    = BTliteral + 0x05,
                    BTlabel     = BTliteral + 0x06;

const BToken    BTnumber = TKnumber,
                    BTint       = BTnumber + 0x01,
                    BTdbl       = BTnumber + 0x02,
                    BThex       = BTnumber + 0x03,
                    BTbin       = BTnumber + 0x04,
                    BToct       = BTnumber + 0x05;

const BToken    BTdelim = TKdelimiter,
                    BTadd       = BTdelim + 0x01,
                    BTsub       = BTdelim + 0x02,
                    BTmul       = BTdelim + 0x03,
                    BTdiv       = BTdelim + 0x04,
                    BTpow       = BTdelim + 0x05,
                    BTlp        = BTdelim + 0x06,
                    BTrp        = BTdelim + 0x07,
                    BTcolon     = BTdelim + 0x10,
                    BTcomma     = BTdelim + 0x11,
                    BTsmcln     = BTdelim + 0x12,
                    BTdot       = BTdelim + 0x13;

const BToken    BTkeyword = TKkeyword,
                    BTequal     = BTkeyword + 0x01,
                    BTlgr       = BTkeyword + 0x02,
                    BTslr       = BTkeyword + 0x04,
                    BTprint     = BTkeyword + 0x05,
                    BTinput     = BTkeyword + 0x06,
                    BTfor       = BTkeyword + 0x07,
                    BTnext      = BTkeyword + 0x08,
                    BTto        = BTkeyword + 0x09,
                    BTstep      = BTkeyword + 0x10,
                    BTgoto      = BTkeyword + 0x11,
                    BTgosub     = BTkeyword + 0x12,
                    BTreturn    = BTkeyword + 0x13,
                    BTif        = BTkeyword + 0x14,
                    BTthen      = BTkeyword + 0x15,
                    BTelse      = BTkeyword + 0x16,
                    BTlet       = BTkeyword + 0x17,
                    BTmid       = BTkeyword + 0x80,
                    BTleft      = BTkeyword + 0x81,
                    BTright     = BTkeyword + 0x82,
                    BTlog       = BTkeyword + 0x83,
                    BTexp       = BTkeyword + 0x84,
                    BTsin       = BTkeyword + 0x85,
                    BTcos       = BTkeyword + 0x86,
                    BTsystem    = BTkeyword + 0x87,
                    BTendstat   = BTkeyword + 0xff;


const BToken    BTstring = TKstring;
const BToken    BTnull   = TKnull;
const BToken    BTerror  = TKerror;
const BToken    BTends   = TKends;
const BToken    BTend    = TKend;

struct KeywordTbl {
    cchar* k; BToken t;
};

static KeywordTbl ktbl[] =
{
    {"PRINT", BTprint},     {"INPUT", BTinput},     {"FOR", BTfor},
    {"NEXT", BTnext},       {"TO", BTto},           {"STEP", BTstep},
    {"GOTO", BTgoto},       {"GOSUB", BTgosub},     {"RETURN", BTreturn},
    {"IF", BTif},           {"THEN", BTthen},       {"ELSE", BTelse},
    {"LET", BTlet},         {"END", BTend},
    {"MID", BTmid},         {"MID$", BTmid},        {"RIGHT", BTright},
    {"RIGHT$", BTright},    {"LEFT", BTleft},       {"LEFT$", BTleft},
    {"LOG", BTlog},         {"EXP", BTexp},         {"SIN", BTsin},
    {"COS", BTcos},         {"SYSTEM", BTsystem},
    {"", BTnull}
};

Token BL_KeywordLookup(cchar* k, Token o = BTliteral)
{
    // brute-forced search, the speed may boost if binary or hash introduced...
    for (int i = 0; ktbl[i].t != BTnull; i++)
        if (!cstricmp(ktbl[i].k, k)) return (ktbl[i].t);

    return (o);
}

Token BL_DelimiterLookup(cchar c)
{
    static BToken tbl1[32] = {
    // 32~  SP,     '!',    '"',    '#',    '$',    '%',    '&',    ''',
            TKnull, TKnull ,TKnull ,TKnull ,TKnull ,TKnull ,TKnull ,TKnull ,
    // 40~  '(',    ')',    '*',    '+',    ',',    '-',    '.',    '/',
            BTlp,   BTrp,   BTmul,  BTadd,  BTcomma,BTsub,  BTdot,  BTdiv,
    // 48~  '0',    '1',    '2',    '3',    '4',    '5',    '6',    '7',
            TKnull, TKnull, TKnull ,TKnull ,TKnull ,TKnull ,TKnull ,TKnull ,
    // 56~  '8',    '9',    ':',    ';',    '<',    '=',    '>',    '?',
            TKnull, TKnull, BTcolon,BTsmcln,BTslr,  BTequal,BTlgr,  BTprint,
        };

    static BToken tbl2[9] = {
    // 88~  'X',    'Y',    'Z',    '[',    '\',    ']',    '^',    '_'
            TKnull, TKnull, TKnull, TKnull, TKnull, TKnull, BTpow,  TKnull,
    // 96~  '`'
            TKnull
        };

    static BToken tbl3[] = {
    // 120~ 'x',    'y',    'z',    '{',    '|',    '}',    '~',    DEL
            TKnull, TKnull, TKnull, TKnull, TKnull, TKnull, TKnull, TKnull
        };


    if (c >= 32 && c < 63)   return (tbl1[c-32]);
    if (c >= 88 && c < 97)   return (tbl2[c-88]);
//  if (c >= 120 && c < 127) return (tbl3[c-120]);
    return (TKnull);
}

#endif

