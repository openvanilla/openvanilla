#!/usr/bin/perl -w -CO
use strict;
use utf8;
use Encode;

my %freqHash;
my %word2IdHash;
my $fn_tsi;

$fn_tsi = $ARGV[0] if defined($ARGV[0]);
$fn_tsi = "tsi_punctuation.src" unless defined($ARGV[0]);
print "begin;\n";

# reading tsi_punctuation.src
open (HNDL, $fn_tsi) or die $!;

my $idCounter = 0;
while(<HNDL>) {
    chomp;
    if (/#?\s*(\S+)\s+(\d+)\s+(.+)/) {
        my ($w, $f, $c)=(decode("utf8", $1), $2, decode("utf8", $3));

	unless (exists($word2IdHash{$w})) { 
		$word2IdHash{$w} = $idCounter;
		printf "insert into word_table values(%d, '%s');\n",
		$idCounter, sprintf("%s", $w);
		$idCounter++;
        }

	my $currentWordId = $word2IdHash{$w};
	$c =~ s/ /\t/g;

        printf "insert into BoPoMoFo_char2word_table values('%s', %d, %f);\n",
            sprintf("%s", $c), $currentWordId, $f;
    }
}
close HNDL;

print "commit;\n";
