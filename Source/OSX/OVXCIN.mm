// OVXCIN.mm

#define OVDEBUG
#include "OVXCIN.h"

#include <fstream>
#include <iostream>

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
	ifstream in(fname, ios_base::binary);
	
	in.seekg(0, ios_base::end);
	int length = in.tellg();
	in.seekg(0);

	cout<<length<<endl;
	
	char *buffer = (char*)calloc(1, length);
	in.get(buffer, length, 0);
	
	for (int i=0;i<length;i++) printf("%c", buffer[i]);
	
	ofstream out("test.txt", ios_base::binary);
	out.write(buffer, length);
	
	out.close();	
	in.close();
	free(buffer);
}

int main(int argc, char**argv)
{        
    OVXCIN cin;
    cin.read(argv[1]);    
}

