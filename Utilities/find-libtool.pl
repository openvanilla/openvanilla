#!/usr/bin/perl

my $libtool;
my @possible_name = qw/libtool libtool15/;
for(@possible_name){
   $libtool = `which $_`;
   if($? == 0){
      print "$libtool";
      exit 0;
   }
}
exit -1;
