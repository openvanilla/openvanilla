#!/usr/bin/perl -w
use strict;
use utf8;
use Encode;

open bpmfTableFile, "<bpmf_punctuation.cin";
my $key2charHashRef;
my $key2wordHashRef;
while(<bpmfTableFile>) {
	chomp;
	next if (/^# /);
	if(/%keyname/) {
		$key2charHashRef = parseKeyname(*bpmfTableFile);
	}
	elsif(/%chardef/) {
		$key2wordHashRef = parseChardef(*bpmfTableFile);
	}
}

my %bpmf2topWordHash;
for(keys(%$key2wordHashRef)) {
	my $key = $_;
	my @alphabet=split '', $key;
	my $bpmf = "";
	foreach(@alphabet) {
		if(defined $key2charHashRef->{$_}) {
			$bpmf .= $key2charHashRef->{$_};
		}
	}
	$bpmf2topWordHash{$bpmf} = $key2wordHashRef->{$key};
	$bpmf = "";
}
close bpmfTableFile;

open lmFile, "<iasl.lm";
while(<lmFile>) {
	chomp;
	if(/#?\s*(\S+)\s+([^_]+)_(\S+)\s+(\S+)/) {
		my($p, $w, $c, $b) =
			($1, decode("utf8", $2), decode("utf8", $3), $4);
	
        	unless(exists $bpmf2topWordHash{$c} &&
			$w ne $bpmf2topWordHash{$c}) {
			print "$_\n";
		}
	}
}
close lmFile;

sub parseKeyname {
	my $hndl=shift;
	my %key2charHash;
	while(<$hndl>) {
		chomp;
		last if (/%keyname/);
		my @a=split;
		$key2charHash{lc $a[0]}=decode("utf8", $a[1]);
	}

	return \%key2charHash;
}

sub parseChardef {
	my $hndl=shift;
	my %key2topWordHash;
	while (<$hndl>) {
		chomp;
		next unless $_;
		last if (/%chardef/);
		my @a=split;
		unless (exists $key2topWordHash{decode("utf8", $a[0])})
		{
			$key2topWordHash{decode("utf8", $a[0])} =
				decode("utf8", $a[1]);
		}
	}

	return \%key2topWordHash;
}
