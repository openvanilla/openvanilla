#!/usr/bin/perl

use strict;
use IO::All;

my $uv = io("UniVariants");

my $output;
my $size;
while ($_ = $uv->getline) {
    next if /#/;
    my ($char, @vars) = split(" ", $_);

    my $charvar = choose_jp_kanji_char(@vars);
    if ($charvar && ($charvar ne $char)) {
        $output .= "0x${char}, 0x${charvar},\n";
        $size++;
    }
}
$output =~ s/,\n$/\n/s;

my $o = io "ZhTwToJpKanji.c";

$o->print("unsigned short ZhTwToJpKanjiTable[$size*2]={\n");
$o->print($output);
$o->print("};\n");

print "$o generated\n";

use Encode;
sub choose_jp_kanji_char {
    my @chars = @_;
    for my $codepoint (@chars) {
        my $char = chr(hex($codepoint));
        my $jis  = Encode::encode("shiftjis", $char, Encode::FB_QUIET);
        if ($jis) {
            ## This char is inside the range of shiftjis
            return $codepoint;
        }
    }
    return "";
}
