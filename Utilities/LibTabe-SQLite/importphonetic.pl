#!/usr/bin/perl -w
use strict;
use utf8;
use Encode;
use DBI;
use YAML;

my %tonemap=("3" => 3, "4" => 4, "6" => 2, "7" => 5);

my $dbh=DBI->connect("dbi:SQLite:dbname=tsi.db", "", "");
$dbh->do("begin;");
my $sth=$dbh->prepare("insert into bpmf values (?,?,?,?);");

while(<>) {
    last if /%chardef/;
}

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
    
    my $tone;
    if ($a[0] =~ /(3|4|6|7)/) {
        $tone=$tonemap{$1};
        $a[0] =~ s/3|4|6|7//g;
    }
    else {
        $tone=0;
    }
    
    # print "$a[0], $a[1], $tone, $lastorder\n";
    $sth->execute($a[0], $a[1], $tone, $lastorder);    
}

$dbh->do("commit;");

