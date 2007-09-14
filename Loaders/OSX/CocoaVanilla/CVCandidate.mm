// CVCandidate.mm: CocoaVanilla implementation of OVCandidate
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "CVCandidate.h"
#include "OVDisplayServer.h"

CVCandidateState::CVCandidateState(NSString *s, Point p, BOOL o) {
    str=[[NSString alloc] initWithString:s];
    pos=p;
    onscreen=o;
}

CVCandidateState::~CVCandidateState() {
	[str release];
}

CVCandidate::CVCandidate(id s) {
	srvr=s;
	text=[NSMutableString new];
	pos=(Point){0, 0};
	onscreen=NO;
}

CVCandidate::~CVCandidate() {
	[text release];
}

void CVCandidate::changeDisplayServer(id newServer)
{
	srvr = newServer;
}

OVCandidate* CVCandidate::clear() {
	[text setString:@""];
    return this;
}

OVCandidate* CVCandidate::append(const char *s) {
    NSString *u8s=[NSString stringWithUTF8String:s];
	if (u8s) [text appendString:u8s];
    return this;
}

OVCandidate* CVCandidate::hide() {
	if (onscreen) {
		if (srvr) [srvr candidateHide];
		onscreen=NO;
	}
    return this;
}

OVCandidate* CVCandidate::show() {
	if (!onscreen) {
		if (srvr) [srvr candidateShow];
		onscreen=YES;
	}
    return this;
}

OVCandidate* CVCandidate::update() {
	if (srvr) [srvr candidateUpdate:text position:pos];
    return this;
}

int CVCandidate::onScreen() {
    return onscreen;
}

OVCandidate* CVCandidate::setPosition(Point p) {
	pos=p;
    return this;
}

CVCandidateState* CVCandidate::saveState() {
	return new CVCandidateState(text, pos, onscreen);
}

OVCandidate* CVCandidate::restoreState(CVCandidateState *s) {
	[text setString:s->str];
	pos=s->pos;
	update();
	if (s->onscreen) show(); else hide();
    return this;
}

