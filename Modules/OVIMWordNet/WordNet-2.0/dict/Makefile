SHELL=/bin/sh

# Makefile for WordNet 2.0 database directory "dict"

WN_ROOT = /usr/local/WordNet-2.0
WN_INSTALLDIR = $(WN_ROOT)/dict

INSTALLCMD = cp
INSTALLFLAGS = -p

WN_FILES = data.noun data.verb data.adj data.adv index.noun index.verb index.adj index.adv noun.exc verb.exc adj.exc adv.exc index.sense cntlist cntlist.rev lexnames sentidx.vrb sents.vrb

all: $(WN_FILES)

install: $(WN_FILES)
	@if [ ! -d $(WN_INSTALLDIR) ] ; then \
		echo "Making directory $(WN_INSTALLDIR)" ; \
		mkdir -p $(WN_INSTALLDIR) ; \
		chmod 755 $(WN_INSTALLDIR) ; \
	fi ; 
	@echo "Installing database files in $(WN_INSTALLDIR)"
	@for file in $(WN_FILES) ; \
	 do \
		filename=$(WN_INSTALLDIR)/$$file ; \
		if [ -f $$filename ] ; then \
			echo "Cannot install $$filename: file exists" ; \
		else \
			echo "Installing $$filename" ; \
			$(INSTALLCMD) $(INSTALLFLAGS) $$file $$filename ; \
		fi ; \
	 done ;
	@echo "Done installing database files in $(WN_INSTALLDIR)"

uninstall:
	@echo "Cannot uninstall database files automatically" ; \
	echo "You must delete them from $(WN_INSTALLDIR) manually" ; \
	echo "This is dangerous if you set INSTALLCMD to 'mv'" ; \
	echo "Since this is your only copy of WordNet" ;

reallyuninstall:
	@echo "Uninstalling database files from $(WN_INSTALLDIR)"
	@for file in $(WN_FILES) ; \
	 do \
		filename=$(WN_INSTALLDIR)/$$file ; \
		if [ ! -f $$filename ] ; then \
			echo "Cannot uninstall $$filename: not present" ; \
		else \
			echo "Uninstalling $$filename" ; \
			rm -f $$filename ; \
		fi ; \
	 done ;
	@echo "Done uninstalling database files from $(WN_INSTALLDIR)"

clean:
	@rm -f *~ "#"*

cleandbfiles:
	@echo "Removing WordNet 2.0 database files from `pwd`"
	@for file in $(WN_FILES) ; \
	 do \
		if [ ! -f $$file ] ; then \
			echo "Cannot remove $$file" ; \
		else \
			echo "Removing $$file" ; \
			rm -f $$file ; \
		fi ; \
	done ;
	@echo "Done removing WordNet 2.0 database files"
