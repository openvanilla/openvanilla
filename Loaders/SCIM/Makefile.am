## Makefile.am -- Process this file with automake to produce Makefile.in

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
	-DSCIM_CHEWING_ICON_FILE=\"@SCIM_ICONDIR@/scim-chewing.png\"

noinst_HEADERS = scim_chewing_imengine.h scim_chewing_config_entry.h

moduledir = @SCIM_MODULEDIR@/IMEngine
module_LTLIBRARIES = chewing.la

chewing_la_SOURCES  = scim_chewing_imengine.cpp

chewing_la_CFLAGS = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@
chewing_la_CXXFLAGS = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@

chewing_la_LDFLAGS = \
	-avoid-version \
	-rpath $(moduledir) \
	-module \
	@LIBTOOL_EXPORT_OPTIONS@ \
	@INTLLIBS@ @SCIM_LIBS@ \
	@CHEWING_LIBS@

if SCIM_BUILD_SETUP
SETUP_MODULE = chewing-imengine-setup.la
endif

setupmoduledir = @SCIM_MODULEDIR@/SetupUI
setupmodule_LTLIBRARIES = $(SETUP_MODULE)

chewing_imengine_setup_la_SOURCES = scim_chewing_imengine_setup.cpp

chewing_imengine_setup_la_CFLAGS = \
	@SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@

chewing_imengine_setup_la_CXXFLAGS = \
	@SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@

chewing_imengine_setup_la_LDFLAGS = \
	-avoid-version \
	-rpath $(setupmoduledir) \
	-module \
	@LIBTOOL_EXPORT_OPTIONS@ \
	@SCIM_GTKUTILS_LIBS@ \
	@INTLLIBS@ \
	@SCIM_LIBS@
