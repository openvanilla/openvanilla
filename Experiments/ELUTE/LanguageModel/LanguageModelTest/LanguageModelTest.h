#include <cxxtest/TestSuite.h>
#include "LanguageModel.h"

using namespace std;

class LanguageModelTest : public CxxTest::TestSuite 
{
public:
	LanguageModelTest()
	{
		string lmFilePath("../../as_large-kn-2.lm");
		lm_ = new LanguageModel(lmFilePath);
	}

	~LanguageModelTest()
	{
		delete lm_;
	}

	void setUp(void) {}
	
	void testGetLogProb(void)
	{
		string gram = "\xE4\xB8\x8B\xE9\x9B\xA8 \xE5\xA4\xA9"; //rainy /xiayv/, day /tian/
		double logprob = lm_->getLogProb(gram);
		TS_ASSERT_LESS_THAN(logprob, 0.0L);
		cerr << gram << " logprob: " << logprob << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8\xE5\xA4\xA9";
		logprob = lm_->getLogProb(gram);
		TS_ASSERT_LESS_THAN(logprob, 0.0L);
		cerr << gram << " logprob: " << logprob << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8";
		logprob = lm_->getLogProb(gram);
		TS_ASSERT_LESS_THAN(logprob, 0.0L);
		cerr << gram << " logprob: " << logprob << endl;
		
		gram = "\xE9\x9B\xA8\xE5\xA4\xA9";
		logprob = lm_->getLogProb(gram);
		TS_ASSERT_LESS_THAN(logprob, 0.0L);
		cerr << gram << " logprob: " << logprob << endl;
		
		gram = "\xE4\xB8\x8B";
		logprob = lm_->getLogProb(gram);
		TS_ASSERT_LESS_THAN(logprob, 0.0L);
		cerr << gram << " logprob: " << logprob << endl;
		
		gram = "\xE9\x9B\xA8";
		logprob = lm_->getLogProb(gram);
		TS_ASSERT_LESS_THAN(logprob, 0.0L);
		cerr << gram << " logprob: " << logprob << endl;
		
		gram = "\xE5\xA4\xA9";
		logprob = lm_->getLogProb(gram);
		TS_ASSERT_LESS_THAN(logprob, 0.0L);
		cerr << gram << " logprob: " << logprob << endl;
	}
	
	void testGetBackOff(void)
	{
		string gram = "\xE4\xB8\x8B\xE9\x9B\xA8 \xE5\xA4\xA9";
		double backoff = lm_->getBackOff(gram);
		TS_ASSERT_EQUALS(backoff, 0.0L);
		// The highest order grams have no backoff weights.
		cerr << gram << " back-off: " << backoff << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8\xE5\xA4\xA9";
		backoff = lm_->getBackOff(gram);
		TS_ASSERT_LESS_THAN(backoff, 0.0L);
		cerr << gram << " back-off: " << backoff << endl;
		
		gram = "\xE4\xB8\x8B\xE9\x9B\xA8";
		backoff = lm_->getBackOff(gram);
		TS_ASSERT_LESS_THAN(backoff, 0.0L);
		cerr << gram << " back-off: " << backoff << endl;
		
		gram = "\xE9\x9B\xA8\xE5\xA4\xA9";
		backoff = lm_->getBackOff(gram);
		TS_ASSERT_LESS_THAN(backoff, 0.0L);
		cerr << gram << " back-off: " << backoff << endl;
		
		gram = "\xE4\xB8\x8B";
		backoff = lm_->getBackOff(gram);
		TS_ASSERT_LESS_THAN(backoff, 0.0L);
		cerr << gram << " back-off: " << backoff << endl;
		
		gram = "\xE9\x9B\xA8";
		backoff = lm_->getBackOff(gram);
		TS_ASSERT_LESS_THAN(backoff, 0.0L);
		cerr << gram << " back-off: " << backoff << endl;
		
		gram = "\xE5\xA4\xA9";
		backoff = lm_->getBackOff(gram);
		TS_ASSERT_LESS_THAN(backoff, 0.0L);
		cerr << gram << " back-off: " << backoff << endl;
	}

	void tearDown (void) {}
	
private:
	LanguageModel* lm_;
};
