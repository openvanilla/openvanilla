#!/usr/bin/perl -w
use strict;

die "USAGE: cin-sqlconvert file1 file2 ..." if (!@ARGV);

for my $fn (@ARGV) {
    open HNDL, "<$fn";
    my @a=($fn=~/(\w+)\..+/);
    my $tblname=lc $a[0];
    
    print "create table $tblname (key);\n";
    printf "create index %s_index_key on %s (key);\n", $tblname, $tblname;
    print "begin;\n";
    while(<HNDL>) {
        chomp;
        next if (/^# /);
        next if (/^-/);
        parse_dict($tblname);
    }
    print "commit;\n";
}

sub parse_dict {
    my $tblname=shift;
    my @a=split;
    return if (!scalar(@a));
    if ($a[0] =~ /^%.+/) { $a[0]=substr($a[0], 1, length($a[0])-1);  }
    $a[0] =~ s/\'/\'\'/g;
    if ($a[1]) { $a[1] =~ s/\'/\'\'/g; } else { $a[1]="" };
    printf "insert into %s values ('%s\');\n", $tblname, lc $a[0];
}
