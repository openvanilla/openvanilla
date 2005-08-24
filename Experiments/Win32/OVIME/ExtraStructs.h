#define MAXSTRSIZE (1024)
#define MAXCOMPSIZE (1024)
#define MAXCANDSTRNUM (40)
#define MAXCANDSTRSIZE (32)

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
    wchar_t szResultStr[MAXCOMPSIZE];	
} MYCOMPSTR, *LPMYCOMPSTR;

typedef struct _tagMYCAND{
    CANDIDATEINFO  ci;
    CANDIDATELIST  cl;
    DWORD          offset[MAXCANDSTRNUM];
    wchar_t          szCandStr[MAXCANDSTRNUM][MAXCANDSTRSIZE];
} MYCAND, *LPMYCAND;