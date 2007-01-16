## Makefile.am -- Process this file with automake to produce Makefile.in

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src \
	-I/usr/local/include \
	-I/usr/include \
	-D__STDC_ISO_10646__ \
	-DOV_MODULEDIR=\"@OV_MODULEDIR@\" \
	-DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
	-DSCIM_CHEWING_ICON_FILE=\"@SCIM_ICONDIR@/scim-chewing.png\" 
	

#noinst_HEADERS = scim_chewing_imengine.h scim_chewing_config_entry.h

moduledir = @SCIM_MODULEDIR@/IMEngine
module_LTLIBRARIES = OVLoader-SCIM.la

OVLoader_SCIM_la_SOURCES  = OVLoader-SCIM.cpp

OVLoader_SCIM_la_CFLAGS = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@ 
OVLoader_SCIM_la_CXXFLAGS = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@ 

OVLoader_SCIM_la_LIBADD = -lltdl -lexpat
OVLoader_SCIM_la_LDFLAGS = \
	-L/usr/local/lib \
	-avoid-version \
	-rpath $(moduledir) \
	-module \
	@LIBTOOL_EXPORT_OPTIONS@ \
	@INTLLIBS@ @SCIM_LIBS@ 

