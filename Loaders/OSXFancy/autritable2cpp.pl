#!/usr/bin/perl -CS

use strict;
use utf8;

my %hash;

while(<>)
{
	my @data=split /\s+/;
	
	my ($a, $b)=(ord $data[0], ord $data[1]);

	if (($a eq $b) || ($a > 0xffff) || ($b > 0xffff)) { next; }
	
	$hash{$a}=$b;	
}

print scalar keys %hash, "\n";

my $c=0;
for my $x (sort { $a <=> $b } keys %hash)
{
	printf ("0x%04x,0x%04x, ", $x, $hash{$x});
	if ($c++ == 4) { print "\n"; $c=0; }
#	printf ("%d %d %s -> %s\n", $x, $hash{$x}, chr $x, chr $hash{$x});
}

	