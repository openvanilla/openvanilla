#include "OVIME2.h"
#include ".\compstr.h"

CompStr::CompStr(void)
{
	cs.dwSize = sizeof(CompStr);
	cs.dwDeltaStart = 0;
	cs.dwCursorPos = 0;

	cs.dwCompReadStrOffset = (BYTE*)&readStr[0] - (BYTE*)this;
	cs.dwCompReadStrLen = 0;
	memset( readStr, 0, sizeof(readStr) );

	cs.dwCompReadAttrOffset = (BYTE*)&readAttr[0] - (BYTE*)this;
	cs.dwCompReadAttrLen = 0;
	memset( readAttr, 0, sizeof(readAttr) );

	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//cs.dwCompReadClauseOffset = DWORD( (BYTE*)&readClause[0] - (BYTE*)this);
	//cs.dwCompReadClauseLen = 0;
	cs.dwCompReadClsOffset = DWORD( (BYTE*)&readClause[0] - (BYTE*)this);
	cs.dwCompReadClsLen = 0;
	//</comment>
	memset( readClause, 0, sizeof(readClause) );


	cs.dwCompStrOffset = (BYTE*)&compStr[0] - (BYTE*)this;
	cs.dwCompStrLen = 0;
	memset( compStr, 0, sizeof(compStr) );

	cs.dwCompAttrOffset = (BYTE*)&compAttr[0] - (BYTE*)(this);
	cs.dwCompAttrLen = 0;
	memset( compAttr, 0, sizeof(compAttr) );

	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//cs.dwCompClauseOffset = DWORD( (BYTE*) &compClause[0] - (BYTE*)this);
	//cs.dwCompClauseLen = 0;
	cs.dwCompClsOffset = DWORD( (BYTE*) &compClause[0] - (BYTE*)this);
	cs.dwCompClsLen = 0;
	//</comment>
	memset( compClause, 0, sizeof(compClause) );


	cs.dwResultReadStrOffset = (BYTE*)&resultReadStr[0] - (BYTE*)this;
	cs.dwResultReadStrLen = 0;
	memset( resultReadStr, 0, sizeof(resultReadStr) );

	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//cs.dwResultClauseOffset = DWORD( (BYTE*)&resultClause[0] - (BYTE*)this);
	//cs.dwResultClauseLen = 0;
	cs.dwResultClsOffset = DWORD( (BYTE*)&resultClause[0] - (BYTE*)this);
	cs.dwResultClsLen = 0;
	//</comment>
	memset( resultClause, 0, sizeof(resultClause) );

	cs.dwResultStrOffset = (BYTE*)&resultStr[0] - (BYTE*)this;
	cs.dwResultStrLen = 0;
	memset( resultStr, 0, sizeof(resultStr) );

	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//cs.dwResultClauseOffset = DWORD( (BYTE*)&resultClause[0] - (BYTE*)this);
	//cs.dwResultClauseLen = 0;
	cs.dwResultClsOffset = DWORD( (BYTE*)&resultClause[0] - (BYTE*)this);
	cs.dwResultClsLen = 0;
	//</comment>
	memset( resultClause, 0, sizeof(resultClause) );


	cs.dwPrivateOffset = DWORD( (BYTE*)&showMsg[0] - (BYTE*)this);
	cs.dwPrivateSize = sizeof(showMsg);
	memset( showMsg, 0, sizeof(showMsg) );
}

CompStr::~CompStr(void)
{
}

void CompStr::setCompStr(LPCWSTR comp_str)
{
	wcscpy( compStr, comp_str );
	cs.dwCompStrLen = wcslen( compStr );
	cs.dwCompAttrLen = cs.dwCompStrLen;
	memset( (char*)compAttr, ATTR_CONVERTED, cs.dwCompAttrLen );
}

void CompStr::setShowMsg(LPCWSTR show_msg)
{
	wcscpy( showMsg, show_msg );
}

void CompStr::setResultStr(LPCWSTR result_str)
{
	wcscpy( resultStr, result_str );
	cs.dwResultStrLen = wcslen( resultStr );
	//<comment author='b6s'> Change to MinGW imm header's attribute name
	//cs.dwResultClauseLen = sizeof(resultClause);
	cs.dwResultClsLen = sizeof(resultClause);
	//</comment>
	resultClause[0] = 0;
	resultClause[1] = cs.dwResultStrLen;
	cs.dwResultReadStrLen = 0;
}

void CompStr::setCursorPos(int pos)
{
	// ATTR_INPUT 	Character currently being entered.
	// ATTR_TARGET_CONVERTED 	Character currently being converted (already converted).
	// ATTR_CONVERTED 	Character given from the conversion.
	// ATTR_TARGET_NOTCONVERTED 	Character currently being converted (yet to be converted).

	memset( &compAttr[cs.dwCursorPos], ATTR_CONVERTED, 1 );

	cs.dwCursorPos = pos;

	memset( &compAttr[pos], ATTR_TARGET_CONVERTED, 1 );
}

void CompStr::setZuin(LPCWSTR zuin)
{
	wcscpy( readStr, zuin );
	cs.dwCompReadStrLen = wcslen(readStr);

	cs.dwCompReadAttrLen = cs.dwCompReadStrLen;
	if(cs.dwCompReadStrLen)
		memset( (char*)readAttr, ATTR_TARGET_NOTCONVERTED, cs.dwCompReadStrLen );
}

void CompStr::beforeGenerateMsg(void)
{
	wchar_t* sinsert = compStr + cs.dwCursorPos;

	if(cs.dwCompStrLen > cs.dwCursorPos)
		memmove( sinsert + cs.dwCompReadStrLen, 
			sinsert, cs.dwCompStrLen - cs.dwCursorPos );		
	else
		memmove( sinsert + cs.dwCompReadStrLen,
			sinsert, cs.dwResultStrLen);
	wcsncpy( sinsert, readStr, cs.dwCompReadStrLen );
	cs.dwCompStrLen += cs.dwCompReadStrLen;
	compStr[cs.dwCompStrLen] = '\0';

	BYTE* ainsert = compAttr + cs.dwCursorPos;
	if(cs.dwCompAttrLen > cs.dwCursorPos)
		memmove( ainsert + cs.dwCompReadAttrLen, 
			ainsert, cs.dwCompAttrLen - cs.dwCursorPos);
	else
		memmove( ainsert + cs.dwCompReadAttrLen, 
			ainsert, cs.dwCompReadAttrLen);
	memcpy( ainsert, readAttr, cs.dwCompReadAttrLen );
	cs.dwCompAttrLen += cs.dwCompReadAttrLen;

	cs.dwCompReadStrLen = cs.dwCompReadAttrLen = 0;

	compClause[0] = 0;
	compClause[1] = cs.dwCompStrLen;
	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//cs.dwCompClauseLen = 0;//sizeof(compClause);
	cs.dwCompClsLen = 0;
	//</comment>

	resultClause[0] = 0;
	resultClause[1] = cs.dwResultStrLen;
	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//cs.dwResultClauseLen = 0;//sizeof(resultClause);
	cs.dwResultClsLen = 0;
	//</comment>

	readClause[0] = 0;
	readClause[1] = cs.dwCompReadStrLen;
	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//cs.dwCompReadClauseLen = 0;//sizeof(readClause);
	cs.dwCompReadClsLen = 0;
	//</comment>

	resultReadClause[0] = 0;
	//<comment author='b6s'> Change to MinGW imm header's attribute names
	//resultReadClause[1] = cs.dwResultReadClauseLen = 0;
	//cs.dwResultReadClauseLen = 0;//sizeof(resultReadClause);
	resultReadClause[1] = cs.dwResultReadClsLen = 0;
	cs.dwResultReadClsLen = 0;
	//</comment>
	

	if( g_useUnicode )	{
		if( compStr[0] == 0 )	// If quick commit
			//<comment author='b6s'> Change to MinGW imm header's attribute names
			//cs.dwCompClauseLen = 0;	// No clause info
			cs.dwCompClsLen = 0;
			//</comment>
		else	{	// This composition string contains Chinese characters
			int i;
			wchar_t* pstr = compStr;
			for( i = 0; i < cs.dwCompStrLen; ++i )	{
				compClause[ i ] = (pstr - compStr);
				++pstr;
				if( ! *pstr )
					break;
			}
			compClause[++i] = cs.dwCompStrLen;
			//<comment author='b6s'> Change to MinGW imm header's attribute names
			//cs.dwCompClauseLen = (i+1) * sizeof(DWORD);
			cs.dwCompClsLen = (i+1) * sizeof(DWORD);
			//</comment>
		}

		if( resultStr[0] == 0 )	// If no result string
			//<comment author='b6s'> Change to MinGW imm header's attribute names
			//cs.dwResultClauseLen = 0;	// No clause info
			cs.dwResultClsLen = 0;
			//</comment>
		else	{	// This result string contains Chinese characters
			int i;
			wchar_t* pstr = resultStr;
			for( i = 0; i < cs.dwResultStrLen; ++i )	{
				resultClause[ i ] = (pstr - resultStr);
				pstr = ++pstr;
				if( ! *pstr )
					break;
			}
			resultClause[++i] = cs.dwResultStrLen;
			//<comment author='b6s'> Change to MinGW imm header's attribute names
			//cs.dwResultClauseLen = (i+1) * sizeof(DWORD);
			cs.dwResultClsLen = (i+1) * sizeof(DWORD);
			//</comment>
		}
	}
	else	// ANSI version
	{
		// We have to convert all unicode data to ANSI here.
		// Since under Win 95/NT4, there is no unicode IME support.

/*		wchar_t* wsinsert = compStr + cs.dwCursorPos;
		int winsert_len = wcslen( sinsert );
		TCHAR sinsert[ sizeof(compStr) ];
		WideCharToMultiByte( CP_ACP, 0, sinsert, winsert_len, sinsert, sizeof(), ' ', NULL );
		cs.dwCompReadStrLen = _tcslen( sinsert );

		if( compStr[0] == 0 )	// If quick commit
			cs.dwCompClauseLen = 0;	// No clause info
		else	{	// This composition string contains Chinese characters
			int i;
			TCHAR* pstr = compStr;
			for( i = 0; i < cs.dwCompStrLen; ++i )	{
				compClause[ i ] = (pstr - compStr);
				pstr = _tcsinc( pstr );
				if( ! *pstr )
					break;
			}
			compClause[++i] = cs.dwCompStrLen;
			cs.dwCompClauseLen = (i+1) * sizeof(DWORD);
		}

		if( resultStr[0] == 0 )	// If no result string
			cs.dwResultClauseLen = 0;	// No clause info
		else	{	// This result string contains Chinese characters
			int i;
			TCHAR* pstr = resultStr;
			for( i = 0; i < cs.dwResultStrLen; ++i )	{
				resultClause[ i ] = (pstr - resultStr);
				pstr = _tcsinc( pstr );
				if( ! *pstr )
					break;
			}
			resultClause[++i] = cs.dwResultStrLen;
			cs.dwResultClauseLen = (i+1) * sizeof(DWORD);
		}
*/
	}
}

void CompStr::setInvervalAry( char* intervalStr ) {
	for( int i = 0; i < strlen( intervalStr ); i++ ) {
		IntervalAry[i] = intervalStr[i] - '0';
	}
}
