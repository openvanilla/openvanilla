#include <vector>
//#include <windows.h>
#include "PCMan.h"

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

ref class CSharpCompWrapper{
private:
	static IMECompRichForm^ m_instance = nullptr;

public:
	static IMECompRichForm^ Instance()
	{
		if(m_instance == nullptr)
			m_instance = gcnew IMECompRichForm();
		return m_instance;
	}
};

HWND _CreateCompPage()//create
{
	return (HWND)(safe_cast<IntPtr^>(
		CSharpCompWrapper::Instance()->Handle)->ToPointer());
}

void _SetCompString(const wchar_t* compStr)
{
	CSharpCompWrapper::Instance()->SetComp(gcnew String(compStr));
}

void _SetCompCaretPosX(int x)
{
	CSharpCompWrapper::Instance()->SetCaretX(x);
}

void _SetCompMarkFrom(int x)
{
	CSharpCompWrapper::Instance()->SetCompMarkFrom(x);
}

void _SetCompMarkTo(int x)
{
	CSharpCompWrapper::Instance()->SetCompMarkTo(x);
}

void _ShowCompPage()
{
	CSharpCompWrapper::Instance()->ShowNoActive();
}

void _MoveCompPage(int x,int y)
{
	CSharpCompWrapper::Instance()->SetLocation(x, y);
}

void _HideCompPage()
{
	CSharpCompWrapper::Instance()->HideNoActive();
}

void _ClearCompPage()
{
	CSharpCompWrapper::Instance()->ClearComp();
}

void _EndCompPage()
{
	CSharpCompWrapper::Instance()->DisposeForm();
}

int _GetCompValue()
{
	return 0;
}

int _GetHeight()
{
	return CSharpCompWrapper::Instance()->Height;
}

int _GetCaretPosX()
{
	return CSharpCompWrapper::Instance()->GetCaretPosX();
}
