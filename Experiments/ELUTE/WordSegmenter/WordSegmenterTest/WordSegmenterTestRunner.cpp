/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ParenPrinter.h>

int main() {
 return CxxTest::ParenPrinter().run();
}
#include "WordSegmenterTest.h"

static WordSegmenterTest suite_WordSegmenterTest;

static CxxTest::List Tests_WordSegmenterTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_WordSegmenterTest( "WordSegmenterTest.h", 10, "WordSegmenterTest", suite_WordSegmenterTest, Tests_WordSegmenterTest );

static class TestDescription_WordSegmenterTest_testSegment : public CxxTest::RealTestDescription {
public:
 TestDescription_WordSegmenterTest_testSegment() : CxxTest::RealTestDescription( Tests_WordSegmenterTest, suiteDescription_WordSegmenterTest, 19, "testSegment" ) {}
 void runTest() { suite_WordSegmenterTest.testSegment(); }
} testDescription_WordSegmenterTest_testSegment;

#include <cxxtest/Root.cpp>
