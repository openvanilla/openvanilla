/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>
#include <cxxtest/Win32Gui.h>

int main( int argc, char *argv[] ) {
 return CxxTest::GuiTuiRunner<CxxTest::Win32Gui, CxxTest::ErrorPrinter>( argc, argv ).run();
}
#include "DictionaryTest.h"

static DictionaryTest suite_DictionaryTest;

static CxxTest::List Tests_DictionaryTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_DictionaryTest( "DictionaryTest.h", 6, "DictionaryTest", suite_DictionaryTest, Tests_DictionaryTest );

static class TestDescription_DictionaryTest_testGetValueVector : public CxxTest::RealTestDescription {
public:
 TestDescription_DictionaryTest_testGetValueVector() : CxxTest::RealTestDescription( Tests_DictionaryTest, suiteDescription_DictionaryTest, 15, "testGetValueVector" ) {}
 void runTest() { suite_DictionaryTest.testGetValueVector(); }
} testDescription_DictionaryTest_testGetValueVector;

#include <cxxtest/Root.cpp>
