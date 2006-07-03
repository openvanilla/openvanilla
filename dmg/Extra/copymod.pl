#!/usr/bin/perl -w
#
# copymod.pl: packaging utility for Extra Pack
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

die "usage: copymod.pl DSTROOT MODULE-SOURCE module-name folder-en folder-zh_TW folder-zh_CN" if (@ARGV<6);

my ($d, $m, $n, $en, $tw, $cn)=@ARGV;

$td="$d/$n.localized/";
$nd=$td . ".localized";
runcmd ("mkdir -p $nd");
writetofile("$nd/en.strings", $n, $en);
writetofile("$nd/zh_TW.strings", $n, $tw);
writetofile("$nd/zh_CN.strings", $n, $cn);

if (-d "$m/$n/") {
    # print "#### DIRECTORY $m/$n/ exists\n";
    $c=`ls $m/$n/ | wc -l`;
    chomp($c);
    $c =~ s/ //g;
    
    if ($c) {
        runcmd("cp -R $m/$n $td/");
    }
    # print "#### $m/$n/ has ", $c, " files = ", ($c ? "not empty" : "empty"), "\n";
}

runcmd ("cp -R $m/$n.* $td");

sub writetofile {
    my ($fn, $s, $ts)=@_;
    open F, "> $fn";
    print F "\"$s\" = \"$ts\";\n";
    close F;
}

sub runcmd {
    my $x=shift;
    print "$x\n";
    system $x;
}
