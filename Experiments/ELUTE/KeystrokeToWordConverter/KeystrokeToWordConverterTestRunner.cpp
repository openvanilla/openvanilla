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
#include "KeystrokeToWordConverterTest.h"

static KeystrokeToWordConverterTest suite_KeystrokeToWordConverterTest;

static CxxTest::List Tests_KeystrokeToWordConverterTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_KeystrokeToWordConverterTest( "KeystrokeToWordConverterTest.h", 10, "KeystrokeToWordConverterTest", suite_KeystrokeToWordConverterTest, Tests_KeystrokeToWordConverterTest );

static class TestDescription_KeystrokeToWordConverterTest_testConvert : public CxxTest::RealTestDescription {
public:
 TestDescription_KeystrokeToWordConverterTest_testConvert() : CxxTest::RealTestDescription( Tests_KeystrokeToWordConverterTest, suiteDescription_KeystrokeToWordConverterTest, 42, "testConvert" ) {}
 void runTest() { suite_KeystrokeToWordConverterTest.testConvert(); }
} testDescription_KeystrokeToWordConverterTest_testConvert;

static class TestDescription_KeystrokeToWordConverterTest_testFixedToken : public CxxTest::RealTestDescription {
public:
 TestDescription_KeystrokeToWordConverterTest_testFixedToken() : CxxTest::RealTestDescription( Tests_KeystrokeToWordConverterTest, suiteDescription_KeystrokeToWordConverterTest, 47, "testFixedToken" ) {}
 void runTest() { suite_KeystrokeToWordConverterTest.testFixedToken(); }
} testDescription_KeystrokeToWordConverterTest_testFixedToken;

#include <cxxtest/Root.cpp>
