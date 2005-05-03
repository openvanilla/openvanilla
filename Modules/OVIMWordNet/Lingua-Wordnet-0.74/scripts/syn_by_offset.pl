#!/usr/bin/perl -w

use strict;

use Lingua::Wordnet;

my $wn = new Lingua::Wordnet();

my $syn = $wn->lookup_synset_offset($ARGV[0]);

print $syn;
