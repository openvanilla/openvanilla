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

ref class CSharpCandiWrapper{
private:
	static IMECandidateForm^ m_instance = nullptr;

public:
	static IMECandidateForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew IMECandidateForm();
		return m_instance;
	}
};

HWND _CreateCandiPage() //create without handle (x)
{	
	return (HWND)(safe_cast<IntPtr^>(
		CSharpCandiWrapper::Instance()->Handle)->ToPointer());
}

HWND _CreateCandiPageWithHandle(HWND hwnd) //create with handle (x)
{	
	CSharpCandiWrapper::Instance()->SetHWND(safe_cast<IntPtr>(hwnd));
	return (HWND)(safe_cast<IntPtr^>(
		CSharpCandiWrapper::Instance()->Handle)->ToPointer());
}

void _SetCandiString(const wchar_t* candiStr)
{		
	CSharpCandiWrapper::Instance()
		->SetCandidates(gcnew String(candiStr));
}

void _ShowCandiPage()
{		
	CSharpCandiWrapper::Instance()->ShowNoActive();
}

void _MoveCandiPage(int x, int y)
{
	CSharpCandiWrapper::Instance()->SetLocation(x, y);
}

void _HideCandiPage()
{
	CSharpCandiWrapper::Instance()->HideNoActive();
}

void _ClearCandiPage()
{
	CSharpCandiWrapper::Instance()->ClearCandidates();
}

void _EndCandiPage()
{
	CSharpCandiWrapper::Instance()->DisposeForm();
}

void _ExpandCandi() //®i¶} Candi window
{
	CSharpCandiWrapper::Instance()->SwitchCandiMode();	
}
