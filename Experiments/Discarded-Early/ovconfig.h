
const int ovcMaxStrLen=1024;


class Property
{
public:
    char id[ovcMaxStrLen];
    int type;
};


class StringList
{
};

class Localized

class PropertyList
{
public:
    Property *list[ovcMaxListLen];
};

class Config
{
public:
    char ownerid[MaxStrLen];
    
};


class 


config=(owner, list(property))

property=(name, localizednames, configitem)


configitem=
    localizednames |
    string |
    int |
    namedint |
    boolean [derived from namedint]
    namedboolean [derived from namedint]

localizednames=list(locale, string)

int=(range, intvalue)

namedint=list((int, localizednames))
