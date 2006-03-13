del *.*~
cl -EHsc -I..\..\..\Framework\Headers -I..\tinyxml -I.\ AVDictionary.cpp /DOV_DEBUG /DTEST_DICT /link ..\tinyxml\tinyxml.lib 
