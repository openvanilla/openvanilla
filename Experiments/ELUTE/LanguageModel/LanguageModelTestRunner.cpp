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
#include "LanguageModelTest.h"

static LanguageModelTest suite_LanguageModelTest;

static CxxTest::List Tests_LanguageModelTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_LanguageModelTest( "LanguageModelTest.h", 6, "LanguageModelTest", suite_LanguageModelTest, Tests_LanguageModelTest );

static class TestDescription_LanguageModelTest_testGetLogProb : public CxxTest::RealTestDescription {
public:
 TestDescription_LanguageModelTest_testGetLogProb() : CxxTest::RealTestDescription( Tests_LanguageModelTest, suiteDescription_LanguageModelTest, 15, "testGetLogProb" ) {}
 void runTest() { suite_LanguageModelTest.testGetLogProb(); }
} testDescription_LanguageModelTest_testGetLogProb;

static class TestDescription_LanguageModelTest_testGetBackOff : public CxxTest::RealTestDescription {
public:
 TestDescription_LanguageModelTest_testGetBackOff() : CxxTest::RealTestDescription( Tests_LanguageModelTest, suiteDescription_LanguageModelTest, 39, "testGetBackOff" ) {}
 void runTest() { suite_LanguageModelTest.testGetBackOff(); }
} testDescription_LanguageModelTest_testGetBackOff;

#include <cxxtest/Root.cpp>
