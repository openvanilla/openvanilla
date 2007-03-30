#include <cxxtest/TestSuite.h>
#include "LanguageModel.h"

using namespace std;

class LanguageModelTest : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string lmFilePath("./as_large-kn-2.lm");
		lm_ = new LanguageModel(lmFilePath);
	}
	
	void testGetLogProb(void)
	{
		string gram = "\xE4\xB8\x8B\xE9\x9B\xA8 \xE5\xA4\xA9"; //rainy /xiayv/, day /tian/
		cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8\xE5\xA4\xA9";
		cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8";
		cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
		gram = "\xE9\x9B\xA8\xE5\xA4\xA9";
		cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
		gram = "\xE4\xB8\x8B";
		cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
		gram = "\xE9\x9B\xA8";
		cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
		gram = "\xE5\xA4\xA9";
		cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
	}
	
	void testGetBackOff(void)
	{
		string gram = "\xE4\xB8\x8B\xE9\x9B\xA8 \xE5\xA4\xA9";
		cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8\xE5\xA4\xA9";
		cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8";
		cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
		gram = "\xE9\x9B\xA8\xE5\xA4\xA9";
		cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
		gram = "\xE4\xB8\x8B";
		cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
		gram = "\xE9\x9B\xA8";
		cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
		gram = "\xE5\xA4\xA9";
		cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
	}

	void tearDown (void)
	{
		delete lm_;
	}
	
private:
	LanguageModel* lm_;
};
