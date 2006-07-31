#include "LanguageModel.h"

#include "OVStringToolKit.h"
#include "OVFileHandler.h"

#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

LanguageModel::LanguageModel (string& lmFilePath)
{
	LanguageModel::unknownWordLabel = "<unk>";
	OVFileHandler *fh = new OVFileHandler(lmFilePath.c_str());
	vector<string> fileStringVector;
	vector<string> delimiters;
	string delimiter("\t");
	delimiters.push_back(delimiter);
	bool readGramFlag = false;
	if(!fh->isOpened()) {
		cerr << "language model file not found." << endl;
		exit(-1);
	}

	if(fh->getLines(fileStringVector) > 0) {
		for(size_t i = 0; i < fileStringVector.size(); i++) {
			string row = fileStringVector[i];
			if(!readGramFlag &&
				(row == "\\1-grams:" || row == "\\2-grams:"))
					readGramFlag = true;

			if(readGramFlag) {
				vector<string> tuple;			
				if(OVStringToolKit::splitString(row, tuple, delimiters, false) > 1) {
					Gram* gram = new Gram();
					gram->gram = tuple[1];

					istringstream isLogProb(tuple[0]);
					if(!(isLogProb >> gram->logprob)) {
						cerr << "LogProb conversion error: " << tuple[0] << endl;
						exit(-1);
					}

					if(tuple.size() == 2)
						gram->backoff = 0.0f;
					else if(tuple.size() == 3) {
						istringstream isBackOff(tuple[2]);
						if(!(isBackOff >> gram->backoff)) {
							cerr << "BackOff conversion error: " << tuple[2] << endl;
							exit(-1);
						}
					}
					else
					{
						cerr << "lm file format error: " << row <<endl;
						exit(-1);
					}

					lm_.insert(make_pair(gram->gram, gram));
				}
			}
		}	
	}

	delete fh;
}

double LanguageModel::getLogProb(string& gram)
{
	return lm_[gram]->logprob;
}

double LanguageModel::getBackOff(string& gram)
{
	return lm_[gram]->backoff;
}

double LanguageModel::getUnkLogProb()
{
	return lm_[LanguageModel::unknownWordLabel]->logprob;
}

size_t LanguageModel::getCount(string& gram)
{
	return lm_.count(gram);
}

bool LanguageModel::has(string& gram)
{
	return lm_.count(gram) > 0 ? true : false;
}
