// OVXCIN.mm

#define OVDEBUG
#include "OVXCIN.h"

#include <fstream>
#include <iostream>
#include <string>

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
	ifstream in(fname);
	
	in.seekg(0, ios_base::end);
	int length = in.tellg();
	in.seekg(0);
	
	char buffer[length];
	in.get(buffer, length);

	// <TODO>getline() for buffer</TODO>

	in.close();
}

int main(int argc, char**argv)
{        
    OVXCIN cin;
    cin.read(argv[1]);    
}

