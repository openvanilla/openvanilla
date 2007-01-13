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

#include "OVLoader-SCIM.h"

#include "../../AmphiVanilla/AVConfig.h"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

AVConfig im_config("/tmp/org.openvanilla.072.plist");

using namespace scim;

static ConfigPointer _scim_config(0);
static int scan_ov_modules();

extern "C" void scim_module_init() {
   lt_dlinit();
   lt_dlsetsearchpath(OV_MODULEDIR);

   const char *homedir = getenv("HOME");
   string fn = string(homedir) + "/" + ".openvanilla";
   string cmd = "mkdir -p " + fn;
   system(cmd.c_str());

   // string x;
   // x= "echo " + fn + " > /tmp/output.txt";
   // system(x.c_str());
   fn += "/org.openvanilla.072.plist";
   im_config.setFilename(fn);

   if (!im_config.load()) {
       fprintf(stderr, "OpenVanilla: No preset config file\n");
   }
}

extern "C" void scim_module_exit() {
    _scim_config.reset();
    lt_dlexit();
}

extern "C" unsigned int scim_imengine_module_init(const ConfigPointer& c) {
    _scim_config=c;
    return scan_ov_modules();
}

extern "C" IMEngineFactoryPointer scim_imengine_module_create_factory(uint32 e)
{
    if( e < mod_vector.size() )
       return new OVSCIMFactory( mod_vector[e], _scim_config );
    return IMEngineFactoryPointer(0);
}

static OVLibrary* open_module(const char* modname){
   OVLibrary* mod = new OVLibrary();
   mod->handle = lt_dlopen(modname);
   if(mod->handle == NULL){
      SCIM_DEBUG_IMENGINE(2) << "dlopen " << modname << " failed\n";
      goto OPEN_FAILED;
   }
   mod->getModule = (TypeGetModule)lt_dlsym( mod->handle, 
                                             "OVGetModuleFromLibrary" );
   mod->getLibVersion = (TypeGetLibVersion)lt_dlsym( mod->handle, 
                                             "OVGetLibraryVersion" );
   mod->initLibrary = (TypeInitLibrary)lt_dlsym( mod->handle,
                                             "OVInitializeLibrary" );
   if( !mod->getModule || !mod->getLibVersion || !mod->initLibrary ){
      SCIM_DEBUG_IMENGINE(2) << "dlsym " << modname << " failed\n";
      goto OPEN_FAILED;
   }
   if( mod->getLibVersion() < OV_VERSION ){
      SCIM_DEBUG_IMENGINE(2) << modname << " " << mod->getLibVersion() << " is too old\n";
      goto OPEN_FAILED;
   }
   return mod;

OPEN_FAILED:
   delete mod;
   return NULL;
}

static int scan_ov_modules(){
   DIR* dir = opendir(OV_MODULEDIR);
   DummyService srv;
   if(dir){
      struct dirent *d_ent;
      while( (d_ent = readdir(dir)) != NULL ){
         if( strstr( d_ent->d_name, ".so" ) ){
            SCIM_DEBUG_IMENGINE(2) << "Load OV module: " << d_ent->d_name << "\n";
            OVLibrary* mod = open_module(d_ent->d_name);
            if(mod){
               OVModule* m;
               mod->initLibrary(&srv, OV_MODULEDIR);
               for(int i=0; m = mod->getModule(i); i++)
                  mod_vector.push_back(m);
               delete mod;
            }
         }
      }
      closedir(dir);
   }
   return mod_vector.size();
}

// OVSCIMFactory

OVSCIMFactory::OVSCIMFactory(OVModule *i, const ConfigPointer& config) {
    SCIM_DEBUG_IMENGINE(2) << "SCIM-OpenVanilla IMFactory init! id=" << i->identifier() << "\n";
    set_languages("zh_TW,zh_HK,zh_SG");
    
    im = dynamic_cast<OVInputMethod*>(i);
    if(!im)
       SCIM_DEBUG_IMENGINE(2) << "dynamic_cast OVInputMethod* failed\n";
    DummyService srv;

    AVDictionary mainDict = im_config.dictionary();

    const char *idf = im->identifier();
    if (!mainDict.dictionaryExists(idf)) mainDict.createDictionaryForKey(idf);
    AVDictionary imDict = mainDict.getDictionary(idf);

    im->initialize(&imDict, &srv, OV_MODULEDIR);
    im_config.write();
}

OVSCIMFactory::~OVSCIMFactory() {
    delete im;
}

WideString OVSCIMFactory::get_name() const {
    char idbuf[256];
    sprintf(idbuf, "OpenVanilla-%s", im->localizedName("en"));
    return utf8_mbstowcs(idbuf);
}

String OVSCIMFactory::get_uuid() const {
    char hash[256];
    // we just generate some random stuff
    sprintf(hash, "d1f40e24-cdb7-11d9-9359-02061b%02x%02x%02x",
             strlen(im->identifier()), strlen(im->localizedName("en")),
             strlen(im->localizedName("zh_TW"))
    );
    return String(hash);
}

String OVSCIMFactory::get_icon_file() const {
    return String(SCIM_ICONDIR "/rawcode.png");
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
    return new OVSCIMInstance(im->newContext(), this, encoding, id);
}


OVSCIMInstance::OVSCIMInstance(OVInputMethodContext *c, OVSCIMFactory *factory, const String& encoding, int id ) : DIMEInstance(factory,encoding, id), buf(this), candi(this)
{
    im=factory->im;
    cxt=c;
    cxt->start(&buf, &candi, &srv);
}

OVSCIMInstance::~OVSCIMInstance() {
    delete cxt;
}

bool OVSCIMInstance::process_key_event(const KeyEvent& key) {
    // an OpenOffice workaround, code from SCIM-chewing
    if (key.is_key_release()) return true;

    DummyKeyCode kc;
    
    int c=key.get_ascii_code();

    if (key.mask & SCIM_KEY_ShiftMask) kc.setShift(1); 
    if (key.mask & SCIM_KEY_CapsLockMask) kc.setCapslock(1);
    if (key.mask & SCIM_KEY_ControlMask) kc.setCtrl(1);
    if (key.mask & SCIM_KEY_AltMask) kc.setAlt(1);
    
    switch (key.code) {
        case SCIM_KEY_Shift_L:
        case SCIM_KEY_Control_L:
        case SCIM_KEY_Alt_L:
        case SCIM_KEY_Left:      c=ovkLeft; break;
        case SCIM_KEY_Shift_R:
        case SCIM_KEY_Control_R:
        case SCIM_KEY_Alt_R:
        case SCIM_KEY_Right:     c=ovkRight; break;
        case SCIM_KEY_Up:        c=ovkUp; break;
        case SCIM_KEY_Down:      c=ovkDown; break;
        case SCIM_KEY_Delete:    c=ovkDelete; break;
        case SCIM_KEY_Home:      c=ovkHome; break;
        case SCIM_KEY_End:       c=ovkEnd; break;
        case SCIM_KEY_Tab:       c=ovkTab; break;            
        case SCIM_KEY_BackSpace: c=ovkBackspace; break;
        case SCIM_KEY_Escape:    c=ovkEsc; break;
        case SCIM_KEY_space:     c=ovkSpace; break;
        case SCIM_KEY_Return:    c=ovkReturn; break;
    }
    
    kc.setCode(c);
    if (!cxt->keyEvent(&kc, &buf, &candi, &srv)) return false;
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
    cxt->clear();
}

void OVSCIMInstance::focus_in() {

    if (im_config.needSync()) {
        im_config.load();
    }

    AVDictionary mainDict = im_config.dictionary();

    const char *idf = im->identifier();
    if (!mainDict.dictionaryExists(idf)) mainDict.createDictionaryForKey(idf);
    AVDictionary imDict = mainDict.getDictionary(idf);

    im->update(&imDict, &srv);
 

    // clear buffer, context::start()
    buf.clear();
    cxt->start(&buf, &candi, &srv);
    cxt->clear();
}

void OVSCIMInstance::focus_out() {
    // send out remaining texts
    buf.clear();
    cxt->clear();
}

