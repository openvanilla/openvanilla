#!/usr/bin/perl -w -CS
use strict;
use utf8;
use Encode;
my %symbolmap=qw(ㄅ 1 ㄆ q ㄇ a ㄈ z ㄉ 2 ㄊ w ㄋ s ㄌ x ㄍ e ㄎ d ㄏ c ㄐ r ㄑ f ㄒ v ㄓ 5 ㄔ t ㄕ g ㄖ b ㄗ y ㄘ h ㄙ n ㄧ u ㄨ j ㄩ m ㄚ 8 ㄛ i ㄜ k ㄝ X ㄞ 9 ㄟ o ㄠ l ㄡ . ㄢ 0 ㄣ p ㄤ ; ㄥ / ㄦ - ˊ 6 ˇ 3 ˋ 4 ˙ 7);
$symbolmap{"ㄝ"}=",";

my %std_to_phone=reverse %symbolmap;

my $trickchange=join("|", keys %std_to_phone);

while(<>) {
    chomp;
    s/($trickchange)/$std_to_phone{$1}/g;
    print;
    print "\n";
}

