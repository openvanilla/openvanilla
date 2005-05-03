#!/usr/bin/perl -w

BEGIN { @INC = ("../blib/lib", @INC); }

use strict;

use Lingua::Wordnet;
use Data::Dumper;

my $dictdir;
my $word;

foreach (@ARGV) {
	if ($_ =~ /DICTDIR=(.*)/) {
		$dictdir = $1;
	}
	else {
		$word = $_;
	}
}

my $wn = new Lingua::Wordnet($dictdir);

if (!$word) {
	print "Usage: $0 <word> [DICTDIR=/path/to/dictdir]\n";
	exit;
}

print qq|Getting familarit(ies) of word '$word'\n|;
foreach (qw(n v a)) {
	if (my $fam = $wn->familiarity($word, $_)) {
		print qq|\tFamiliarity in '$_': $fam\n|;
	}
}

