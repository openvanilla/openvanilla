#!/bin/sh
if [ "$CXXTEST_HOME" = "" ]; then
echo "Where is CXXTEST_HOME?";
read CXXTEST_HOME
export CXXTEST_HOME
fi
cxxtestgen.pl --error-printer -o runner.cpp TestSequence.h
g++ -I$CXXTEST_HOME -I../ -o runner runner.cpp ../Element.cpp ../Chunk.cpp ../SequenceVisitor.cpp
./runner
rm runner*
