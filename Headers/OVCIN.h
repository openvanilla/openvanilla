// OVCIN.h

#ifndef __OVCIN_h
#define __OVCIN_h

#include <OpenVanilla/OpenVanilla.h>

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
public:	
	// VXCIN::read()
    OVCIN(char* fileName);
	
    ~OVCIN();

	// VXCIN::selKey()
	string& getSelKey();
	
	// VXCIN::name()
	string& getCName();
	
	// VXCIN::name()
	string& getEName();
	
	string& getEndKey();
	
	string& getEncoding();
	
	// VXCIN::isEndKey()
	bool isEndKey(char keyChar);
	
	bool isValidKey(const string& keyString) const;
	
	// VXCIN::getKey()
	int getCharVectorByKey(string inKey,
						   vector<string>& outStringVectorRef);
	
	// VXCIN::find()
	int getWordVectorByChar(string inKey,
							vector<string>& outStringVectorRef);

protected:
    typedef vector< pair<string, vector<string> > > CinMap;
	int getVectorFromMap(CinMap& inMapRef,
						 string inKey,
						 vector<string>& outStringVectorRef);
	
private:
    void lowerStr(string& str);
    void setBlockMap();
    void parseCinVector(const vector<string>& cinVector);
    int setProperty(const string& key, const string& value);
    void insertMap(const string& line);

    _OVCIN::State state;
    string delimiters;
    string properties[_OVCIN::NUM_PROPERTY];
    CinMap maps[_OVCIN::NUM_MAP];
    int curMapIndex;
    vector< pair<string, string > > block_buf;

	string selkey;
    string ename;
    string cname;
	string endkey;
	string encoding;
	
};


#endif
