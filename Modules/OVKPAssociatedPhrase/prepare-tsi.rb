#!/usr/bin/ruby
$KCODE="u"
require "rubygems"
require "active_support"
require "yaml"

if !filename = ARGV.shift
  $stderr.puts "usage: prepare-tsi.rb input"
  exit 1
end

begin
  file = File.open(filename, "r")
rescue => r
  $stderr.puts "error: #{r}"
  exit 1
end

phrases = {}
lines = 0

while !file.eof
  lines += 1
  elements = file.gets.chomp.split(/\s+/)
  
  string = elements[0].chars
  freq = elements[1].to_i
  
  next if string.length < 2
  
  first = [string[0]].pack("U")
  string = string[1..string.length-1]
  
  if !phrases[first]
    phrases[first] = { string.to_s => freq }
  else
    phrases[first][string.to_s] = freq
  end
  
  if (lines % 10000) == 0
    $stderr.puts "#{lines}"
  end
  # puts "read line #{lines}, first #{first}, string #{string.to_s}, freq #{freq}"
end


phrases.keys.each do |k|
  array = []
  phrases[k].keys.each do |pk|
    array << [pk, phrases[k][pk]]
  end
  
  phrases[k] = array
end

puts <<HEADER
%gen_inp
%ename Associated phrases
%keyname begin
%keyname end
%chardef begin
HEADER


phrases.keys.sort.each do |k|
  # puts "#{k}"
  phrases[k].sort {|x, y| y[1] <=> x[1] }[0, 9].each do |p|
    # puts "  #{p[0]} => #{p[1]}"
    puts "#{k} #{p[0]}"
  end
end

puts '%chardef end'