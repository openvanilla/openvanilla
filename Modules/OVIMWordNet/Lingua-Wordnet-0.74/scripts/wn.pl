#!/usr/bin/perl

use Lingua::Wordnet;
use strict;

my $wn = new Lingua::Wordnet;
my $word;
my $pos;


while (1) {
	print "Enter a word: ";
	chomp($word = <STDIN>);
	print "Enter a part-of-speech: ";
	chomp($pos  = <STDIN>);

	my @synsets;
	if ($pos) {
		@synsets = $wn->lookup_synset($word,$pos);
	} else {
		@synsets = $wn->lookup_synset_all($word);
	}

	unless (@synsets) { print "No matches found.\n\n"; next; }

	foreach my $synset (@synsets) {
		print "words: ", $synset->words, "\n";
		print "offset: ", $synset->offset, "\n";
		print "pos: ", $synset->pos, "\n";
		print "gloss: ", $synset->gloss, "\n";
		print "frames: ", $synset->frames, "\n\n";

	}
}

