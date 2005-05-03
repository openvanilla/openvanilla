#!/usr/bin/perl

use Lingua::Wordnet;
use Lingua::Wordnet::Analysis;
use strict;

my $wn = new Lingua::Wordnet;
my $analysis = new Lingua::Wordnet::Analysis;

my $word1 = $wn->lookup_synset("building","n",1);
my $word2 = $wn->lookup_synset("house","n",1);

my @array1 = $analysis->traverse("hyponyms",$word1);
my @array2 = $analysis->traverse("hyponyms",$word2);

foreach my $synset ($analysis->intersection(\@array1, \@array2)) {
    print $synset->words, "\n";
}

