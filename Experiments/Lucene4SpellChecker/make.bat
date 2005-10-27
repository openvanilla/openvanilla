cl /Iclucene-0.9.8\src /I..\..\Headers /EHsc /D"WIN32" /D"UNICODE" /D"_UNICODE" LuceneAdd.cpp ..\..\Source\*.cpp /link clucene-0.9.8\build\lib\CLuceneULib.lib
pause