// vanillacj.cpp

#include <stdio.h>
#include <stdlib.h>
#include "vanillacj.h"

// internal constants, masks, data types/representations, 
// and string data

const unsigned char vcWildcard=27;

char vcCJCodeCharBig5[54]=
{
	0xa4, 0xe9, 0xa4, 0xeb, 0xaa, 0xf7, 0xa4, 0xec, 0xa4, 0xf4, // ABCDE
	0xa4, 0xf5, 0xa4, 0x67, 0xa6, 0xcb, 0xa4, 0xe0, 0xa4, 0x51, // FGHIJ
	0xa4, 0x6a, 0xa4, 0xa4, 0xa4, 0x40, 0xa4, 0x7d, 0xa4, 0x48, // KLMNO
	0xa4, 0xdf, 0xa4, 0xe2, 0xa4, 0x66, 0xa4, 0x72, 0xa4, 0xdc, // PQRST
	0xa4, 0x73, 0xa4, 0x6b, 0xa5, 0xd0, 0xc3, 0xf8, 0xa4, 0x52, // UVWXY
	0xad, 0xab, 0xa1, 0x48										// Z?
};
  
VCUTF16 vcCJCodeCharUTF16[27]=
{
	0x65e5, 0x6708, 0x91d1, 0x6728, 0x6c34, 0x706b, 0x571f,		// ABCDEFG
	0x7af9, 0x6208, 0x5341, 0x5927, 0x4e2d, 0x4e00, 0x5f13,		// HIJKLMN
	0x4eba, 0x5fc3, 0x624b, 0x53e3, 0x5c38, 0x5eff, 0x5c71,		// OPQRSTU
	0x5973, 0x7530, 0x96e3, 0x535c, 0x91cd, 0xff1f				// VWXYZ?
};

VCCJCode::VCCJCode(char keychar, int isWildcard)
{
	if (isWildcard) code=vcWildcard;
	else KeyToCode(keychar);
}

int VCCJCode::KeyToCode(char keychar)
{
	unsigned char i=(((unsigned char)keychar) | 0x20) - 0x61;
	if (i<26)
	{
		code=i+1;
		return 1;
	}

	code=0;
	// fprintf (stderr, "keychar=%04x, i=%04x, code=%04x\n",keychar,i,code);
	return 0;
}

int VCCJCode::IsValidCode()
{
	if (code && (code < vcWildcard+1)) return 1;
	
	// invalid code, we automatically reset code to 0
	code=0;
	return 0;
}

int VCCJCode::IsWildcard()
{
	if (code==vcWildcard) return 1;
	return 0;
}

VCUTF16 VCCJCode::ToCharUTF16()
{
	if (!IsValidCode()) return 0;
	return vcCJCodeCharUTF16[code-1];
}

int VCCJCode::ToCharBig5(char*p)
{
	if (!IsValidCode()) return 0;
	
	int index=(code-1) << 1;		// (code-1)*2
	*p++ = vcCJCodeCharBig5[index];
	*p = vcCJCodeCharBig5[index+1];
	return 2;
}

VCCJString::VCCJString()
{
	stringlen=0;
}
	
int VCCJString::AppendCode(VCCJCode& c)
{
	if (!c.IsValidCode()) return 0;
	if (stringlen==VCMaxCJStringLength) return 0;
	codestring[stringlen++]=c;
	return stringlen;
}

int VCCJString::ChopCode()
{
	if (stringlen) stringlen--;
	return stringlen;
}

int VCCJString::IsEmpty()
{
	if (!stringlen) return 1;
	return 0;
}

void VCCJString::Clean()
{
	stringlen=0;
}

int VCCJString::ToStringBig5(char *s)
{
	for (int i=0; i<stringlen; i++)
	{
		codestring[i].ToCharBig5(s);
		s+=2;
	}
	return stringlen*2;
}

int VCCJString::ToStringUTF16(VCUTF16 *s)
{
	for (int i=0; i<stringlen; i++) s[i]=codestring[i].ToCharUTF16();
	return stringlen;
}

// our search function begins here

struct VCCJTableData
{
	unsigned long key;
	unsigned short data;
};

extern int vcCJTableSize;
extern VCCJTableData vcCJTableData[];

const unsigned long vcHasMoreDataMask = 0x40000000;  // 1 << 30
const unsigned long vcNotHasMoreDataMask = ~vcHasMoreDataMask;
const unsigned long vcEndMask = 0xffffffff;

int vcCJTableDataCompare(const void *e1, const void *e2)
{
	VCCJTableData *d1=(VCCJTableData*)e1, *d2=(VCCJTableData*)e2;
	
	unsigned long k1=d1->key, k2=d2->key;
	unsigned long mk1=k1 & vcNotHasMoreDataMask, mk2 = k2 & vcNotHasMoreDataMask;
	
	// printf (" CMP (%08x,%08x)(%08x,%08x)\n",k1,k2,mk1,mk2);
	
	if (mk1 < mk2) return -1;
	if (mk1 > mk2) return 1;

	// if k1 has the mask, k1 is deinitely bigger than k2	
	if (k1 == k2) return 0;
	if (k1 < k2) return -1;
	return 1;
}


int VCCJString::SearchCandidate(VCUTF16 *s)
{
	int numberfound=0;
	VCCJTableData *start=vcCJTableData, *found;
	VCCJTableData search;
	
	search.key=0;
	search.data=0;
	
	for (int i=0; i<stringlen; i++)
	{
		search.key |= codestring[i].code << (4-i)*5;
	}
	
	// printf ("DEBUG, size=%d, startkey=%08x\n", vcCJTableSize, start->key);

	found=(VCCJTableData*)bsearch(&search, start, vcCJTableSize, sizeof(VCCJTableData), vcCJTableDataCompare);
	if (!found) return 0;
	
	// printf ("DEBUG found, key=%08x\n", found->key);
	
	while (1)
	{
		if (found->key == vcEndMask) break;
		
		// printf ("FOUND %08x SEARCH %08x\n", found->key & vcNotHasMoreDataMask, search.key & vcNotHasMoreDataMask);
		if ((found->key & vcNotHasMoreDataMask) != (search.key & vcNotHasMoreDataMask)) break;
		s[numberfound++]=found->data;
		found++;
	}
	
	return numberfound;
}

extern short vcSXKeyTable[];
extern unsigned short vcSXCharData[];

int VCCJString::SearchCandidateJianyi(VCUTF16* s)
{
    if (stringlen > 2) return 0;    // impossible to have more than 2 codes
    
    int sxcode=(codestring[0].code-1)*27;
    if (stringlen==2) sxcode+=codestring[1].code;
    
    int datafrom=vcSXKeyTable[sxcode], datato=vcSXKeyTable[sxcode+1];
    
    int copyed=0;
    for (int i=datafrom; i<datato; i++)
    {
        s[copyed++]=vcSXCharData[i];
    }
    return copyed;
}

// internal data structure for Cangjei
// DEFINE CANGJEIRECORD=((nextcharflag, code0, code1, code2, code3, code4), char)
// e.g. for code "A" (standing for two characters: Ri4 [sun] and Yue [saying]
//		((0, 1, 0, 0, 0, 0), Ri4)
//		((1, 1, 0, 0, 0, 0), Yue)
// the last record is marked as (0xffffffff, 0xffff)

// internal data structure for Jianyi
// DEFINE CHARTABLE=(char0, char1, ..., 0xffff)
// DEFINE KEYTABLE=ARRAY[(26+1)*(26)+1], the last record KEYTABLE[last]=lastcharindex
// where for any key combination (X,Y) (no wildcard allowed),
// the key value is always (keycode(X)-1)*(26+1)+keycode(Y))
// e.g. (A,undef)=0, (A,A)=1, (A,Z)=26, (B,undef)=27, (B,A)=28...
