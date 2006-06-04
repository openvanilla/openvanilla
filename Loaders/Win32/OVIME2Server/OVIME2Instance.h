#ifndef	_OVIME2_INSTANCE_H_
#define	_OVIME2_INSTANCE_H_

#include "AVEmbeddedLoader.h"
#include "AVDisplayServer.h"
#include "AVBuffer.h"
#include "AVCandidate.h"
#include "AVKeyCode.h"

#include <vector>
#include <string>

using std::vector;
using std::wstring;

class OVIME2Server;

class OVIME2Instance : public AVDisplayServer
{
public:
	OVIME2Instance(OVIME2Server* server);
	virtual ~OVIME2Instance(void);

	void initContext(int n);
	void reloadConfig();
	bool keyEvent( AVKeyCode c);
	void closeModule();
protected:
	AVCandidate *candi;
	AVBuffer *buf;

	vector<OVInputMethodContext*> ctx_vector;
	vector<bool> startedCtxVector;	// 這是很浪費的作法 orz

	int activatedIm;
	OVIME2Server* svr;

public:	// Display server
	virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showCandi(bool t);
	virtual AVDisplayServer *showNotify(const char *str);
	virtual AVDisplayServer *hideNotify();
	virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);

	wstring getCompStr(){	return compStr;	}
	vector<wstring>& getCandidates(){	return candidates;	}
	wstring getResultStr(){	return resultStr;	}
	wstring getNotifyStr(){	return notifyStr;	}
	int getCursorPos(){	return cursorPos;	}
	int getMarkFrom(){	return markFrom;	}
	int getMarkTo(){	return markTo;	}

protected:
	bool showCandWnd;
	bool showCompWnd;
	int cursorPos;
	int markFrom;
	int markTo;
	vector<wstring> candidates;
	wstring compStr;
	wstring resultStr;
	wstring notifyStr;
};

#endif
