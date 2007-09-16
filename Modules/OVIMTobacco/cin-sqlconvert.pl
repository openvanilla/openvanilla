#!/usr/bin/perl -w
use strict;

die "USAGE: cin-sqlconvert file1 file2 ..." if (!@ARGV);

for my $fn (@ARGV) {
    open HNDL, "<$fn";
    my @a=($fn=~/(\w+)\..+/);
    my $tblname=lc $a[0];
    
    print "insert into tablelist values ('$tblname');\n";
    print "create table $tblname (key, value, ord);\n";
    printf "create index %s_index_key on %s (key);\n", $tblname, $tblname;
    printf "create index %s_index_value on %s (value);\n", $tblname, $tblname;
    print "begin;\n";
    while(<HNDL>) {
        chomp;
        next if (/^# /);
        if (/%chardef/) {
            parse_chardef(*HNDL, $tblname);
        }
        elsif (/%keyname/) {
            parse_keyname(*HNDL, $tblname);
        }
        else {
            parse_property($tblname);
        }    
    }
    print "commit;\n";
}

sub parse_property {
    my $tblname=shift;
    my @a=split;
    return if (!scalar(@a));
    if ($a[0] =~ /^%.+/) { $a[0]=substr($a[0], 1, length($a[0])-1);  }
    $a[0] =~ s/\'/\'\'/g;
    if ($a[1]) { $a[1] =~ s/\'/\'\'/g; } else { $a[1]="" };
    printf "insert into %s values ('_property_%s\', '%s\', -1);\n", $tblname, lc $a[0], $a[1];
}

sub parse_keyname {
    my $hndl=shift;
    my $t=shift;
    while(<$hndl>) {
        last if (/%keyname/);
        my @a=split;
        $a[0] =~ s/\'/\'\'/g;
        $a[1] =~ s/\'/\'\'/g;
        printf "insert into %s values ('_key_%s\', '%s', -1);\n", $t, lc $a[0], $a[1];
    }
}

sub parse_chardef {
    my $hndl=shift;
    my $t=shift;
    my %key2orderHash;
    while (<$hndl>) {
        chomp;
	next unless $_;
        last if /%chardef/;
        my @a=split;
        my $currentOrder = 0;
        if (exists $key2orderHash{$a[0]})
        {
            $currentOrder = $key2orderHash{$a[0]};
            $currentOrder = $currentOrder + 1;
            $key2orderHash{$a[0]} = $currentOrder;
        }
        else
        {
            $key2orderHash{$a[0]} = $currentOrder;
        }
        $a[0] =~ s/\'/\'\'/g;
        $a[1] =~ s/\'/\'\'/g;
        printf "insert into %s values('%s', '%s', %s);\n", $t, lc sprintf("%s", $a[0]), $a[1], $currentOrder;
    }
}
