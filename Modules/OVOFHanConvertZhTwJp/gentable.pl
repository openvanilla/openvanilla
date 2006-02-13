#!/usr/bin/perl

use strict;
use IO::All;

my $uv = io("UniVariants");

my $output;
my $size;
while ($_ = $uv->getline) {
    next if /#/;
    my ($char, @vars) = split(" ", $_);
    $output .= "0x${char}, 0x$vars[0],\n";
    $size++;
}

$output =~ s/,\n$/\n/s;
print "unsigned short ZhTwToJpKanjiTable[$size*2]={\n";
print $output;
print "};\n";
