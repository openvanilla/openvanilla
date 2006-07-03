#!/usr/bin/perl -w

# convert-phonedata.pl: converts bpmf.cin into OVPhoneData-compliant data table
#
# Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of OpenVanilla nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

use strict;
use Encode;
use Getopt::Long;

my $tableName="ovPhoneticData";
my $LFcntr=0;

GetOptions("tablename=s" => \$tableName);

my ($punctuation, $ctrlopt, $punctlist)=(0xff00, 0xfe00, 0xff80);

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


my @entries;
my $charstr="";
my $charstrlen=0;
my $lastEntry="";
my $entryStr;
my $maxEntryStrLen=0;
my $maxEntry;
while (<>) {
    if (/^%chardef/) {
        while (<>) {
            last if /^%chardef/;        
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

    if ($s =~ /_punctuation_list/) {
        $r=$punctlist;
    }    
    elsif ($s =~ /_punctuation_(.+)/) {
        $r=$punctuation | ord(uc $1);
    }
    elsif ($s =~ /_ctrl_opt_(.+)/) {
        $r=$ctrlopt | ord(uc $1);
    }
    else { 
        for my $i (0..length($s)-1) {
            $r |= $stdKeyTable{substr($s, $i, 1)};
        }
    }    
    # printf "stdkey %s = code 0x%04x\n", $s, $r;
    $r;
}
