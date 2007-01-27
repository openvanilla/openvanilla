//#define OV_DEBUG  
#include <vector>
//#include <windows.h>
#include "PCMan.h"
#include "AVConfig.h"

#pragma managed

//<comment author='b6s'>
//If you want to use "#using" here instead of adding references in
//the project property, another project property "Resolve #using referecen"
//should also be set to "..\CSharpFormLibrary\bin\Debug"
//#using <mscorlib.dll>
//#using <System.dll>
//#using <System.Windows.Forms.dll>
//#using <CSharpFormLibrary.dll>
//</comment>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections;
using namespace std;
using namespace CSharpFormLibrary;

ref class CSharpSymbolWrapper{
private:
	static CSharpFormLibrary::IMECandidateForm^ m_instance = nullptr;

public:
	static CSharpFormLibrary::IMECandidateForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew CSharpFormLibrary::IMECandidateForm();
		return m_instance;
	}
};

HWND _CreateSymbolPage()//create without handle
{		
	return (HWND)(safe_cast<IntPtr^>(
		CSharpSymbolWrapper::Instance()->Handle)->ToPointer());
}

HWND _CreateSymbolPageWithHandle(HWND hwnd)//create with handle
{	
	CSharpSymbolWrapper::Instance()->SetHWND(safe_cast<IntPtr>(hwnd));
	return (HWND)(safe_cast<IntPtr^>(
		CSharpSymbolWrapper::Instance()->Handle)->ToPointer());
}

void _SetSymbolString(const wchar_t* symbolStr)
{		
	CSharpSymbolWrapper::Instance()
		->SetCandidates(gcnew String(symbolStr));
}

void _ShowSymbolPage()
{		
	CSharpSymbolWrapper::Instance()->ShowNoActive();
}

void _MoveSymbolPage(int x, int y)
{
	CSharpSymbolWrapper::Instance()->SetLocation(x, y);
}

void _HideSymbolPage()
{
	CSharpSymbolWrapper::Instance()->HideNoActive();
}

void _ClearSymbolPage()
{
	CSharpSymbolWrapper::Instance()->ClearCandidates();
}

void _EndSymbolPage()
{
	CSharpSymbolWrapper::Instance()->DisposeForm();	
}

void _ExpandSymbol() //®i¶} Symbol window
{
	CSharpSymbolWrapper::Instance()->SwitchCandiMode();	
}
