MODULEID=OVOFPerlBased
SOURCES=OVOFPerlBased
DATA=ov_perl_filter.pl
PERL=/usr/bin/perl5.8.6

include ../Mk/ov.module.mk

override CFLAGS=-F/Library/Frameworks/ `$(PERL) -MExtUtils::Embed -e ccopts`
override LDFLAGS=-dl `$(PERL) -MExtUtils::Embed -e ldopts`
