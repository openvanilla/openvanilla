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
	static AVLoader* open();
	
	static void close();	

	void initContext(int n);
	void connectDisplayServer(AVDisplayServer *svr);
	bool keyEvent(int n, AVKeyCode c);
	bool moduleName(int i, char *str);
	void unloadCurrentModule();

	
	int getInputMethodCount() { return (int) imCount;} // excluding kp
	int getKeypreprocessorCount() { return (int) kpCount;}
	int getOutputFilterCount() {return (int) ofCount;} // -1 for excluding Fullcharaterwidth OF module.
	

	int getSwitchedBoPoMoFoLayoutModIndex(int currentId);
	void getAllModuleNames(const char* nameList[]);
	OVInputMethodContext* GetcurrentContext() {  
		if(activatedIm > -1)
		return ctx_vector[activatedIm]; 
		else 
		return NULL;
	};
	const char* getBaseDirectory() ;
	const char* getGlobalConfigKey(const char*);
	void setGlobalConfigKey(const char*, const char*);
	void setGlobalConfig(const char* name);
	void AVLoader::syncMenuConfig(int);
	void AVLoader::syncConfigMenu(int);

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
	//vector<bool> startedCtxVector;	// 這是很浪費的作法 orz

	int activatedIm;

	size_t kpCount;
	size_t imCount;
	size_t ofCount;

	bool hasBoPoMoFo;
	bool hasPhoneticHsu;

private:
	static AVLoader *globalLoader;
};
