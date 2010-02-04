# OpenVanilla-Renaissance

This is a new version of the OpenVanilla Framework, a cross-platform input method framework written in C++. It aims to address a number of shortfalls in the original framework.

The code supplied here is the basic framework that builds on both Mac OS X and Windows, 32-bit and 64-bit Intel and PowerPC architectures. Supported development tools include:

*   Xcode 3.2 on Mac OS X
*   Microsoft Visual Studio 2005 on Windows

This project also includes unit testing modules. Build `TestOpenVanilla.xcodeproj` or `TestOpenVanilla.sln` to run the test. A shell script, `ExternalLibraries/getexterns.sh`, fetches all the external dependencies ([UnitTest++](http://unittest-cpp.sourceforge.net/) and [SQLite](http://www.sqlite.org/)) for you.

The framework has a version number of 1.5.0. Efforts are made to ensure that modules written with this framework are never loaded by the currently circulating OpenVanilla implementations, which range from version 0.6 to 0.9.

The framework is divided into these main components:

*   The input method framework, including the classes that define what means to be a loadable module, an input method, an output filter, or an around filter. This would be the "Controller" part in a Model-View-Controller architecture.
*   The classes that define UI events and components, including the key input and different types of candidate windows.
*   Database services, including a table-driven lookup service and an optional SQLite-backed service.
*   Service and helper classes. OpenVanilla is designed to be self-contained and therefore relies on very few dependencies. To achieve this, we implement a number of frequently used helpers, including UTF-16-to-UTF-8 converter, file wrappers, localization helpers, and a wildcard matcher (in essence a very limited subset of regex).


## Acknowledgment

A number of friends have given generous feedback on the design of the framework.


## Copyright and License

Copyright (c) 2007-2010 Lukhnos D. Liu.

`OVKey.h` Copyright (c) 2007-2010 Lukhnos D. Liu and Weizhong Yang.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
