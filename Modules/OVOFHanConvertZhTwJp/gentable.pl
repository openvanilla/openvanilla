#!/usr/bin/perl

use strict;
use IO::All;

my $uv = io("tw2jp.txt")->utf8;

my $output;
my $size = 0;
while ($_ = $uv->getline) {
    next if /#/;
    my ($tw, $jp) = map { ord $_ } split(" ", $_);
    $output .= "$tw, $jp,\n";
    $size++;
}
$output =~ s/,\n$/\n/s;

my $o = io "ZhTwToJpKanji.c";
$o->print("unsigned short ZhTwToJpKanjiTable[$size*2]={\n");
$o->print($output);
$o->print("};\n");

print "$o generated\n";
