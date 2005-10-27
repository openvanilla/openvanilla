1. Download clucene-0.9.8.zip, unzip to trunk/Experiments/Lucene4SpellChecker/clucene-0.9.8
2. Use trunk/Experiments/Lucene4SpellChecker/clucene-0.9.8/win32/Clucene.sln to build the library.
3. Just use OVIMRomanNew.vcproj or go to trunk/Modules/OVIMRoman/ and then run the make.bat to get the DLL.
4. Use trunk/Experiments/Lucene4SpellChecker/make.bat to get LuceneAdd.exe
-5. Copy trunk/Data/dict-en.txt to trunk/Experiments/Lucene4SpellChecker/dict.txt (Feel free to use alternative dictionary, just make sure it is in the same format and filename with dict.txt)-
-6. Run LuceneAdd.exe to build Lucene's index.-
7. Put the index directory into OVIMRomanNew's module directory, i.e. %WINDIR%\OpenVanilla\Modules\OVIMRomanNew
8. Convert dict.txt into SQLite3 database dict.db by cin-sqlconvert.pl and then put it to the right place.

Step 5. and 6. seem to be not work on Win32.