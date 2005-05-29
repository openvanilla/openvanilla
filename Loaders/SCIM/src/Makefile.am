## Makefile.am -- Process this file with automake to produce Makefile.in

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I../../../Headers/ \
	-DSCIMOV_MODULEDIR=\"@SCIMOV_MODULEDIR@\" \
	-DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
	-DSCIM_CHEWING_ICON_FILE=\"@SCIM_ICONDIR@/scim-chewing.png\" 
	

#noinst_HEADERS = scim_chewing_imengine.h scim_chewing_config_entry.h

moduledir = @SCIM_MODULEDIR@/IMEngine
module_LTLIBRARIES = scim_ov.la

scim_ov_la_SOURCES  = SCIMOV.cpp

scim_ov_la_CFLAGS = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@
scim_ov_la_CXXFLAGS = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@

scim_ov_la_LIBADD = -lltdl
scim_ov_la_LDFLAGS = \
	-avoid-version \
	-rpath $(moduledir) \
	-module \
	@LIBTOOL_EXPORT_OPTIONS@ \
	@INTLLIBS@ @SCIM_LIBS@ \
	@CHEWING_LIBS@

