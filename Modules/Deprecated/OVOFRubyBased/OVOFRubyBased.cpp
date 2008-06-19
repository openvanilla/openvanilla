#define OV_DEBUG
#include <ruby.h>
#include <string>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

static VALUE rubyFilterInstance;
static int rubyFilterSymbol;
static int processSymbol;

static VALUE OVServiceClass;
static VALUE serviceInstance;
OVService *currentServiceInstance = NULL;

typedef VALUE RBMethodFuncType(...);

static VALUE OVServiceNotify(VALUE self, VALUE string) {
	if (currentServiceInstance) {
		int i, len = RSTRING(string)->len;
		char *ptr = RSTRING(string)->ptr;
		char *buf = (char*)calloc(1, len + 1);
		for (i = 0; i < len; i++, ptr++) buf[i] = *ptr;
		buf[i] = 0;
		
		fprintf(stderr, "sending notify string (source %x, length %d): %s\n", string, len, buf);
		currentServiceInstance->notify(buf);
		
		free(buf);
	}
	else {
		fprintf(stderr, "no OVService instance available\n");
	}

	return Qnil;
}

class OVOFRubyBasedFilter : public OVOutputFilter {
public:
    OVOFRubyBasedFilter() {
		stringBuffer = NULL;
	}

    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath) {
		return 1;
    }
    
    virtual const char *identifier() { return "OVOFRubyBasedFilter"; }

    virtual const char *process (const char *src, OVService *srv) {
		currentServiceInstance = srv;
	
		VALUE rubyString = rb_str_new2(src);	
		VALUE returnString = rb_funcall(rubyFilterInstance, processSymbol, 2, rubyString, serviceInstance);
		
		int i, len = RSTRING(returnString)->len;
		char *ptr = RSTRING(returnString)->ptr;
		
		if (stringBuffer) free(stringBuffer);
		stringBuffer = (char*)calloc(1, len + 1);
		for (i = 0; i < len; i++, ptr++) stringBuffer[i] = *ptr;
		stringBuffer[i] = 0;
		
		currentServiceInstance = NULL;
		
		return stringBuffer;
    }
    
protected:
	char *stringBuffer;
};

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    return (idx==0) ? new OVOFRubyBasedFilter : NULL;
}

// initialize Ruby
extern "C" int OVInitializeLibrary(OVService* s, const char* libPath) { 
	string scriptPath;
		scriptPath = string(libPath) + string(s->pathSeparator()) + string("OVOFRubyBased") + string(s->pathSeparator()) + string("ov_ruby_filter.rb");
	fprintf(stderr, "script path = %s\n", scriptPath.c_str());

	ruby_init();
	ruby_init_loadpath();
	ruby_script("ov_ruby_filter");
	rb_require(scriptPath.c_str());
	
	// export the OVService class
	OVServiceClass = rb_define_class("OVService", rb_cObject);
	rb_define_method(OVServiceClass, "notify", (RBMethodFuncType*)OVServiceNotify, 1);
	serviceInstance = rb_class_new_instance(0, 0, rb_const_get(rb_cObject, rb_intern("OVService")));
	
	rubyFilterInstance = rb_class_new_instance(0, 0, rb_const_get(rb_cObject, rb_intern("OVOFRubyFilter")));
	rubyFilterSymbol = rb_intern("ruby_filter");
	processSymbol = rb_intern("process");
	
//	ruby_finalize();

    return 1; 
}

