// OVCIN.h

#ifndef __OVCIN_h
#define __OVCIN_h

//#include <OpenVanilla/OpenVanilla.h>
#include "OpenVanilla.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace _OVCIN{
    const int NUM_PROPERTY = 5;
    const int NUM_MAP = 2;
    enum EnumProperty{
        P_SELKEY=0, P_ENAME, P_CNAME, P_ENDKEY, P_ENCODING
    };
    enum EnumMap {
        M_KEY, M_CHAR
    };
    enum State {
        PARSE_BLOCK, PARSE_LINE
    };

    template<class T1, class T2>
    struct CmpPair {
        typedef pair<T1, T2> T;
        bool operator() (const T& a, const T& b){
            return a.first < b.first;
        }
    };

    typedef CmpPair<string, string> cmpBlockEntry;
    typedef CmpPair<string, vector<string> > cmpMapEntry;
};

class OVCIN
{
private:
    typedef vector< pair<string, vector<string> > > CinMap;
    _OVCIN::State state;
    string delimiters;
    string properties[_OVCIN::NUM_PROPERTY];
    vector< pair<string, string > > block_buf;
    CinMap maps[_OVCIN::NUM_MAP];
    int curMapIndex;

public:    
    // VXCIN::read()
    OVCIN(char* fileName);
    
    // VXCIN::selKey()
    inline string& getSelKey();
    
    // VXCIN::name()
    inline string& getCName();
    
    // VXCIN::name()
    inline string& getEName();
    
    inline string& getEndKey();
    
    inline string& getEncoding();
    
    // VXCIN::isEndKey()
    inline bool isEndKey(char keyChar);
    
    inline bool isValidKey(const string& keyString) const;
    
    // VXCIN::getKey()
    inline int getCharVectorByKey(const string& inKey,
                           vector<string>& outStringVectorRef);
    
    // VXCIN::find()
    inline int getWordVectorByChar(const string& inKey,
                            vector<string>& outStringVectorRef);

protected:
    int getVectorFromMap(const CinMap& inMapRef,
                         const string& inKey,
                         vector<string>& outStringVectorRef);
    void lowerStr(string& str);
    void parseCinVector(const vector<string>& cinVector);
    void setBlockMap();
    int setProperty(const string& key, const string& value);
    void insertMap(const string& line);
    int searchCinMap(const CinMap& m, const string& key) const;
};


string& OVCIN::getSelKey()
{
    return properties[_OVCIN::P_SELKEY];
}

string& OVCIN::getCName()
{
    return properties[_OVCIN::P_CNAME];
}

string& OVCIN::getEName()
{
    return properties[_OVCIN::P_ENAME];
}

string& OVCIN::getEncoding()
{
    return properties[_OVCIN::P_ENCODING];
}

string& OVCIN::getEndKey()
{
    return properties[_OVCIN::P_ENDKEY];
}

bool OVCIN::isEndKey(char keyChar)
{
    int foundIndex = getEndKey().find(keyChar, 0);
    if(foundIndex > -1)
        return true;
    else
        return false;
}

bool OVCIN::isValidKey(const string& keyString) const
{
    if( searchCinMap( maps[_OVCIN::M_KEY], keyString ) != -1 )
        return true;
    else
        return false;
}

int OVCIN::getCharVectorByKey(const string& inKey,
                              vector<string>& outStringVectorRef)
{
    return getVectorFromMap(maps[_OVCIN::M_KEY], inKey, outStringVectorRef);
}

int OVCIN::getWordVectorByChar(const string& inKey,
                               vector<string>& outStringVectorRef)
{
    return getVectorFromMap(maps[_OVCIN::M_CHAR], inKey, outStringVectorRef);
}
#endif
