#!/usr/bin/perl -w
use strict;

die "USAGE: cin-sqlconvert some.cin" if (!@ARGV);

for my $fn (@ARGV) {
    open HNDL, "<$fn";
    my @a=($fn=~/(\w+)\..+/);
    my $tblname=lc $a[0];
    
    print "create table $tblname (key TEXT, freq INTEGER);\n";
    printf "create index %s_index_key on %s (key);\n", $tblname, $tblname;
    print "begin;\n";
    while(<HNDL>) {
        chomp;
        my @b=split /\t/, $_;
        printf "insert into %s values ('%s\', %d);\n", $tblname, $b[0], $b[1];
    }
    print "commit;\n";
}