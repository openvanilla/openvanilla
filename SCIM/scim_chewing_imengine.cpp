// OVSCIM.cpp: the first OpenVanilla-SCIM bridge

#define Uses_SCIM_UTILITY
#define Uses_SCIM_IMENGINE
#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_CONFIG_PATH
#define Uses_SCIM_LOOKUP_TABLE
#define Uses_SCIM_IMENGINE_MODULE
#define Uses_SCIM_ICONV
#define Uses_SCIM_DEBUG
#define Uses_SCIM_C_STRING

#include <stdio.h>
#include <scim.h>
#include "scim_chewing_imengine.h"
// #include "OVIMPhoneticStatic.cpp"
#include "OVPhoneticLib.cpp"
extern "C" {
#include "OVPhoneticData.c"
};



using namespace scim;

static IMEngineFactoryPointer _scim_ov_factory(0);
static ConfigPointer _scim_config(0);

// SCIM .so entry points
extern "C" {

    void scim_module_init() {
    }
    
    void scim_module_exit() {
        _scim_config.reset();
    }
    
    unsigned int scim_imengine_module_init(const ConfigPointer& c) {
        _scim_config=c;
        return 1;
    }
    
    IMEngineFactoryPointer scim_imengine_module_create_factory(uint32 e)
    {
        // we have only one engine
        if (e!=0) return IMEngineFactoryPointer(0);
    
        if (_scim_ov_factory.null()) {
            OVSCIMFactory *f=new OVSCIMFactory(_scim_config);
            if (f)
                _scim_ov_factory = f;
            else
                delete f;
        }
        return _scim_ov_factory;
    }
}

// OVSCIMFactory

OVSCIMFactory::OVSCIMFactory(const ConfigPointer& config) {
    fprintf(stderr, "factory init!\n");
	set_languages( "zh_TW,zh_HK,zh_SG" );
}

OVSCIMFactory::~OVSCIMFactory() {
}

WideString OVSCIMFactory::get_name() const {
	return utf8_mbstowcs("OpenVanilla-Phonetic");
}

String OVSCIMFactory::get_uuid() const {
	return String("d1f40e24-cdb7-11d9-9359-02061b0179cb");
}

String OVSCIMFactory::get_icon_file() const {
	return String("/usr/X11R6/share/scim/icons/rawcode.png");
}

WideString OVSCIMFactory::get_authors() const {
	return utf8_mbstowcs("The OpenVanilla Project <http://openvanilla.org>");
}

WideString OVSCIMFactory::get_credits() const {
	return WideString();
}

WideString OVSCIMFactory::get_help() const {
	return utf8_mbstowcs("Help unavailable");
}

IMEngineInstancePointer OVSCIMFactory::create_instance(const String& encoding, int id) {
    return new OVSCIMInstance(this, encoding, id);
}


OVSCIMInstance::OVSCIMInstance(OVSCIMFactory *factory, const String& encoding, int id )
	: IMEngineInstanceBase( factory, encoding, id ) {
    imp.initialize(&dict, &srv, "/tmp/");
    cxt=imp.newContext();
    cxt->start(&buf, &candi, &srv);
}

OVSCIMInstance::~OVSCIMInstance() {
}

bool OVSCIMInstance::process_key_event(const KeyEvent& key) {
    // an OpenOffice workaround, code from SCIM-chewing
	if (key.is_key_release()) return true;

    int c=key.get_ascii_code();
    
    // unmasked key codes (mask = SCIM_KEY_ShiftMask, _ControlMask, etc.)
    if (key.mask==0) {             
		switch ( key.code ) {
			case SCIM_KEY_Shift_L:
			case SCIM_KEY_Shift_R:
			case SCIM_KEY_Control_L:
			case SCIM_KEY_Control_R:
			case SCIM_KEY_Alt_L:
			case SCIM_KEY_Alt_R:
			case SCIM_KEY_Left:
			case SCIM_KEY_Right:
			case SCIM_KEY_Up:
			case SCIM_KEY_Down:
			case SCIM_KEY_Delete:
			case SCIM_KEY_Home:
			case SCIM_KEY_End:
			case SCIM_KEY_Tab:
                return false;
                
			case SCIM_KEY_BackSpace:
			     c=ovkBackspace;
			     break;
			case SCIM_KEY_Escape:
			     c=ovkEsc;
			     break;
			case SCIM_KEY_space:
			     c=32;
			     break;
			case SCIM_KEY_Return:
			     c=ovkReturn;
			     break;
			default:
			     break;
        }
    }
    
    DummyKeyCode kc(c);
    if (!cxt->keyEvent(&kc, &buf, &candi, &srv)) return false;

    if (candi.onscreen) {
        WideString cs=utf8_mbstowcs(candi.buf);
        update_aux_string(cs); 
        show_aux_string();
    }
    else {
        update_aux_string(WideString()); 
        hide_aux_string();
    }
    
    if (buf.state==1) {
        WideString bs=utf8_mbstowcs(buf.buf);
        commit_string(bs);              
        buf.clear();
    }
    else {
        if (buf.isEmpty()) {
            update_preedit_string(WideString());
            hide_preedit_string();
        }
        else {
            WideString us=utf8_mbstowcs(buf.buf);
            update_preedit_string(us);
            show_preedit_string();
        }
    }
	return true;
}

void OVSCIMInstance::select_candidate (unsigned int index) {
    // a candidate at index is choosen
}

void OVSCIMInstance::update_lookup_table_page_size(unsigned int page_size) {
}

void OVSCIMInstance::lookup_table_page_up() {
    // "page up" of candidate list
}

void OVSCIMInstance::lookup_table_page_down() {
    // "page down" of candidate list
}

void OVSCIMInstance::reset() {
    // reset everything, incl. context properties
}

void OVSCIMInstance::focus_in() {
    // clear buffer, context::start()
}

void OVSCIMInstance::focus_out() {
    // send out remaining texts
}

