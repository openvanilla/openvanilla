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

moduledir = $(libdir)/@SCIM_MODULEDIR@/IMEngine
module_LTLIBRARIES = OVLoader-SCIM.la

OVLoader_SCIM_la_SOURCES  = AVDictionary.cpp AVConfig.cpp OVLoader-SCIM.cpp AVConfig.h AVDictionary.h DummyLoader.h OVLoader-SCIM.h

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

EXTRA_DIST= OpenVanilla/OVLibrary.h OpenVanilla/OVUtility.h OpenVanilla/OpenVanilla.h
