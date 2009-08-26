#!/usr/bin/ruby

require "stringio"

def replace_info_plist(filename, version)
  file = File.open(filename, "r")
  output = StringIO.new  
  while line = file.gets
    
    if line =~ /CFBundleVersion/
      output.puts line
      file.gets
      line = "\t<string>#{version}</string>"
    end
    
    output.puts line
  end
  file.close  
  file = File.open(filename, "w")
  file.print output.string
  file.close
end

if ARGV.size < 2
  STDERR.puts "usage: replace-bundle-version.rb files... version"
  exit 1
end

version = ARGV.pop

ARGV.each do |f|
  replace_info_plist(f, version)
end


    