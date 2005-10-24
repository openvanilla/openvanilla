#include <cxxtest/TestSuite.h>
#include "LanguageModel.h"

using namespace std;

class TestLanguageModel : public CxxTest::TestSuite 
{
public:
	void testGetLogProb(void)
	{
		string gram = "下雨 天";
		cerr << gram << " logprob: " << lm_.getLogProb(gram) << endl;
		
		gram = "下雨天";
		cerr << gram << " logprob: " << lm_.getLogProb(gram) << endl;
		
		gram = "下雨";
		cerr << gram << " logprob: " << lm_.getLogProb(gram) << endl;
		
		gram = "雨天";
		cerr << gram << " logprob: " << lm_.getLogProb(gram) << endl;
		
		gram = "下";
		cerr << gram << " logprob: " << lm_.getLogProb(gram) << endl;
		
		gram = "雨";
		cerr << gram << " logprob: " << lm_.getLogProb(gram) << endl;
		
		gram = "天";
		cerr << gram << " logprob: " << lm_.getLogProb(gram) << endl;
	}
	
	void testGetBackOff(void)
	{
		string gram = "下雨 天";
		cerr << gram << " back-off: " << lm_.getBackOff(gram) << endl;
		
		gram = "下雨天";
		cerr << gram << " back-off: " << lm_.getBackOff(gram) << endl;
		
		gram = "下雨";
		cerr << gram << " back-off: " << lm_.getBackOff(gram) << endl;
		
		gram = "雨天";
		cerr << gram << " back-off: " << lm_.getBackOff(gram) << endl;
		
		gram = "下";
		cerr << gram << " back-off: " << lm_.getBackOff(gram) << endl;
		
		gram = "雨";
		cerr << gram << " back-off: " << lm_.getBackOff(gram) << endl;
		
		gram = "天";
		cerr << gram << " back-off: " << lm_.getBackOff(gram) << endl;
	}
	
private:
	LanguageModel lm_;
};