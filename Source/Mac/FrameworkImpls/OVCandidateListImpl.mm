//
// OVCandidateListImpl.mm
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import "OVCandidateListImpl.h"

using namespace OpenVanilla;

OVCandidateListImpl::OVCandidateListImpl()
{
    m_array = [[NSMutableArray alloc] init];
}

OVCandidateListImpl::~OVCandidateListImpl()
{
}

void OVCandidateListImpl::clear()
{
    [m_array removeAllObjects];
}

size_t OVCandidateListImpl::size() const
{
    return (size_t)[m_array count];
}

string OVCandidateListImpl::candidateAtIndex(size_t index) const
{
    return string([[m_array objectAtIndex:index] UTF8String]);
}

NSString *OVCandidateListImpl::candidateAtIndex(size_t index)
{
    return [m_array objectAtIndex:(NSUInteger)index];
}

void OVCandidateListImpl::setCandidate(size_t index, const string& candidate)
{
    NSCAssert(index < [m_array count], @"Index must be less than array size");
    [m_array replaceObjectAtIndex:(NSUInteger)index withObject:[NSString stringWithUTF8String:candidate.c_str()]];
}

void OVCandidateListImpl::setCandidates(const vector<string>& candidates)
{
    [m_array removeAllObjects];
    addCandidates(candidates);
}

void OVCandidateListImpl::addCandidate(const string& candidate)
{
    [m_array addObject:[NSString stringWithUTF8String:candidate.c_str()]];
}

void OVCandidateListImpl::addCandidates(const vector<string>& candidates)
{
    for (vector<string>::const_iterator i = candidates.begin(), e = candidates.end(); i != e; ++i) {
        addCandidate(*i);
    }
}
