OpenVanilla 0.7.2 (beta) for OS X
Extra Modules and Input Method Data Tables

Copyright (c) 2004-2006 The OpenVanilla Project

Since 0.7.2, OpenVanilla allows extra modules and input method data tables
to be installed under specific directories within user's home directory.
This package comes with a number of useful modules not included in the
Base installation package (which includes the Standard Set of Modules).

Please read "How to Install Extra Modules" and "How to Install Data Tables"
before you proceed to the "Package Content" section.

== How to Install Modules ==

Let's take the "Unicode Decoder" (which is an output filter) for example.
After you have mounted this Extra Package's disk image, you will find
there is a directory named "(Module) Unicode Decoder", in which you will
only find one file: OVOFUnicodeDecoder.dylib.

To install that module, simply copy *the file* into this directory:

    ~/Library/OpenVanilla/0.7.2/Modules/
    
Where "~" is the Unix convention that represents the name of your 
Home Directory (such as /Users/foobar). PLEASE DO NOT COPY THE DIRECTORY
OUTSIDE THE .DYLIB INTO THE Modules/ . Copy only the .dylib into 
that Modules/ directory.

Please note that, prior to OpenVanilla 0.7.2, you have to copy the
module library files into OS X's /Library/OpenVanilla/version_number/Modules
(note there is no "~" here). This is no longer necessary although it 
is still allowed to do so.

Some module, such as the one labelled "Phonetic with Large Char. Set",
comes with a data directory along with the .dylib file. Please also
copy that directory into Modules/ .

To sum up: copy (drag) THE CONTENT (.dylib and data directory, if any)
into your ~/Library/OpenVanilla/0.7.2/Modules/ , and you're done.

One last note: Currently running applications won't see the newly
installed moudles, but newly opened applications will. This is another
oddity of OS X's design, but we won't talk about that much further here.
Have fun!

== How to Install Data Tables ==

OpenVanilla offers two modules that make use of the .cin format:
the OVIMGeneric (the "Generic Input Method generator" module) and
the OVOFReverseLookup (the "radical look-up filter" module).
If a .cin file does not begin with the prefix "lookup-", it's mostly
a data table for the Generic Input Method module. Otherwise, it's
for the look-up filter.

To install a .cin input method data table, simply copy the file to:

    ~/Library/OpenVanilla/0.7.2/UserSpace/OVIMGeneric/
    
Whereas a look-up table is copied to:

    ~/Library/OpenVanilla/0.7.2/UserSpace/OVOFReverseLookup/

And that's pretty much of it!

== Package Content (Selective) ==

Below is a short description of what each module does here. Note that
description is omitted for the .cin tables, as their names does not
translate into English: you have to be a Chinese-speaking user so
that the names mean something to you. In any case, you can refer to 
the Chinese version of this README file instead.

"Roman (Alphanumeric)". This is a functional duplicate of the system's
ASCII keyboard layout. There is not so much point using this module.
Yet: if you enter more Chinese characters than Latin ones, or if 
you want to stay with OpenVanilla, not wanting to switch back and
forth between OpenVanilla and your default keyboard layout often,
of if you want to make use of the filters (such as the "Smart
Quotations" filters below), you have to install and use this 
"input method".

"Tibetan". This is a functional Tibentan input method, supporting four
keyboard layouts: Sambhota Keymap one, Sambhota Keymap two, TCC Keyboard
One and TCC Keyboard Two. More information can be found at:
http://tibetanim.blogspot.com/ 

"UIM-Anthy". This is an "interface" to the UIM library and provides the
Anthy Japanese input method in Hiragana mode. Actually it depends on
MacUIM 0.4.1-1 (http://sourceforge.jp/projects/macuim/) and you'll have
to install MacUIM before you can use this module. It's only a functional
IM and not as aesthetically refined as MacUIM. So the question: since
there's already MacUIM, why OpenVanilla's own take of Japanese IM? 
It's true, indeed. On the other hand, if you only occasionally needs
to type a few Japanese characters or phrases, and switching input method
is a pain for you (at least for me, thanks to OS X's bizarre design),
this UIM-Anthy module may save you a few tenths of a second.

"Unicode IME". Type the Unicode and get the character.

"Zhuyinwen Filter". It's really a Taiwanese phenomenon, but some people
in Taiwan do have got into the habit of using phonetic syllables to
write their articles. (Not that it's not allowed, it's just a matter
of style. Mandarin Chinese is not like Japanese; the latter makes 
extensive use of syllabic writing, whereas in the former ideographs
dominate). Anyway, if you just detest those funny symbols and readings
and don't want them to ever show up on your screen, install this module
and enable it. You'll become a stylistic puritan.

"Smart Quotations for English, French and German". This set of fitlers
emulates the "intelligent quotations" found in Microsoft Word.

"HTML Character Entities Filter". Type & and you got &amp; < for &lt;
and > for &gt; HTML developers may find it useful.

"Unicode Decoder". Type any character with OpenVanilla's any input 
method and get the Unicode codepoint of the character.

"zh.Wikipedia Helper". zh.wikipeida.org uses a full stop separator 
symbol that is different from what most Chinese input methods can come
up with. This filter converts that runaway symbol into a conforming
one.

== Copyright and Terms of Use ==

The package itself, along with the Roman, UIM-Anthy, Zhuyinwen and HTML
Entities modules are written by the OpenVanilla Team members and 
released under the same terms of OpenVanilla (the 3-clause BSD license).
All other modules are written by zonble and released under the one-sentence
zonble's License: 

    "How could a person who doesn't even believe in law adopt any license?"
    
crossbone contributed all Cangjei-related data tables. cj-ext.cin, 
simplex-ext.cin and bpmf-ext.cin (on which OVIMPhoneticSQLite's large
bpmf.db is based) are generated from the CNS11643 data table of
the CMEX Foundation (http://www.cmex.org.tw/cmex) and are put under
the same terms of use that OpenDesktop.org.tw (http://www.opendesktop.org.tw/)
designates to the same set of data.

