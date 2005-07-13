#!/usr/bin/perl -w -CO
use strict;
use utf8;
use Encode;
print "begin;\n";

my %char2wordHash;
my %word2freqHash;
while(<>) {
    chomp;
    if (/#?\s*(\S+)\s+(\d+)\s+(.+)/) {
        my ($p, $f, $s)=(decode("big5-hkscs", $1), $2, decode("big5-hkscs", $3));

        # deal with irregularities in tsi.src first
        my @syl;
        if (length $p==1 && $s=~/\s+/) {
            $s =~ s/ /,/g;
            @syl=produce("[$s]");
        }
        else 
        {
            @syl=produce($s);
        }

        $word2freqHash{$p} = $f unless exists $word2freqHash{$p};

        for my $x (@syl) {
            $x =~ s/ /\t/g;
            if (exists $char2wordHash{$p})
            {
                my $charsRef = $char2wordHash{$p};
                push @{$charsRef}, $x;
                $char2wordHash{$p} = $charsRef;
            }
            else
            {
                my @chars = ($x);
                $char2wordHash{$p} = \@chars;
            }
        }
    }
}

my $idCounter = 0;
my @words = keys(%word2freqHash);
@words = sort @words;
for(@words) {
    my $word = $_;
    my $freq = $word2freqHash{$word};
    printf "insert into word_table values(%d, '%s');\n",
        $idCounter, sprintf("%s", $word);
    printf "insert into generic_freq_table values(%d, %d);\n",
        $idCounter, $freq;

    my $currentCharsRef = $char2wordHash{$word};
    for(@{$currentCharsRef}) {
        my $currentChar = $_;
        printf "insert into Phonetic_char2word_table values('%s', %d);\n",
            sprintf("%s", $currentChar), $idCounter;
    }   
        
    $idCounter = $idCounter + 1;
}

print "commit;\n";

# this makes the product of homophones (e.g. [A,B] x [C,D] x [E,F])
sub produce {
    my @s = split / /, shift;
    my @p;    
    my $x;
    for $x (@s) {
        if ($x =~ /\[(\S+)\]/) {
            push @p, [split(/,/, $1)];
        }
        else {
            push @p, [$x];
        }
    }
    
    my @r;
    for $x (reverse @p) {
        if (!@r) {
            for my $y (@$x) {
                push @r, $y;
            }
        }
        else {
            my @rr;
            for my $y (@$x) {
                for my $z (@r) {
                    push @rr, "$y\t$z";
                }
            }
            @r=@rr;
        }
    }
    
    @r;
}