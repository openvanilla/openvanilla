$KCODE = "u"

# use active_support's chars if you have it
# require "active_support"

class OVOFRubyFilter
  $stderr.puts "OVRubyFilter class init"
  
  def process(str)
    $stderr.puts "Received string: #{str}"

    # this splits each codepoint into separate UTF-8 strings
    array = str.split //
    $stderr.puts array
    
    # do some brain-dead, 火星文-style substituion
    str.gsub!(/ㄅ/, "吧")
    str.gsub!(/ㄇ/, "嗎")
    
    # this demos the power when you have regex
    str.gsub!(/吃|ㄔ/, "喫")
    
    # yet another brain-dead substituion
    str.gsub!(/惡劣/, "ㄚˋ劣")
    
    # returns the string
    str
  end
end