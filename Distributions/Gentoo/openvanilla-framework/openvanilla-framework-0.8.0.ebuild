# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit eutils subversion
DESCRIPTION="OpenVanilla Framework"

HOMEPAGE="http://openvanilla.org/"
SRC_URI=""
ESVN_REPO_URI="http://openvanilla.googlecode.com/svn/trunk/Framework/Headers/"
LICENSE="bsd"
SLOT="0"
KEYWORDS="~x86"
DEPEND=""

# Run-time dependencies. Must be defined to whatever this depends on to run.
# The below is valid if the same run-time depends are required to compile.
RDEPEND="${DEPEND}"

# Source directory; the dir where the sources can be found (automatically
# unpacked) inside ${WORKDIR}.  The default value for S is ${WORKDIR}/${P}
# If you don't need to change it, leave the S= line out of the ebuild
# to keep it tidy.
#S="${WORKDIR}/${P}"

src_install() {
	# You must *personally verify* that this trick doesn't install
	# anything outside of DESTDIR; do this by reading and
	# understanding the install part of the Makefiles.
	# This is the preferred way to install.
    insinto /usr/include/OpenVanilla
	doins *.h
	# When you hit a failure with emake, do not just use make. It is
	# better to fix the Makefiles to allow proper parallelization.
	# If you fail with that, use "emake -j1", it's still better than make.

	# For Makefiles that don't make proper use of DESTDIR, setting
	# prefix is often an alternative.  However if you do this, then
	# you also need to specify mandir and infodir, since they were
	# passed to ./configure as absolute paths (overriding the prefix
	# setting).
	#emake \
	#	prefix="${D}"/usr \
	#	mandir="${D}"/usr/share/man \
	#	infodir="${D}"/usr/share/info \
	#	libdir="${D}"/usr/$(get_libdir) \
	#	install || die "emake install failed"
	# Again, verify the Makefiles!  We don't want anything falling
	# outside of ${D}.

	# The portage shortcut to the above command is simply:
	#
	#einstall || die "einstall failed"
}
