//#define OV_DEBUG  
#include <vector>
//#include <windows.h>
#include "PCMan.h"
#include "AVConfig.h"

#pragma managed

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
//#using <CSharpFormLibrary.dll>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace std;
//using namespace CSharpFormLibrary;

ref class CSharpCandiWrapper{
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

HWND _CreateCandiPage() //create without handle (x)
{	
	return (HWND)(safe_cast<IntPtr^>(
		CSharpCandiWrapper::Instance()->GetHandle())->ToPointer());
}

HWND _CreateCandiPageWithHandle(HWND hwnd) //create with handle (x)
{	
	CSharpCandiWrapper::Instance()->SetHWND(safe_cast<IntPtr>(hwnd));
	return (HWND)(safe_cast<IntPtr^>(
		CSharpCandiWrapper::Instance()->GetHandle())->ToPointer());
}

void _SetCandiString(const wstring& candidate)
{		
	CSharpCandiWrapper::Instance()
		->SetCandidates(gcnew String(candidate.c_str()));
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
