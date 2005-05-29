AC_INIT(scim-openvanilla, 0.1.0, openvanilla.org)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/OVLoader-SCIM.cpp])

cflags_set=${CFLAGS+set}
cppflags_set=${CPPFLAGS+set}

AC_PROG_CXX
AC_LANG(C++)

AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_LIB_LTDL
AC_SUBST(LIBTOOL_DEPS)

# SCIM variables

SCIM_VERSION=1.0.0
PKG_CHECK_MODULES(SCIM, [scim >= $SCIM_VERSION])


# Extra args.
AC_ARG_ENABLE(debug,
              [  --enable-debug     Turn on debugging],
              [case "${enableval}" in
               yes) debug=yes ;;
               no)  debug=no ;;
               *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
              esac],[debug=no])

if test "x$debug" = "xyes"; then
  test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
  test "$cppflags_set" = set || CPPFLAGS="$CXXFLAGS -g"
  SCIM_DEBUG_FLAGS="-DENABLE_DEBUG=1"
else
  SCIM_DEBUG_FLAGS=""
fi
AC_SUBST(SCIM_DEBUG_FLAGS)


SCIM_ICONDIR=`pkg-config --variable=icondir scim`
SCIM_MODULEDIR=`pkg-config --variable=moduledir scim`
SCIM_DATADIR=`pkg-config --variable=scimdatadir scim`

if test "x$SCIM_ICONDIR" = "x"; then
  SCIM_ICONDIR=%{prefix}/share/scim/icons
fi

if test "x$SCIM_MODULEDIR" = "x"; then
   SCIM_MODULEDIR=${libdir}/scim-1.0
fi

OV_MODULEDIR="/usr/local/lib/openvanilla/"
    
AC_SUBST(SCIM_ICONDIR)
AC_SUBST(SCIM_MODULEDIR)
AC_SUBST(SCIM_DATADIR)
AC_SUBST(OV_MODULEDIR)


AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
