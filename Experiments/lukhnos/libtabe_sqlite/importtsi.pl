#!/usr/bin/perl -w -CO
use strict;
use utf8;
use Encode;
use DBI;
use YAML;
my $dbh=DBI->connect("dbi:SQLite:dbname=tsi.db", "", "");
$dbh->do("begin;");
my $sth=$dbh->prepare("insert into tsi values (?,?,?,?);");

my %symbolmap=qw(ㄅ 1 ㄆ q ㄇ a ㄈ z ㄉ 2 ㄊ w ㄋ s ㄌ x ㄍ e ㄎ d ㄏ c ㄐ r ㄑ f ㄒ v ㄓ 5 ㄔ t ㄕ g ㄖ b ㄗ y ㄘ h ㄙ n ㄧ u ㄨ j ㄩ m ㄚ 8 ㄛ i ㄜ k ㄝ X ㄞ 9 ㄟ o ㄠ l ㄡ . ㄢ 0 ㄣ p ㄤ ; ㄥ / ㄦ =);
$symbolmap{"ㄝ"}=",";

my $trickchange=join("|", keys %symbolmap);

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

        for my $x (@syl) {
            my ($ph, $tn)=splittone($x);
            $ph =~ s/($trickchange)/$symbolmap{$1}/g;
            $sth->execute($p, $f, $ph, $tn);
        }
    }
}

$dbh->do("commit;");

# this splits the tones
sub splittone {
    my @t=split(/ /, shift);
    my (@reals, @realt);
    for my $s (@t) {
        if ($s =~ /(\D+)(\d?)/) {
            push @reals, $1;
            if ($2) { push @realt, $2; } else { push @realt, "0"; }
        }
    }
    
    (join(" ", @reals), join(" ", @realt));
}

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
                    push @rr, "$y $z";
                }
            }
            @r=@rr;
        }
    }
    
    @r;
}