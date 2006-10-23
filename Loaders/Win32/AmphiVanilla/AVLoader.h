#include "AVEmbeddedLoader.h"
#include "AVDisplayServer.h"
#include "AVBuffer.h"
#include "AVCandidate.h"
#include "AVKeyCode.h"

#include <vector>

using std::vector;

class AVLoader : public OVBase
{
public:
	static AVLoader *getLoader();
	static void shutdown();
	~AVLoader();
	void initContext(int n);
	void reloadConfig();
	void connectDisplayServer(AVDisplayServer *svr);
	bool keyEvent(int n, AVKeyCode c);
	bool moduleName(int i, char *str);
	void closeModule();

	//<comment author='b6s'>Add these two functions temporarily, will refactor them later.
	int getInputMethodCount();
	int getOutputFilterCount();
	//</comment>

private:
	AVLoader();
	static AVLoader *globalLoader;
protected:
	AVEmbeddedLoader *em;
	AVDisplayServer *dsvr;
	AVCandidate *candi;
	AVBuffer *buf;

	vector<OVInputMethodContext*> ctx_vector;
	vector<OVOutputFilter*> ovof_vector;
	vector<bool> startedCtxVector;	// 這是很浪費的作法 orz

	int activatedIm;
};
