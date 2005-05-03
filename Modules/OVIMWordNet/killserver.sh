#!/bin/sh
ps auxwww | grep Perl | grep OVSocketServer
ps auxwww | grep Perl | grep OVSocketServer | perl -n -e 'split; print "kill $_[1]\n"'
ps auxwww | grep Perl | grep OVSocketServer | perl -n -e 'split; system "kill $_[1]"'
