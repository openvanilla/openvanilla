package OpenVanilla::Utility::Generator::Test;
use strict;
use warnings;
use base 'OpenVanilla::Utility::Generator';
use Template;
use File::Spec;

sub generate {
    my ($self, $outdir, $module_id) = @_;
    my $vars = { module_id => $module_id };
    my $tt = Template->new;
    my %output;
    my %files = $self->files;

    for (keys %files) {
        my $outfile = File::Spec->catfile($outdir, $_);
        $tt->process(\$files{$_}, $vars, $outfile );
        print "$outfile generated\n";
    }
}

sub files {
    my $self = shift;
    local $/ = undef;
    my $f = <DATA>;
    my %files = ('',split(/^__(.*)__$/m, $f));
    delete $files{''};
    return %files
}

1;

__DATA__
__OVTester.cpp__
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

#define OVIMlib "../[% module_id %].dylib"

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
__Makefile__
CPPFLAGS=-Wall -I/usr/local/include -L/usr/local/lib -ltap 
OS=$(shell uname)

ifeq "$(OS)" "FreeBSD"
CPPFLAGS+= -pthread
endif

all: OVTester
	@./OVTester

clean:
	@rm -f OVTester *~ *.o

OVTester: OVTester.cpp
	g++ $(CPPFLAGS) -o $@ $<
