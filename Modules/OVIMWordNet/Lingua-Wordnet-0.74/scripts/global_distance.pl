#!/usr/bin/perl -w

BEGIN { @INC = ("../blib/lib", @INC); }
use strict;

use Lingua::Wordnet;
use Lingua::Wordnet::Analysis;

my $wn = new Lingua::Wordnet;
my $wna = new Lingua::Wordnet::Analysis;

my ($word1, $word2, $maxdist) = @ARGV;

unless ($word1 && $word2) {
	print "Usage: $0 word1 word2\n";
	exit;
}

$maxdist = 7 if (!$maxdist || $maxdist =~ /\D/);
my @syn1 = $wn->lookup_synset($word1, "n");
my @syn2 = $wn->lookup_synset($word2, "n");

# my @parents = map { $_->offset } @syn2;

my ($dist, $path) = $wna->global_distance(\@syn1, \@syn2, $maxdist);

$dist = (defined $dist) ? $dist : "undef";
print "CALCULATED DISTANCE: $dist\n";
print "Path:                $path\n" if $path;

if ($path && $dist ne "undef") {
	my @path = split(/ : /, $path);
	foreach (@path) {
		my ($offset, $type) = split(/\|/, $_);
		$type ||= "";
		my $synset = $wn->lookup_synset_offset($offset);
		print "\t$synset ($offset)";
		print " [is $type of]" if $type;
		print "\n";
	}
}
