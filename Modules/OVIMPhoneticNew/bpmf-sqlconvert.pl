#!/usr/bin/perl -w
use strict;

print "begin;\n";
while(<>) {
    if (/%chardef/) {
        parse_chardef();
    }
    if (/%keyname/) {
        parse_keyname();
    }
    else {
        parse_property();
    }    
}
print "commit;\n";

sub parse_property {
    my @a=split;
    exit if (!scalar(@a));
    if ($a[0] =~ /^%.+/) { $a[0]=substr($a[0], 1, length($a[0])-1);  }
    $a[0] =~ s/\'/\'\'/g;
    if ($a[1]) { $a[1] =~ s/\'/\'\'/g; } else { $a[1]="" };
    printf "insert into bpmf values ('_property_%s\', '%s\', -1);\n", lc $a[0], $a[1];
}

sub parse_keyname {
    while(<>) {
        last if (/%keyname/);
        my @a=split;
        $a[0] =~ s/\'/\'\'/g;
        $a[1] =~ s/\'/\'\'/g;
        printf "insert into bpmf values ('_key_%s\', '%s', -1);\n", lc $a[0], $a[1];
    }
}

sub parse_chardef {
    my ($lastbpmf, $lastorder)=("", 0);
    while (<>) {
        chomp;
        last if /%chardef/;
        my @a=split;
        if ($a[0] eq $lastbpmf) {
            $lastorder++;
        }
        else
        {
            $lastorder=0;
            $lastbpmf=$a[0];
        }
        $a[0] =~ s/\'/\'\'/g;
        $a[1] =~ s/\'/\'\'/g;
        printf "insert into bpmf values('%s', '%s', %s);\n", sprintf("%s", $a[0]), $a[1], $lastorder;
    }
}
