#!/usr/bin/perl -w -CO
use strict;
use utf8;
use Encode;

my %char2wordHash;
my %word2freqHash;
my %word2charHash;
my $fn_tsi;

$fn_tsi = $ARGV[0] if defined($ARGV[0]);
$fn_tsi = "tsi.src" unless defined($ARGV[0]);
shift @ARGV;
die "USAGE: $0 tsi.src 1.cin 2.cin ..." if (!@ARGV);
print "begin;\n";

# reading tsi.src
open (HNDL, $fn_tsi) or die $!;
while(<HNDL>) {
    chomp;
    if (/#?\s*(\S+)\s+(\d+)\s+(.+)/) {
        my ($p, $f, $s)=(decode("big5-hkscs", $1), $2, decode("big5-hkscs", $3));

        $word2freqHash{$p} = $f unless exists $word2freqHash{$p};
	{
	    my @chars = split (//,$p);
	    $word2charHash{$p} = \@chars;
	}
    }
}
close HNDL;

# reading xxx.cin
for my $fn_cin (@ARGV) {
    open (HNDL, "<:utf8",$fn_cin) or die $!;
    my $table_prefix; # grep ename, for sqlite
    my %keymap; # a for X, b for Y, .... z for Z...
    my %charmap; # abc for XY..etc
    my $keyname_sec = 0;
    my $chardef_sec = 0;
    while(<HNDL>) {
	chomp;
	if ($_ =~ m/^%ename\s+([^\s]+)/) {
	    $table_prefix = $1;
	    printf "CREATE TABLE %s_char2word_table(characters TEXT, wordID INTEGER);\n",
		$table_prefix;
	    printf "CREATE INDEX %s_index_characters ON %s_char2word_table(characters);\n",
		$table_prefix, $table_prefix;
	}
	$keyname_sec = 1 if ($_ =~ m/^%keyname\s+begin/);
	$keyname_sec = 0 if ($_ =~ m/^%keyname\s+end/);
	$chardef_sec = 1 if ($_ =~ m/^%chardef\s+begin/);
	$chardef_sec = 0 if ($_ =~ m/^%chardef\s+end/);
	if ($keyname_sec and $_ =~ m/^(\S)\s+(\S+)/) {
	    $keymap{$1} = $2;
	}
	if ($chardef_sec and $_ =~ m/^(\S+)\s+(\S+)/) {
	    push @{$charmap{$2}}, $1; # NOTE: reverse...
	}
    }
    close HNDL;
    my $idCounter = 0;
    my @words = keys(%word2freqHash);
    @words = sort @words;
    for(@words) {
	my $word = $_;
	my $freq = $word2freqHash{$word};
#	printf "insert into word_table values(%d, '%s') ON CONFLICT IGNORE;\n",
#	    $idCounter, sprintf("%s", $word);
#	printf "insert into generic_freq_table values(%d, %d) ON CONFLICT IGNORE;\n",
#	    $idCounter, $freq;

	my $currentCharsRef = $word2charHash{$word};
	my $currentKeyCode = "";
	my @temp;
	for(@{$currentCharsRef}) {
	    my $k;
	    if (defined(@{$charmap{$_}})) {
		$k = join "，", @{$charmap{$_}};
	    }
	    push @temp, $k if defined($k);
	}
	if (@temp) {
	    $currentKeyCode = "[".join ("] [",@temp). "]";
	    my @syl=produce($currentKeyCode);
	    foreach (@syl) {
		my @currentCharCodes = split //,$_;
		$currentKeyCode = "";
		for (@currentCharCodes) {
		    $currentKeyCode .= $keymap{$_} if defined($keymap{$_});
		    $currentKeyCode .= $_ if $_ eq "\t";
		}
		$currentKeyCode =~ s/\s*$//g;
		$currentKeyCode =~ s/'/''/g;
		printf "insert into %s_char2word_table values('%s', %d);\n",
		    $table_prefix, sprintf("%s", $currentKeyCode), $idCounter;
	    }
	}
#		foreach (@charmaps) { # "abcd", then "defc"
#		    my @currentCharCodes = split //,$_; # a, b, c, d ..
#		    for (@currentCharCodes) {
##			$currentKeyCode .= $keymap{$_};
#		    }
#		    $currentKeyCode .= "\t";
#		}
#	    if (defined(@{$charmap{$_}})) {
#		my @charmaps = @{$charmap{$_}}; # {abcd, defc}
#		foreach (@charmaps) { # "abcd", then "defc"
#		    my @currentCharCodes = split //,$charmap{$_}; 
#		    for (@currentCharCodes) {
#			$currentKeyCode .= $keymap{$_};
#		    }
#		    $currentKeyCode .= "\t";
#		}
#	    }
#	}
#	$currentKeyCode =~ s/\s*$//g;
#	printf "insert into %s_char2word_table values('%s', %d);\n",
#	    $table_prefix, sprintf("%s", $currentKeyCode), $idCounter;
	    
	$idCounter = $idCounter + 1;
    }
}

print "commit;\n";

# this makes the product of homophones (e.g. [A,B] x [C,D] x [E,F])
sub produce {
    my @s = split / /, shift;
    my @p;    
    my $x;
    for $x (@s) {
        if ($x =~ /\[(\S+)\]/) {
            push @p, [split(/，/, $1)];
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
