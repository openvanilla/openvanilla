//#define OV_DEBUG
#include <vector>
//#include <windows.h>
#include "PCMan.h"
#include "AVConfig.h"

#pragma managed

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
//<comment author='b6s'>
//If you want to use "#using" here, the project property
//"Resolve #using referecen" should also be set to
//"..\CSharpFormLibrary\bin\Debug"
//#using <CSharpFormLibrary.dll>
//using namespace CSharpFormLibrary;
//</comment>

using namespace System;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections;
using namespace std;

ref class CSharpStatusWrapper{
private:
	static CSharpFormLibrary::IMEStatusForm^ m_instance = nullptr;

public:
	static CSharpFormLibrary::IMEStatusForm^ Instance()
	{		
		if(m_instance == nullptr)
			m_instance = gcnew CSharpFormLibrary::IMEStatusForm();
		return m_instance;		
	}
};

HWND _CreateStatusPage() //create
{		
	return (HWND)(safe_cast<IntPtr^>(
		CSharpStatusWrapper::Instance()->GetHandle())->ToPointer());
}

void _SetStatusAppHWnd(HWND hwnd)
{	
	CSharpStatusWrapper::Instance()->SetAppHWnd(safe_cast<IntPtr>(hwnd));
}

void _ShowStatusPage()
{	
	CSharpStatusWrapper::Instance()->ShowNoActive();	
}

void _MoveStatusPage(int x,int y)
{	
	CSharpStatusWrapper::Instance()->SetLocation(x,y);	
}

void _SetStatusChiEng(bool isChinese)
{	
	CSharpStatusWrapper::Instance()->SetChiEng(isChinese);	
}

void _SetStatusSimpifiedOrTraditional(bool isTraditional)
{		
	CSharpStatusWrapper::Instance()->SetSimpifiedOrTraditional(isTraditional);	
}

void _SetStatusModString(int index)
{
	CSharpStatusWrapper::Instance()->SetModString(index);		
}

void _ClearStatusMenuModString()
{
	CSharpStatusWrapper::Instance()->ClearMenuModString();	
}

void _SetStatusMenuModString(const std::wstring& statusModName)
{
	CSharpStatusWrapper::Instance()->SetMenuModString(
		gcnew String(statusModName.c_str()));	
}

void _HideStatusPage()
{
	CSharpStatusWrapper::Instance()->HideNoActive();	
}

int _GetStatusSelectedModuleIndex()
{
	return CSharpStatusWrapper::Instance()->GetSelectedModuleIndex();	
}

void _SetUserDir()
{	
	AVConfig *cfg= new AVConfig();
	const char *baseDir=cfg->getBaseDir();
	const char *moduleDir=cfg->getModuleDir();
	const char *userDir=cfg->getUserDir();
	CSharpStatusWrapper::Instance()->
		SetDir(gcnew String(baseDir),gcnew String(moduleDir),gcnew String(userDir));		
}

void _RotateStatusSelectedModule()
{
	CSharpStatusWrapper::Instance()->RotateModule();	
}
