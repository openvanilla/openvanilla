#define OV_DEBUG

#include <EXTERN.h>
#include <perl.h>
#include <string.h>


#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

OVService *currentServiceInstance = NULL;

static PerlInterpreter *my_perl;

static void
PerlOVFilterProcess(const char *str, char *ret) {
  dSP;
  STRLEN n_a;

  ENTER;
  SAVETMPS;
  PUSHMARK(SP);
  XPUSHs(sv_2mortal(newSVpvn(str, strlen(str))));
  PUTBACK;

  murmur("Before call_pv");

  call_pv("OV::OutputFilter::process", G_SCALAR);

  SPAGAIN;

  sprintf(ret,"%s",POPpx);

  PUTBACK;
  FREETMPS;
  LEAVE;
}

class OVOFPerlBasedFilter : public OVOutputFilter {
public:
  OVOFPerlBasedFilter() {
    stringBuffer = (char*)malloc((size_t)1024);
  }

  virtual int initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath) {
    return 1;
  }

  virtual const char *identifier() { return "OVOFPerlBasedFilter"; }

  virtual const char *process (const char *src, OVService *srv) {
    murmur("OVOF Proccess: %s", src);
    PerlOVFilterProcess(src, stringBuffer);
    murmur("OVOF Proccessed: %s", stringBuffer);
    return stringBuffer;

   /*
      perl_destruct(my_perl);
      perl_free(my_perl);
      PERL_SYS_TERM();
    */

  }

protected:
  char *stringBuffer;
};

extern "C" unsigned int OVGetLibraryVersion() {
  return OV_VERSION;
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
  return (idx==0) ? new OVOFPerlBasedFilter : NULL;
}

// initialize Perl
extern "C" int OVInitializeLibrary(OVService* s, const char* libPath) {
  int my_argc = 1;
  char *my_argv[2] = { "", "" };

  murmur("OVInitializeLibrary");

  my_argv[1] = (char*)malloc((size_t)1024);
  sprintf(my_argv[1], "%sOVOFPerlBased%sov_perl_filter.pl", libPath, s->pathSeparator());

  PERL_SYS_INIT(&my_argc,&my_argv);

  my_perl = perl_alloc();
  perl_construct(my_perl);

  perl_parse(my_perl, NULL, my_argc, my_argv, NULL);
  PL_exit_flags |= PERL_EXIT_DESTRUCT_END;

  return 1;
}
