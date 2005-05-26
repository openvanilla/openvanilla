// OVSCIM.h: the first OpenVanilla-SCIM bridge

#ifndef __OVSCIM_H
#define __OVSCIM_H

#include <scim.h>
using namespace scim;

#include "OpenVanilla.h"
#include "DummyLoader.h"

class OVSCIMFactory : public IMEngineFactoryBase {
public:
    OVSCIMFactory(OVInputMethod *i, const ConfigPointer& config);
    virtual ~OVSCIMFactory();

	virtual WideString get_name() const;
	virtual String get_uuid() const;
	virtual String get_icon_file() const;
	virtual WideString get_authors() const;
	virtual WideString get_credits() const;
	virtual WideString get_help() const;

	virtual IMEngineInstancePointer create_instance (const String& encoding, int id=-1);
	friend class OVSCIMInstance;

protected:
    OVInputMethod *im;
};

class OVSCIMInstance : public DIMEInstance
{
public:
	OVSCIMInstance(OVInputMethodContext *c, OVSCIMFactory *factory, const String& encoding, int id=-1);
	virtual ~OVSCIMInstance();

	virtual bool process_key_event( const KeyEvent& key );
	virtual void select_candidate( unsigned int index );
	virtual void update_lookup_table_page_size( unsigned int page_size );
	virtual void lookup_table_page_up();
	virtual void lookup_table_page_down();
	virtual void reset();
	virtual void focus_in();
	virtual void focus_out();
	
	OVInputMethodContext *cxt;
	
    DummyBuffer buf;
    DummyCandidate candi;
    DummyService srv;
    DummyDictionary dict;
};

#endif

