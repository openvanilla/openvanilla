#!/usr/bin/perl -w
use strict;
use Encode;
use Getopt::Long;

my $tableName="ovPhoneticData";
my $LFcntr=0;

GetOptions("tablename=s" => \$tableName);

my ($consonant, $middleVowel, $vowel, $tone)=(0x1f, 0x60, 0x780, 0x3800);

our %stdKeyTable=(
'1' => 0x0001, 'q' => 0x0002, 'a' => 0x0003, 'z' => 0x0004,     # ㄅㄆㄇㄈ
'2' => 0x0005, 'w' => 0x0006, 's' => 0x0007, 'x' => 0x0008,     # ㄉㄊㄋㄌ
'e' => 0x0009, 'd' => 0x000a, 'c' => 0x000b,                    # ㄎㄍㄏ
'r' => 0x000c, 'f' => 0x000d, 'v' => 0x000e,                    # ㄐㄑㄒ
'5' => 0x000f, 't' => 0x0010, 'g' => 0x0011, 'b' => 0x0012,     # ㄓㄔㄕㄖ
'y' => 0x0013, 'h' => 0x0014, 'n' => 0x0015,                    # ㄗㄘㄙ
'u' => 0x0020, 'j' => 0x0040, 'm' => 0x0060,                    # ㄧㄨㄩ
'8' => 0x0080, 'i' => 0x0100, 'k' => 0x0180, ',' => 0x0200,     # ㄚㄛㄜㄝ
'9' => 0x0280, 'o' => 0x0300, 'l' => 0x0380, '.' => 0x0400,     # ㄞㄟㄠㄛ
'0' => 0x0480, 'p' => 0x0500, ';' => 0x0580, '/' => 0x0600,     # ㄢㄣㄤㄥ
'-' => 0x0680,                                                  # ㄦ
'6' => 0x0800, '3' => 0x1000, '4' => 0x1800, '7' => 0x2000,     # 2,3,4,5 tones
);


# skip until %chardef begin
while (<>) {
    last if (/^%chardef/);
}

my @entries;
my $charstr="";
my $charstrlen=0;
my $lastEntry="";
my $entryStr;
my $maxEntryStrLen=0;
my $maxEntry;
while (<>) {
    last if (/^%chardef/);
    my @pair=split;
    
    if ($pair[0] eq $lastEntry) {
        $entryStr .= decode("utf8", $pair[1]);
    }
    else {
        if ($entryStr) {
            my $len=length($entryStr);
            
            # printf ("%s => %s\n", $lastEntry, encode("utf8", $entryStr));            
            push @entries, [stdKeyToCode($lastEntry), $len, $entryStr];
            $maxEntryStrLen=$len if ($len > $maxEntryStrLen);
            $charstrlen+=$len;
        }
        $lastEntry=$pair[0];
        $entryStr = decode("utf8", $pair[1]);
    }
}

# puke the last entry
push @entries, [stdKeyToCode($lastEntry), length($entryStr), $entryStr];
$maxEntryStrLen=length($entryStr) if (length($entryStr) > $maxEntryStrLen);
$charstrlen+=length($entryStr);

print "unsigned short $tableName", "[", 
    1+1+((scalar @entries)+1)*2+$charstrlen+1, "]={\n";

print "// count of entry nodes (phonetic code, position of candidate string)\n";
output((scalar @entries)+1);
crlf();
print "// max length of candidate string\n";
output($maxEntryStrLen);
crlf();

print "// entry nodes section (the last is a padding node)\n";

my @sorted = sort { $$a[0] <=> $$b[0] } @entries; 

my $idx=0;
for my $e (@sorted) {
    output ($$e[0], $idx);
    $idx+=$$e[1];
    $charstr .= $$e[2];
}

output(0xffff, $idx);

crlf();
print "// candidate string section (the last is a padding code)\n";

for my $i (0..length($charstr)-1) {
    output (ord substr($charstr, $i, 1));
}

printf ("\n0xffff\n};\n");

sub crlf {
    if ($LFcntr) {
        $LFcntr=0;
        print "\n";
    }
}

sub output {
    while (defined(my $x=shift)) { 
        print STDERR $x, "\n" if ($x <0 );
        printf ("0x%04x, ", $x); 
        $LFcntr++;
        if ($LFcntr==10) {
            $LFcntr=0;
            print "\n";
        }
    }
}

sub stdKeyToCode {
    my ($s, $r)=(shift, 0);
    for my $i (0..length($s)-1) {
        $r |= $stdKeyTable{substr($s, $i, 1)};
    }
    $r;
}
