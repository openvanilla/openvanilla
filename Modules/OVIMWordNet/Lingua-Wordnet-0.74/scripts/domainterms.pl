#!/usr/bin/perl -w

BEGIN { @INC = ("../blib/lib", @INC); }

use strict;

use Lingua::Wordnet;

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
if ($dictdir && !-d $dictdir) {
	print "No such directory: $dictdir\n";
	&usage;
}
if (!$word) {
	&usage;
}

my $wn = new Lingua::Wordnet($dictdir);

print qq|Getting terms of domain '$word'\n|;
&get_domainterms($word);


sub get_domainterms {
	my $word = shift || "music";
	my @synsets = $wn->lookup_synset($word, "n");

	foreach (@synsets) {
		print "Synset: $_\n";

#		my @domains = $_->all_domainterms;
#		print qq|\tAll Domain Terms:\n|;
#		foreach (@domains) {
#			print "\t\t$_\n";
#		}
		@domains = $_->category_domainterms;
		print qq|\tTerms in Category Domain:\n|;
		foreach (@domains) {
			print "\t\t$_\n";
		}
		@domains = $_->region_domainterms;
		print qq|\tTerms in Region Domains:\n|;
		foreach (@domains) {
			print "\t\t$_\n";
		}
		@domains = $_->usage_domainterms;
		print qq|\tTerms in Usage Domains:\n|;
		foreach (@domains) {
			print "\t\t$_\n";
		}
		print "\n";
	}
}

sub usage {
	print "Usage: $0 <word> [DICTDIR=/path/to/dictdir]\n";
	exit;
}
