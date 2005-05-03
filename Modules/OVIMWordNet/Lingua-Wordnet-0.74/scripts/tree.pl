#!/usr/bin/perl

use Lingua::Wordnet;
use strict;

my $wn = new Lingua::Wordnet;
my $word;
my $pos;
my $sense;


    print "Enter a word: ";
    chomp($word = <STDIN>);
    print "Enter a part-of-speech: ";
    chomp($pos  = <STDIN>);
    print "Enter the sense number: ";
    chomp($sense = <STDIN>);
 
    my $i = 0;
    my $synset = $wn->lookup_synset($word,$pos,$sense);
    print "->", $synset->words, "\n";
    while ($synset = ($synset->hypernyms)[0]) {
        $i++;  
        print " "x$i, "->", $synset->words, "\n";
     }


