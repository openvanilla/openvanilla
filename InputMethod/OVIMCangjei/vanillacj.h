// vanillacj.h: Cangjei (Tsang-chieh) library interface for VanillaInput

#ifndef __vanillacj_h
#define __vanillacj_h

// 0 is always error in our system -- we don't use BOOL!

// as Cangjei's design remains unfixed for over almost three decades (?),
// much of the work can be done in hard and very efficient code.
// our hommage to Mr. Zhu Bangfu...

// TO DO: FUSE VCUTF16 WITH VPUTF16 -- MAYBE WE'LL NEED A COMMON
//        VANILLA LIBRARY (AND A TYPE E.G. VLUTF16) TO ACCOMMODATE THE UTF16 TYPE
typedef unsigned short VCUTF16;

class VCCJCode
{
	friend class VCCJString;	// we allow VCCJString to get its own hands into us
public:
	VCCJCode(char keychar=0, int isWildcard=0);		// VJCJCode(0,1) gives a wildcard
	int KeyToCode(char);		// returns 0 if an invalud key character

	int IsValidCode();
	int IsWildcard();
	
	VCUTF16 ToCharUTF16();
	int ToCharBig5(char*);   // the buffer must be an array of length >= 2, memory fault is the caller's fault
	
protected:
	unsigned char code;
};

const int VCMaxCJStringLength=5;

class VCCJString
{
public:
	VCCJString();
	
	int AppendCode(VCCJCode&);		// returns the length after appending (0=failed)
 	int ChopCode();					// returns the length after chopping
	int IsEmpty();
	int Length()	{ return stringlen; }

	void Clean();

	int ToStringBig5(char*);		// returns the length (in bytes!)
	int ToStringUTF16(VCUTF16*);	// returns the length (in UTF16 characters)
	
	// no memory boundry check, so the caller must prepare a big enough VCUTF16 string
	int SearchCandidate(VCUTF16*);  // returns the length of candiate list
	
	// NOT IMPLEMENTED YET
	int SearchCandidateJianyi(VCUTF16*);	// Jianyi method
	// int SearchCandidateWildcard(VCUTF16*);   // not implemented
	
protected:
	VCCJCode codestring[VCMaxCJStringLength];
	int stringlen;
};

#endif		// of #ifndef __vanillacj_h
