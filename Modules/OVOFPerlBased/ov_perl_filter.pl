package OV::OutputFilter;
use strict;
use warnings;
use utf8;

sub process {
    my $str = shift;
    utf8::decode($str);

    $str =~ s/吃/喫/g;
    $str =~ s/才/纔/g;

    return $str;
}


1;

