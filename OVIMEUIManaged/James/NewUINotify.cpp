//#define OV_DEBUG  
#include <vector>
//#include <windows.h>
//#include "PCMan.h"
#include "OVIMEUI.h"


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
//using namespace CSharpFormLibrary;
using namespace OVManagedUI;

ref class CSharpNotifyWrapper{
private:
	static IMENotifyForm^ m_instance = nullptr;

public:
	static IMENotifyForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew IMENotifyForm();
		return m_instance;
	}
};

HWND _CreateNotifyPage()//create
{
	return (HWND)(safe_cast<IntPtr^>(
		CSharpNotifyWrapper::Instance()->Handle)->ToPointer());
}

HWND _CreateNotifyPageWithHandle(HWND hwnd)//create
{
	CSharpNotifyWrapper::Instance()->SetHWND(safe_cast<IntPtr>(hwnd));
	return (HWND)(safe_cast<IntPtr^>(
		CSharpNotifyWrapper::Instance()->Handle)->ToPointer());
}

void _SetNotifyString(const wchar_t* notifyStr)
{
	CSharpNotifyWrapper::Instance()->SetNotifyStr(gcnew String(notifyStr));
}

void _ShowNotifyPage()
{
	CSharpNotifyWrapper::Instance()->ShowNoActive();
}

void _MoveNotifyPage(int x,int y)
{
	CSharpNotifyWrapper::Instance()->SetLocation(x, y);
}

void _HideNotifyPage()
{
	CSharpNotifyWrapper::Instance()->HideNoActive();
}

void _ClearNotifyPage()
{
	CSharpNotifyWrapper::Instance()->ClearNotify();
}

void _ExpandNotify() //®i¶} Notify window
{
	//Not implemented?
}

void _EndNotifyPage()
{
	CSharpNotifyWrapper::Instance()->DisposeForm();
}
int _GetNotifyWidth()
{
	return CSharpNotifyWrapper::Instance()->GetWidth();
}
int _GetNotifyHeight()
{
	return CSharpNotifyWrapper::Instance()->GetHeight();
}

void _SetNotifyFont(const wchar_t * ff, int fontsize)
{
	CSharpNotifyWrapper::Instance()->SetFont(gcnew String(ff), fontsize);	
}

void _SetNotifyColor(int forecolor, int backcolor)
{
	CSharpNotifyWrapper::Instance()->SetColor(forecolor, backcolor);	
}

void _SetNotifyEnabled(bool b)
{
	CSharpNotifyWrapper::Instance()->SetNotifyEnabled(b);	
}
void _SetNotifyOpacity(int o)
{
	CSharpNotifyWrapper::Instance()->SetOpacity(o);	
}