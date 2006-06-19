
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>

#define OV_DEBUG

static int tests = 0;

static char *OVIMlib = "../OVIMPhonetic.dylib";

inline static void report(const char* state, const char* format,...) {
  va_list args;
  fprintf (stdout, "%3d : %5s - ", tests++, state);
  va_start (args, format);
  vfprintf (stdout, format, args);
  va_end (args);
  fprintf (stdout, "\n");
}

static int test_loadlib (char *lib) {
  void *libh;
  char *reportstr;
  _OVGetLibraryVersion_t *g;
  _OVInitializeLibrary_t *i;
  _OVGetModuleFromLibrary_t *m;

  printf("==> loadlib tests start\n\n");

  libh = dlopen( lib , RTLD_LAZY);

  reportstr = "load library %s";
  if (!libh) {
    report("FAIL", reportstr, OVIMlib );
    return 0;
  } else {
    report("OK", reportstr, OVIMlib );
  }

  g = (_OVGetLibraryVersion_t*)dlsym(libh, "OVGetLibraryVersion");
  i = (_OVInitializeLibrary_t*)dlsym(libh, "OVInitializeLibrary");
  m = (_OVGetModuleFromLibrary_t*)dlsym(libh, "OVGetModuleFromLibrary");

  reportstr = "g,i,m symbols are ready.";
  if (!g || !i || !m) {
    report("FAIL", reportstr);
    return 0;
  } else {
    report("OK", reportstr);
  }

  reportstr = "loadlib(): Version up-to-date (%s)";
  if (g() < OV_VERSION) {
    report("FAIL", reportstr, lib);
    return 0;
  } else {
    report("OK", reportstr, lib);
  }

  printf("\n\n==> loadlib tests end.\n");
  return 1;
}


int main(void) {

  printf("\n"
         "---------------\n"
         "OVTester Starts\n"
         "---------------\n\n");

  test_loadlib(OVIMlib);

  printf("\n\n"
         "-------------\n"
         "OVTester Ends\n"
         "-------------\n");
}

