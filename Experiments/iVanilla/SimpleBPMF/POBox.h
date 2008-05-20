// This code is experimental and does not guarantee it will run.
// No build instruction is given. It might harm your system.
//
// Copyright (c) 2007-2008 Yao-Chung Chang and Lukhnos D. Liu
// Released under the same terms under which OpenVanilla is released 
// (The New BSD License)
//
// See LICENSE.TXT under OpenVanilla's source repository trunk/License/
// for the full text of the New BSD License.
//

@interface CSearch : NSObject
{
    int _mismatch;
    unsigned int _epsilon;
    unsigned int _shiftpat[256];
    unsigned int _pluspat[256];
    unsigned int _length;
}

- (id)initWithPattern:(char *)fp8 withAmbiguity:(int)fp12;
- (int)match:(char *)fp8;
- (int)length;

@end


@interface WordInfo : NSObject
{
    NSString *_word;
    NSString *_yomi;
    int _inConnection;
    int _outConnection[10];
    int _outConnections;
}

- (id)initWithWord:(id)fp8 withYomi:(id)fp12 inConnection:(int)fp16 outConnection:(int)fp20;
- (void)dealloc;
- (void)addOutConnection:(int)fp8;
- (id)word;
- (id)yomi;
- (int)inConnection;
- (int *)outConnection;
- (int)outConnections;

@end



@interface POBox : NSObject
{
    WordInfo *_learnDict[200];
    int _dictTopIndex;
    int _dictBottomIndex;
    int _maxCands;
    NSMutableArray *_candidates;
    int _lastConnection[10];
    int _lastConnections;
    int _searchMode;
    CSearch *_css[100];
    int _ncss;
    char *_wordStack[10];
    char *_yomiStack[10];
    int _ncands;
    int _linkInd;
    NSData *_wordListData;
    NSData *_connectionWordListData;
    NSData *_dictData;
    NSData *_connectionDictData;
}

- (id)init;
- (void)dealloc;
- (BOOL)checkGSEventQueueContainsMouseEvent;
- (int)addCandidate:(char *)fp8 yomi:(char *)fp12 inConnection:(int)fp16 outConnection:(int)fp20 nCands:(int)fp24;
- (void)addConnectedCandidate:(char *)fp8 yomi:(char *)fp12 inConnection:(int)fp16 outConnection:(int)fp20 withLevel:(int)fp24;
- (void)generateCand0:(int)fp8 matchedLen:(int)fp12 level:(int)fp16 matchedStr:(char *)fp20 matchedYomi:(char *)fp24;
- (void)generateCand;
- (id)getOneCandidate:(id)fp8;
- (id)getCandidates:(id)fp8 fixPat:(id)fp12;
- (void)regist:(id)fp8 withYomi:(id)fp12 inConnection:(int)fp16 outConnection:(int *)fp20 outConnections:(int)fp24;
- (id)lastWord;
- (void)setSearchMode:(int)fp8;
- (void)setMaxCands:(int)fp8;

@end

