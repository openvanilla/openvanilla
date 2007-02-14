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
	static AVLoader* open()
	{
		if(globalLoader == NULL)
			globalLoader = new AVLoader();
		return globalLoader;
	}

	static void close();	

	void initContext(int n);
	void connectDisplayServer(AVDisplayServer *svr);
	bool keyEvent(int n, AVKeyCode c);
	bool moduleName(int i, char *str);
	void unloadCurrentModule();

	//<comment author='b6s'>Add these two functions temporarily, will refactor them later.
	int getInputMethodCount();
	int getOutputFilterCount();
	//</comment>

	int getSwitchedBoPoMoFoLayoutModIndex();
	void getAllModuleNames(const char* nameList[]);

protected:
	AVLoader();
	~AVLoader();

	void loadPrefs();
	void loadModules();

	AVEmbeddedLoader *em;
	AVDisplayServer *dsvr;
	AVCandidate *candi;
	AVBuffer *buf;

	vector<OVInputMethodContext*> ctx_vector;
	vector<OVOutputFilter*> ovof_vector;
	vector<bool> startedCtxVector;	// 這是很浪費的作法 orz

	int activatedIm;

	bool hasBoPoMoFo;
	bool hasPhoneticHsu;

private:
	static AVLoader *globalLoader;
};
