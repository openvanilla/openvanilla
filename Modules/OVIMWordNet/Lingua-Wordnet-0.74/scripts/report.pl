#!/usr/bin/perl -w

# This program accesses the Lingua::Wordnet DB hashes directly in order 
# to generate database statistics. It is not recommended that you do this 
# from other programs.

use strict;
use Lingua::Wordnet;
use DB_File;

my (%synsets,%senses) = ();
my $wn = new Lingua::Wordnet;
my ($st,$key,$value);
$key = $value = 0;

print "Generating report. This could take lots of resources. Please wait ...\n";

for ($st = $wn->{indexobj}->seq($key, $value, R_FIRST) ;
    $st == 0 ;
    $st = $wn->{indexobj}->seq($key, $value, R_NEXT) )

    { 
        $key =~ /%(\w)$/ && do {
            $senses{$1}++;
        }
    };

print "\nUnique strings: \n";
print "  Noun: ", $senses{'n'}, "\n";
print "  Verb: ", $senses{'v'}, "\n";
print "  Adj:  ", $senses{'a'}, "\n";
print "  Adv:  ", $senses{'r'}, "\n";

for ($st = $wn->{dataobj}->seq($key, $value, R_FIRST) ;
    $st == 0 ;
    $st = $wn->{dataobj}->seq($key, $value, R_NEXT) )

    {
        $key =~ /%(\w)$/ && do {
            $synsets{$1}++;
        }
    };

print "\n\nSynsets: \n";
print "  Noun: ", $synsets{'n'}, "\n";
print "  Verb: ", $synsets{'v'}, "\n";
print "  Adj:  ", $synsets{'a'} + $synsets{'s'}, "\n";
print "  Adv:  ", $synsets{'r'}, "\n";

# TODO:
# Add polysemy count/ratio, attribute/function count, etc.

$wn->close;

