1. Prepare Hunspell library
1.1 For Win32, just follow the instruction 4 in trunk/README.Win32.zh_HANT.UTF-8.txt
1.2 For Mac OS X:
1.2.1. Get the source from
	http://hunspell.sourceforge.net/
1.2.2. Build the library. Universal Binary does not work yet, see
	http://sourceforge.net/tracker/index.php?func=detail&aid=1740138&group_id=143754&atid=756395
for the details of this issue. Anyway, under the source code folder:
	> ./configure && make && sudo make install
1.2.3. Copy the license files
	> sudo cp license.* $PREFIX/include/hunspell
where $PREFIX is usually /usr/local

2. Prepare Hunspell dictionary
2.1. Get affix and dictionary files from
	http://ftp.services.openoffice.org/pub/OpenOffice.org/contrib/dictionaries/en_US.zip
2.2. Unzip en_US.zip *here*.

3. Build OVIMRomanNew
3.1. For Win32, use OVIMRomanNew.vcproj
3.2. For Mac OS X, do it here:
	> make && sudo make install
