#!/usr/bin/perl -w -CO
use strict;
use utf8;
use Encode;
print "begin;\n";

my %word2charsHash;
my %word2freqHash;
while(<>) {
    chomp;
    if (/#?\s*(\S+)\s+(\d+)\s+(.+)/) {
        my ($p, $f, $s)=(decode("big5-hkscs", $1), $2, decode("big5-hkscs", $3));

        $word2freqHash{$p} = $f unless exists $word2freqHash{$p};

	$s =~ s/ /\t/g;
	push @{$word2charsHash{$p}}, $s;
    }
}

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
