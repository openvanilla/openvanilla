#!/usr/bin/perl -w -CO
use strict;
use utf8;
use Encode;

my %word2charsHash;
my %word2freqHash;
my $fn_tsi;

$fn_tsi = $ARGV[0] if defined($ARGV[0]);
$fn_tsi = "tsi_punctuation.src" unless defined($ARGV[0]);
print "begin;\n";

# reading tsi_punctuation.src
open (HNDL, $fn_tsi) or die $!;
while(<HNDL>) {
    chomp;
    if (/#?\s*(\S+)\s+(\d+)\s+(.+)/) {
        my ($w, $f, $c)=(decode("utf8", $1), $2, decode("utf8", $3));

		if !(exists $word2freqHash{$w} && $f <= $word2freqHash{$w})
        	$word2freqHash{$w} = $f;

	$c =~ s/ /\t/g;
	push @{$word2charsHash{$w}}, $c;
    }
}
close HNDL;

my $idCounter = 0;
my @words = keys(%word2charsHash);
@words = sort @words;
for(@words) {
    my $word = $_;
    my $freq = $word2freqHash{$word};
    printf "insert into word_table values(%d, '%s');\n",
        $idCounter, sprintf("%s", $word);
    printf "insert into generic_freq_table values(%d, %d);\n",
        $idCounter, $freq;

    my $currentCharsRef = $word2charsHash{$word};
    for(@{$currentCharsRef}) {
        my $currentChar = $_;
	$currentChar =~ s/\'/\'\'/g;
        printf "insert into BoPoMoFo_char2word_table values('%s', %d);\n",
            sprintf("%s", $currentChar), $idCounter;
    }   
        
    $idCounter = $idCounter + 1;
}

print "commit;\n";
