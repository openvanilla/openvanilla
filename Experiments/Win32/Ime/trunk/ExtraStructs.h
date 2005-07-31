#define MAXSTRSIZE (1024)
#define MAXCOMPSIZE (1024)
#define MAXCANDSTRNUM (40)
#define MAXCANDSTRSIZE (32)

typedef struct _tagTRANSMSG {
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
} TRANSMSG, FAR *LPTRANSMSG;

typedef struct _tagUICHILD{
    HWND    hWnd;
	POINT   pt;
	SIZE    sz;
} UICHILD, NEAR *PUICHILD, FAR *LPUICHILD;

typedef struct _tagUIEXTRA{
    UICHILD  uiStatus;
    UICHILD  uiCand;
	UICHILD  uiComp;
} UIEXTRA, NEAR *PUIEXTRA, FAR *LPUIEXTRA;

// IMPORTANT
typedef struct _tagMYPRIVATE {
	// This private is used for exchanging data between
	// IME conversion interface and the UI interface.
	// Please understand how to use:
	// In ImeInquire: 
	// set lpIMEInfo->dwPrivateDataSize = sizeof(MYPRIVATE);
	// Then every IMC generated will share this private, access
	// through IMC->hPrivate field
	_TCHAR PreEditStr[MAXSTRSIZE];
	_TCHAR CompStr[MAXSTRSIZE];
} MYPRIVATE, NEAR *PMYPRIVATE, FAR *LPMYPRIVATE;

typedef struct _tagMYCOMPSTR{
    COMPOSITIONSTRING cs;
    _TCHAR szCompStr[MAXCOMPSIZE];
    _TCHAR szResultStr[MAXCOMPSIZE];	
} MYCOMPSTR, NEAR *PMYCOMPSTR, FAR *LPMYCOMPSTR;

typedef struct _tagMYCAND{
    CANDIDATEINFO  ci;
    CANDIDATELIST  cl;
    DWORD          offset[MAXCANDSTRNUM];
    TCHAR          szCandStr[MAXCANDSTRNUM][MAXCANDSTRSIZE];
} MYCAND, NEAR *PMYCAND, FAR *LPMYCAND;