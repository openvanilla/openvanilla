#!/usr/bin/perl -w

# bpmf-sqlconvert.pl: convert bpmf.cin into SQLite database
#
# Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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

print "begin;\n";
while(<>) {
    if (/%chardef/) {
        parse_chardef();
    }
    elsif (/%keyname/) {
        parse_keyname();
    }
    else {
        parse_property();
    }    
}
print "commit;\n";

sub parse_property {
    my @a=split;
    return if (!scalar(@a));
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
