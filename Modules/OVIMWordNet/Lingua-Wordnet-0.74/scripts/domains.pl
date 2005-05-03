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
	&usage
}

my $wn = new Lingua::Wordnet($dictdir);

print qq|Getting domains of word '$word'\n|;
&get_domains($word);


sub get_domains {
	my $word = shift || "piano";
	my @synsets = $wn->lookup_synset($word, "n");

	foreach (@synsets) {
		print "Synset: $_\n";

#		my @domains = $_->all_domains;
#		print qq|\tAll Domains:\n|;
#		foreach (@domains) {
#			print "\t\t$_\n";
#		}
		my @domains = $_->category_domains;
		print qq|\tCategory Domains:\n|;
		foreach (@domains) {
			print "\t\t$_\n";
		}
		@domains = $_->region_domains;
		print qq|\tRegion Domains:\n|;
		foreach (@domains) {
			print "\t\t$_\n";
		}
		@domains = $_->usage_domains;
		print qq|\tUsage Domains:\n|;
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
