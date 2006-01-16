#!/usr/bin/perl -w

# use this Perl script to generate "-lookup.cin" cin files

use strict;
use utf8;

while (<>) {
    last if (/%chardef/);
    print;
}

print;
while (<>) {
    chomp;
    last if (/%chardef/);
    my @l = split;
    ($a, $b) = (shift (@l), shift(@l));
    print join (" ", $b, $a, @l), "\n";
}
print;
print "\n";

while (<>) {
    print;
}

    