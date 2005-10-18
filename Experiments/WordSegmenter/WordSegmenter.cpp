#include "WordSegmenter.h"

#include <iostream>
#include <vector>

using namespace std;

bool WordSegmenter::loadModel(map<string, int>& lm)
{
	lm_ = lm;
	
	return true;
}

bool WordSegmenter::segment(string& inputString, string& outputString)
{
	size_t length = inputString.length();
	vector<int> scores(length + 1);
	vector<int> tracks(length + 1);
	for(int i = 0; i < length + 1; i++)
	{
		scores[i] = 0;
		tracks[i] = 0;
	}
		
	for(int index = 1; index <= length; index++)
	{
		for(int prefix = index - 1; prefix >= 0; prefix--)
		{
			cerr << endl << "(" << prefix << "," << index << "):";
			string rightGram = inputString.substr(prefix, index - prefix);
			cerr << rightGram.c_str() << "=";
			
			if(lm_.count(rightGram) > 0) {
				int tempScore = scores[prefix];
				cerr << scores[prefix] << " + ";
				for(int left = prefix - 1; left >= 0; left--)
				{
					string leftGram = inputString.substr(left, prefix - left);
					string biGram = leftGram + " " + rightGram;
					if(lm_.count(biGram) > 0) {
						cerr << lm_[biGram] << "*3 + ";
						tempScore += lm_[biGram]*3;
					}
				}

				cerr << lm_[rightGram];
				tempScore += lm_[rightGram];
				cerr << " = " << tempScore << endl;
				if(tempScore > scores[index]) {
					cerr << "argmax=" << prefix << "," << tempScore << endl;
					scores[index] = tempScore;
					tracks[index] = prefix;
				}
			}
			else
				cerr << "none" << endl;
		}
		cerr << "----" << endl;
	}
	
	int boundary = length;
	int suffix = length;
	cerr << endl << "backtrack:";
	while(boundary > 0) {
		boundary = tracks[boundary];
		cerr << " " << boundary;
		outputString =
			inputString.substr(boundary, suffix - boundary) +
			" " + outputString;
		suffix = boundary;
	}
	cerr<<endl;
	outputString = outputString.substr(0, outputString.length() - 1);

	return true;
}