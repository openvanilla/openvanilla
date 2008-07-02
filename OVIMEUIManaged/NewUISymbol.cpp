//#define OV_DEBUG  
#include <vector>
#include "OVIMEUI.h"
//#include <windows.h>
//#include "PCMan.h"

#pragma managed

//<comment author='b6s'>
//If you want to use "#using" here instead of adding references in
//the project property, another project property "Resolve #using referecen"
//should also be set to "..\OVManagedUI\bin\Debug"
//#using <mscorlib.dll>
//#using <System.dll>
//#using <System.Windows.Forms.dll>
//#using <OVManagedUI.dll>
//</comment>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections;
using namespace std;
//using namespace OVManagedUI;
using namespace OVManagedUI;

ref class CSharpSymbolWrapper{
private:
	static OVManagedUI::IMECandidateForm^ m_instance = nullptr;

public:
	static OVManagedUI::IMECandidateForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew OVManagedUI::IMECandidateForm();
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

void _MoveSymbolPage(int x, int y, int compHeight)
{
	CSharpSymbolWrapper::Instance()->SetLocation(x, y, compHeight);
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
