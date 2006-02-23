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
$o->println("#include <stdlib.h>");
$o->println("const size_t ZhTwToJpKanjiTableSize = $size*2;");
$o->println("unsigned short ZhTwToJpKanjiTable[$size*2]={");
$o->print($output);
$o->println("};");

print "$o generated\n";
