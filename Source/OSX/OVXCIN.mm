// OVXCIN.mm

#define OVDEBUG
#include "OVXCIN.h"

#include <fstream>
#include <locale>

using namespace std;

OVXCIN::OVXCIN() {}

OVXCIN::~OVXCIN()
{
	keytable.clear();
	chartable.clear();
}

vector<string> OVXCIN::find(string key)
{
    return chartable[key];
}

void OVXCIN::read(char *fname, int shiftselkey)
{
	locale utf8Locale("");
	locale::global(utf8Locale);
	
	ifstream in(fname);
	in.imbue(utf8Locale);
	    
	ofstream out("test.txt");
	string buf;
	out.imbue(utf8Locale);
    in >> buf;
	out<<buf<<endl;
	in.close();
	out.close();
}

int main(int argc, char**argv)
{        
    OVXCIN cin;
    cin.read(argv[0]);    
}

