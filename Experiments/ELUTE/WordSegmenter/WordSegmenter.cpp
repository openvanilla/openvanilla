#include "WordSegmenter.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool WordSegmenter::loadModel(string& lmFilePath)
{
	if(lm_ = new LanguageModel(lmFilePath))
		return true;
	else
		return false;
}

bool WordSegmenter::segment(vector<Token*>& inputTokens, vector<Token*>& outputTokens)
{
	size_t length = inputTokens.size();
	cerr << "input tokens: " << length << endl;
	for(size_t shift = 0; shift < length; shift++)
		cerr << inputTokens[shift]->word << endl;		
	cerr << "----" << endl;
	vector<double> scores(length + 1);
	vector<int> tracks(length + 1);
	for(size_t i = 0; i < length + 1; i++)
	{
		scores[i] = 0.0;
		tracks[i] = -1;
	}

	for(size_t index = 1; index <= length; index++)
	{
		double bestScore = 0.0f;
		int bestPrefix = -1;
		for(int prefix = index - 1; prefix >= 0; prefix--)
		{
			cerr << endl << "(" << prefix << "," << index << "):";
			//string rightGram = inputString.substr(prefix, index - prefix);
			string rightGram("");
			for(size_t shift = prefix, count = 0; count < index - prefix; count++, shift++)
				rightGram += inputTokens[shift]->word;
			cerr << rightGram.c_str() << "=";
			
			if(lm_->has(rightGram)) {
				double tempScore = scores[prefix];
				cerr << scores[prefix] << " + ";

				int left = tracks[prefix];
				if(left >= 0 && left != prefix)
				{
					//string leftGram = inputString.substr(left, prefix - left);
					string leftGram("");
					for(size_t count = 0, shift = left; count < prefix - left; count++, shift++)
						leftGram += inputTokens[shift]->word;
					string bigram = leftGram + " " + rightGram;
					cerr << "(test bigram:" << bigram << ")";
					if(lm_->has(bigram)) {
						double bigramScore = lm_->getLogProb(bigram);
						cerr << bigramScore << " + ";
						tempScore += bigramScore;
					}
					else if(lm_->has(leftGram)) {
						double bigramBackOff =
							lm_->getLogProb(leftGram) + lm_->getBackOff(rightGram);
						cerr << bigramBackOff << " + ";
						tempScore += bigramBackOff;
					}
				}

				double unigramScore = lm_->getLogProb(rightGram) + scores[prefix];
				cerr << unigramScore;
				tempScore += unigramScore;
				cerr << " = " << tempScore << endl;
				if(bestScore == 0.0f || tempScore > bestScore)
				{
					bestScore = tempScore;
					bestPrefix = prefix;
					cerr << "argmax=" << prefix << "," << tempScore << endl;
				}
				
			}
			else if(index - prefix == 1)
			{
				bestScore += lm_->getUnkLogProb() + scores[prefix];
				bestPrefix = prefix;
			}
			else
				cerr << "none" << endl;
		}
		scores[index] = bestScore;
		tracks[index] = bestPrefix;
		cerr << endl << "best:" << bestPrefix << ", score=" << bestScore << endl;

		if(tracks[index] == -1)	tracks[index] = index - 1;
		
		cerr << endl << "----" << endl;
	}
	
	size_t boundary = length;
	size_t suffix = length;
	cerr << endl << "backtrack:";
	while(boundary > 0) {
		boundary = tracks[boundary];
		cerr << " " << boundary;
		//outputString =
		//	inputString.substr(boundary, suffix - boundary) +
		//	" " + outputString;
		Token* token = new Token();
		for(size_t count = 0, shift = boundary; count < suffix - boundary; count++, shift++)
			token->word += inputTokens[shift]->word;
		outputTokens.push_back(token);
		suffix = boundary;
	}
	cerr<<endl;
	//outputString = outputString.substr(0, outputString.length() - 1);
	reverse(outputTokens.begin(), outputTokens.end());

	return true;
}
