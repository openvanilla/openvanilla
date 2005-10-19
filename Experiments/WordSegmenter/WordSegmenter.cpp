#include "WordSegmenter.h"

#include <iostream>
#include <vector>

using namespace std;

bool WordSegmenter::loadModel(map<string, Bin*>& lm)
{
	lm_ = lm;
	
	return true;
}

bool WordSegmenter::segment(string& inputString, string& outputString)
{
	size_t length = inputString.length();
	vector<double> scores(length + 1);
	vector<int> tracks(length + 1);
	for(int i = 0; i < length + 1; i++)
	{
		scores[i] = 0.0;
		tracks[i] = -1;
	}
		
	for(int index = 1; index <= length; index++)
	{
		for(int prefix = index - 1; prefix >= 0; prefix--)
		{
			cerr << endl << "(" << prefix << "," << index << "):";
			string rightGram = inputString.substr(prefix, index - prefix);
			cerr << rightGram.c_str() << "=";
			
			if(lm_.count(rightGram) > 0) {
				double tempScore = scores[prefix];
				cerr << scores[prefix] << " + ";

				int left = tracks[prefix];
				if(left >= 0 && left != prefix)
				{
					string leftGram = inputString.substr(left, prefix - left);
					string bigram = leftGram + " " + rightGram;
					cerr << "(test bigram:" << bigram << ")";
					if(lm_.count(bigram) > 0) {
						double bigramScore = lm_[bigram]->logprob;
						cerr << bigramScore << " + ";
						tempScore += bigramScore;
					}
					else if(lm_.count(leftGram) > 0) {
						double bigramBackOff =
							lm_[leftGram]->backoff + lm_[rightGram]->backoff;
						cerr << bigramBackOff << " + ";
						tempScore += bigramBackOff;
					}
				}

				double unigramScore = lm_[rightGram]->logprob;
				cerr << unigramScore;
				tempScore += unigramScore;
				cerr << " = " << tempScore << endl;
				if(scores[index] == 0.0f || tempScore > scores[index]) {
					cerr << "argmax=" << prefix << "," << tempScore << endl;
					scores[index] = tempScore;
					tracks[index] = prefix;
				}
			}
			else
				cerr << "none" << endl;
		}
		if(tracks[index] == -1)	tracks[index] = index - 1;
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