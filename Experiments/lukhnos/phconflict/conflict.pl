#!/usr/bin/perl -w
use utf8;
use IO::All;
use List::Compare;
use Quantum::Superpositions;

$, = " ";

my @stdkeys =  map {(split(/\s+/,$_))[0]} io(shift)->getlines;
my @etkeys  =  map {(split(/\s+/,$_))[0]} io(shift)->getlines;

# my $lc = List::Compare->new(\@stdkeys, \@etkeys);
# print scalar(@stdkeys) . "std keys\n";
# print scalar(@etkeys) . "etkeys\n";
# my @ckey = $lc->get_intersection;
# print scalar(@ckey) . "conflicts\n";

$|=1;
for(@stdkeys) {
    print ".";
    push @ckey,$_    if($_ eq any(@etkeys));
}

print "conflict keys = ", @ckey;
