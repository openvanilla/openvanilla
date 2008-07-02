//#define OV_DEBUG
#include "OVUtility.h"
#include "AVBuffer.h"


AVBuffer::AVBuffer(vector<OVOutputFilter*> *ovof, OVService *s)
	: ovof_vector(ovof), srv(s)
{};
AVBuffer::AVBuffer(AVDisplayServer* svr, vector<OVOutputFilter*> *ovof, OVService *s)
       	: dsvr(svr), ovof_vector(ovof), srv(s)
{};

void AVBuffer::setDisplayServer(AVDisplayServer *svr)
{
	dsvr = svr;
}

OVBuffer* AVBuffer::clear()
{
	bufstr = "";
	return this;
}
OVBuffer* AVBuffer::append(const char *s)
{
	bufstr += s;
	return this;
}
OVBuffer* AVBuffer::send()
{
	vector<OVOutputFilter*>::iterator of;
	for(of = ovof_vector->begin(); of != ovof_vector->end(); of++) {
		if(!strcmp((*of)->identifier(), "OVOFFullWidthCharacter")){
			murmur("Fullshape output filter");
			if(dsvr->getisFullShapeFlag() )   
				bufstr = (*of)->process(bufstr.c_str(), srv);
		} 
		else if (dsvr->getisChineseFlag()&& strcmp(bufstr.c_str()," ") ) {
			murmur("Pass filter %s", (*of)->identifier());
			bufstr = (*of)->process(bufstr.c_str(), srv);
		}
	}
	dsvr->sendBuf(bufstr.c_str());
	clear();
	//dsvr->showBuf(false); 
	return this;
}

OVBuffer* AVBuffer::update() //set + show
{
	murmur("OVBuffer* AVBuffer::update() ");
	return update(-1, -1, -1);
} 

OVBuffer* AVBuffer::update(int cursorPos, int markFrom=-1, int markTo=-1)
{
	murmur("OVBuffer* AVBuffer::update(cursorPos=%d, markFrom=%d, markTo=%d ",cursorPos,markFrom,markTo);
	dsvr->setBufString(bufstr.c_str(),cursorPos,markFrom,markTo);
	dsvr->setMarkFrom(markFrom);
	dsvr->setMarkTo(markTo);
	//dsvr->showBuf(true); 
	return this;
} 

int AVBuffer::isEmpty()
{
	if (!bufstr.length()) return 1; return 0;
}
