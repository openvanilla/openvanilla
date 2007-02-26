1. Download UnitTest++
2. Unzip it and "make" in the unzipped folder.
3. Copy all .h files in "src" folders, included the subfolder "Posix" or "Win32" to /usr/local/include.
4. Copy built library "libUnitTest++.a" to /usr/local/lib
5. For Mac OS X, run "sudo ranlib /usr/local/lib/libUnitTest++.a"
6. Read any tuple of TestXXX.cpp/make_TestXXX.sh/run_TestXXX.sh for the rest.
