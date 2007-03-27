#include <string>
#include <iostream>
using namespace std;

string group0= "1qaz2wsxedcrfv5tgbyhn";
string group1= "ujm";
string group2= "8ik,9ol.0p;/-";
string group3= " 6347";

void convert2LegalBPMF(string input, string &output)
{			
	bool flags[4];
	string foo[4];
	for (int i=0;i<4;i++)
	{
		flags[i]=false;		
	}	
	for(int i=0;i<input.length();i++)
	{
		if(group0.find(input[i]) != string::npos)
		{
			flags[0]=true;
			foo[0]=input[i];			
		}
		else if(group1.find(input[i])!= string::npos)
		{
			flags[1]=true;
			foo[1]=input[i];
		}
		else if(group2.find(input[i])!= string::npos)
		{
			flags[2]=true;
			foo[2]=input[i];
		}
		else if(group3.find(input[i])!= string::npos)
		{
			flags[3]=true;
			foo[3]=input[i];
		}
	}		
	output="";
	for (int i=0;i<4;i++)
	{
		if(flags[i])
		{
			output += foo[i];
		}		
	}	
	return ;
}