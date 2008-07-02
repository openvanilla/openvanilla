#ifndef ExtraStructs_h
#define ExtraStructs_h

#define MAXSTRSIZE (1024)
#define MAXCOMPSIZE (1024)
#define MAXCANDSTRNUM (256)
#define MAXCANDSTRSIZE (16)

typedef struct _tagTRANSMSG {
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
} TRANSMSG, *LPTRANSMSG;

// IMPORTANT
typedef struct _tagMYPRIVATE {
	// This private is used for exchanging data between
	// IME conversion interface and the UI interface.
	// Please understand how to use:
	// In ImeInquire: 
	// set lpIMEInfo->dwPrivateDataSize = sizeof(MYPRIVATE);
	// Then every IMC generated will share this private, access
	// through IMC->hPrivate field
	wchar_t PreEditStr[MAXSTRSIZE];
	wchar_t CandStr[MAXSTRSIZE];
} MYPRIVATE, *LPMYPRIVATE;

typedef struct _tagMYCOMPSTR{
    COMPOSITIONSTRING cs;

    wchar_t szCompStr[MAXCOMPSIZE];
	BYTE compAttr[ MAXCOMPSIZE/sizeof(TCHAR) ];
	//DWORD compClause[MAXCOMPSIZE+1];
	DWORD compClause[2];

    wchar_t szResultStr[MAXCOMPSIZE];	
	DWORD resultClause[MAXCOMPSIZE+1];
	
	
	wchar_t szReadStr[MAXSTRSIZE];	
	BYTE readAttr[ MAXSTRSIZE/sizeof(TCHAR) ];
	DWORD readClause[2];

	wchar_t szResultReadStr[MAXSTRSIZE];	
	DWORD resultReadClause[2];
	
} MYCOMPSTR, *LPMYCOMPSTR;

typedef struct _tagMYCAND{
    CANDIDATEINFO  ci;
    CANDIDATELIST  cl;
    DWORD          offset[MAXCANDSTRNUM];
    wchar_t          szCandStr[MAXCANDSTRNUM][MAXCANDSTRSIZE];
} MYCAND, *LPMYCAND;

#endif // ExtraStructs_h
