
#ifdef     __cplusplus
extern   "C"   {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>

// libtap required. Grab it from
// http://jc.ngo.org.uk/trac-bin/trac.cgi/wiki/LibTap
#include <tap.h>

#define OVIMlib "../OVIMGeneric.dylib"

#define OV_DEBUG
#undef HAVE_LIBPTHREAD

int
main(void) {
  void *libh;
  _OVGetLibraryVersion_t *g;
  _OVInitializeLibrary_t *i;
  _OVGetModuleFromLibrary_t *m;

  plan_tests(5);

  ok ( libh = dlopen(OVIMlib, RTLD_LAZY), "Module loaded");

  ok (g = (_OVGetLibraryVersion_t*)dlsym(libh, "OVGetLibraryVersion"),
      "symbol OVGetLibraryVersion is there");

  ok (i = (_OVInitializeLibrary_t*)dlsym(libh, "OVInitializeLibrary"),
      "symbol OVInitializeLibrary is there");

  ok (m = (_OVGetModuleFromLibrary_t*)dlsym(libh, "OVGetModuleFromLibrary"),
      "symbol OVGetModuleFromLibrary is there");
  ok (g() == OV_VERSION, "Version is up-to-date");

  return exit_status();
}

#ifdef     __cplusplus
}
#endif
