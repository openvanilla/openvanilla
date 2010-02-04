#!/usr/bin/ruby
#
# cin-to-sqlite.rb
#
# Copyright (c) 2007-2010 Lukhnos D. Liu
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#

$KCODE = "u"

def insert_value(line, table_name, prefix = "")
  if !(line =~ /(\S+)\s+(.+)/)
    a = line.split(/\s+/)
    k = a[0] || ""
    v = a[1] || ""
  else
    k = $1 || ""
    v = $2 || ""  
  end
  
  k.gsub!(/'/, "''")
  v.gsub!(/'/, "''")
  puts "INSERT INTO '#{table_name}' VALUES('#{prefix}#{k}', '#{v}');"
end

if ARGV.size < 2
  $stderr.puts "usage: cin-to-sqlite.rb .cin table-name --chardef-only"
  exit 1
end

filename = ARGV.shift
table_name = ARGV.shift
chardef_only = (ARGV.shift == "--chardef-only")

begin
  f = File.open(filename, "r")
rescue
  $stderr.puts "cannot open: #{filename}"
  exit 1
end

puts "BEGIN;"

while line = f.gets
  line.chomp!
  
  if line =~ /^%chardef\s+begin/i
    while line = f.gets
      line.chomp!
      break if line =~ /^%chardef\s+end/i
      insert_value(line, table_name)
    end
  elsif line =~ /^%keyname\s+begin/i && !chardef_only
    while line = f.gets
      line.chomp!
      break if line =~ /^%keyname\s+end/i
      insert_value(line, table_name, "__property_keyname-")
    end
  elsif line =~ /^%/ && !chardef_only
    insert_value(line[1..line.size-1], table_name, "__property_")
  end  
end

f.close

puts "COMMIT;"