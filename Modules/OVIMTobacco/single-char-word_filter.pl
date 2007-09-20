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
	foreach(keys(%{$key2wordHashRef->{$key}})) {
		$bpmf2topWordHash{$_}{$bpmf} = 1;
	}
	$bpmf = "";
}
close bpmfTableFile;

open lmFile, "<iasl.lm";
while(<lmFile>) {
	chomp;
	my $line = $_;
	if(/#?\s*(\S+)\s+([^_]+)_(\S+)\s+(\S+)/) {
		my($p, $w, $c, $b) =
			($1, decode("utf8", $2), decode("utf8", $3), $4);
	
        	if(length($w) > 1) {
			print "$line\n";
		}
		else {
			foreach(keys(%{$bpmf2topWordHash{$w}})) {
				if($c eq $_) {
					print "$line\n";
				}
			}
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
		my $key = decode("utf8", $a[0]);
		my $word = decode("utf8", $a[1]);
		unless (keys(%{$key2topWordHash{$key}}) == 1)
		{
			$key2topWordHash{$key}{$word} = 1;
		}
	}

	return \%key2topWordHash;
}
