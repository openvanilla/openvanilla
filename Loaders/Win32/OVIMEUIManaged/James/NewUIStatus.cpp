#define OV_DEBUG
#include <vector>
//#include <windows.h>
#include "PCMan.h"
#include "AVDictionary.h"
#include "AVConfig.h"


#pragma managed

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
using namespace System;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Collections;
using namespace std;

__gc class StatusFormAssembly{
private:
	StatusFormAssembly(){}
	static Assembly* pasm;
	static bool hasPasm = false;

	static Object* objStatusForm;
	static bool hasObjStatusForm = false;

	static Type* typeStatus;
	static bool hasTypeStatus = false;

	static MethodInfo* methodStatusShowNoActive;
	static bool hasMethodStatusShowNoActive = false;

	static MethodInfo* methodStatusMove;
	static bool hasMethodStatusMove = false;

	static MethodInfo* methodStatusSetChiEng;
	static bool hasMethodStatusSetChiEng = false;

	static MethodInfo* methodStatusSetSimpifiedOrTraditional;
	static bool hasMethodStatusSetSimpifiedOrTraditional = false;	

	static MethodInfo* methodStatusSetDir;
	static bool hasMethodStatusSetDir = false;

	static MethodInfo* methodStatusHide;
	static bool hasMethodStatusHide = false;

	static MethodInfo* methodStatusSetModString;
	static bool hasMethodStatusSetModString = false;

	static MethodInfo* methodStatusSetMenuModString;
	static bool hasMethodStatusSetMenuModString = false;

	static MethodInfo* methodStatusClearMenuModString;
	static bool hasMethodStatusClearMenuModString = false;

	__nogc struct IsFormCreated
	{
		bool isCreated;
		void PutValue(bool flag)
		{
			isCreated = flag;
		}

		bool GetValue()
		{
			return isCreated;
		}
		__declspec(property(get = GetValue, put = PutValue)) bool IsCreated;
	};

public:
	
	static ArrayList* argCollection = __gc new ArrayList();
	static IsFormCreated* FormStatus=new IsFormCreated();
	static Assembly* Instance()
	{
		if(!hasPasm) 
		{
			pasm = Reflection::Assembly::LoadFile("C:\\WINDOWS\\OpenVanilla\\CSharpFormLibrary.dll");	
			hasPasm = true;
		}
		return pasm;
	}
	
	static Object* StatusForm()
	{
		if(!hasObjStatusForm)
		{
			objStatusForm =
				pasm->CreateInstance("CSharpFormLibrary.IMEStatusForm",true);
			hasObjStatusForm = true;
		}
		return objStatusForm;
	}

	static Type* StatusType()
	{
		if(!hasTypeStatus)
		{
			typeStatus =
				StatusFormAssembly::Instance()
					->GetType("CSharpFormLibrary.IMEStatusForm");
			hasTypeStatus = true;
		}
		return typeStatus;
	}

		static MethodInfo* StatusShowNoActive()
	{
		if(!hasMethodStatusShowNoActive)
		{
			methodStatusShowNoActive =
				StatusFormAssembly::StatusType()->GetMethod("ShowNoActive");
			hasMethodStatusShowNoActive = true;
		}
		return methodStatusShowNoActive;
	}

	static MethodInfo* StatusMove()
	{
		if(!hasMethodStatusMove)
		{
			methodStatusMove =
				StatusFormAssembly::StatusType()	->GetMethod("SetLocation");
			hasMethodStatusMove = true;
		}
		return methodStatusMove;
	}

	static MethodInfo* StatusSetChiEng()
	{
		if(!hasMethodStatusSetChiEng)
		{
			methodStatusSetChiEng = 
				StatusFormAssembly::StatusType()->GetMethod("SetChiEng");
			hasMethodStatusSetChiEng = true;
		}
		return methodStatusSetChiEng;
	}

	static MethodInfo* StatusSetSimpifiedOrTraditional()
	{
		if(!hasMethodStatusSetSimpifiedOrTraditional)
		{
			methodStatusSetSimpifiedOrTraditional = 
				StatusFormAssembly::StatusType()->GetMethod("SetSimpifiedOrTraditional");
			hasMethodStatusSetSimpifiedOrTraditional = true;
		}
		return methodStatusSetSimpifiedOrTraditional;
	}
	
	static MethodInfo* StatusSetDir()
	{
		if(!hasMethodStatusSetDir)
		{
			methodStatusSetDir = 
				StatusFormAssembly::StatusType()->GetMethod("SetDir");
			hasMethodStatusSetDir = true;
		}
		return methodStatusSetDir;
	}

	static MethodInfo* StatusSetModString()
	{
		if(!hasMethodStatusSetModString)
		{
			methodStatusSetModString = 
				StatusFormAssembly::StatusType()->GetMethod("SetModString");
			hasMethodStatusSetModString = true;
		}
		return methodStatusSetModString;
	}

	static MethodInfo* StatusSetMenuModString()
	{
		if(!hasMethodStatusSetMenuModString)
		{
			methodStatusSetMenuModString = 
				StatusFormAssembly::StatusType()->GetMethod("SetMenuModString");
			hasMethodStatusSetMenuModString = true;
		}
		return methodStatusSetMenuModString;
	}

	static MethodInfo* StatusClearMenuModString()
	{
		if(!hasMethodStatusClearMenuModString)
		{
			methodStatusClearMenuModString = 
				StatusFormAssembly::StatusType()->GetMethod("ClearMenuModString");
			hasMethodStatusClearMenuModString = true;
		}
		return methodStatusClearMenuModString;
	}

	static MethodInfo* StatusHide()
	{
		if(!hasMethodStatusHide)
		{
			methodStatusHide =
				StatusFormAssembly::StatusType()	->GetMethod("HideNoActive");
			hasMethodStatusHide = true;
		}
		return methodStatusHide;
	}

};

HWND _CreateStatusPage()//create
{	
	//System::Diagnostics::Debug::WriteLine("Status Create");
	//MethodInfo* methodCreateWindow =
	//	StatusFormAssembly::StatusType()->GetMethod("IMEStatusForm");
	
	PropertyInfo* propertyHandle =
		StatusFormAssembly::StatusType()->GetProperty("Handle");
	return
		(HWND)(
			dynamic_cast<IntPtr*>(
				propertyHandle->GetValue(StatusFormAssembly::StatusForm(), NULL))
		)->ToPointer();
	
}

void _SetStatusAppHWnd(HWND hwnd)
{
	murmur("_SetStatusAppHWnd: hwnd -> %x", hwnd);
	try{		
		UInt64 handle = (UInt64)hwnd;		
		StatusFormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(handle)));
		Object* param[] =
			dynamic_cast<Object*[]>(StatusFormAssembly::argCollection
				->ToArray(System::Type::GetType("System.Object")));
		StatusFormAssembly::StatusType()
			->GetMethod("SetAppHWnd")
			->Invoke(StatusFormAssembly::StatusForm(), param);
	} catch(System::Exception* e) {
		Debug::WriteLine(e->StackTrace);
	}
}
void _ShowStatusPage()
{	
	//System::Diagnostics::Debug::WriteLine("Comp Show");
	//try 
	//{
		//System::Reflection::Assembly* foo = CandFormAssembly::Instance();	
		//System::Type* bar = foo->GetType("CSharpFormLibrary.IMECompForm");
		//MethodInfo* methodShowWindow = bar->GetMethod("ShowNoActive");
		//methodShowWindow->Invoke(CandFormAssembly::CandidateForm(), NULL);
		//Object* ret = CandFormAssembly::CandiShowNoActive()
	Object* ret = StatusFormAssembly::StatusShowNoActive()
			->Invoke(StatusFormAssembly::StatusForm(), NULL);
	//}
	/*
	catch(System::Exception* e)
	{
		System::Diagnostics::Debug::WriteLine(e->Source);
		System::Diagnostics::Debug::WriteLine(e->TargetSite);
		System::Diagnostics::Debug::WriteLine(e->HelpLink);
		System::Diagnostics::Debug::WriteLine(e->InnerException);
		System::Diagnostics::Debug::WriteLine(e->Message);
		System::Diagnostics::Debug::WriteLine(e->StackTrace);
	}
	*/
}

void _MoveStatusPage(int x,int y)
{	
	//System::Diagnostics::Debug::WriteLine("Candi Move");
	StatusFormAssembly::argCollection->Clear();
	StatusFormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(x)));
	StatusFormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(y)));	

	Object* param[] =
		dynamic_cast<Object*[]>(StatusFormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	Object* ret =
		StatusFormAssembly::StatusMove()->Invoke(StatusFormAssembly::StatusForm(), param);
}

void _SetStatusChiEng(bool inChinese)
{	
	//System::Diagnostics::Debug::WriteLine("Candi Move");
	StatusFormAssembly::argCollection->Clear();
	StatusFormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(inChinese)));
	//StatusFormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(y)));	

	Object* param[] =
		dynamic_cast<Object*[]>(StatusFormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	Object* ret =
		StatusFormAssembly::StatusSetChiEng()->Invoke(StatusFormAssembly::StatusForm(), param);
}
void _SetStatusSimpifiedOrTraditional(bool isTraditional)
{		
	StatusFormAssembly::argCollection->Clear();
	StatusFormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(isTraditional)));
	Object* param[] =
		dynamic_cast<Object*[]>(StatusFormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	Object* ret =
		StatusFormAssembly::StatusSetSimpifiedOrTraditional()->Invoke(StatusFormAssembly::StatusForm(), param);
}

void _SetStatusModString(const std::wstring& statusModName)
{
	//System::Diagnostics::Debug::WriteLine("Status SetString");
	StatusFormAssembly::argCollection->Clear();
	StatusFormAssembly::argCollection
		->Add(dynamic_cast<Object*>((__gc new System::String(statusModName.c_str()))));
	Object* param[] =
		dynamic_cast<Object*[]>(StatusFormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	StatusFormAssembly::StatusSetModString()->Invoke(StatusFormAssembly::StatusForm(), param);
}

void _ClearStatusMenuModString()
{
	StatusFormAssembly::StatusClearMenuModString()
		->Invoke(StatusFormAssembly::StatusForm(), NULL);
}

void _SetStatusMenuModString(const std::wstring& statusModName)
{
	StatusFormAssembly::argCollection->Clear();
	StatusFormAssembly::argCollection
		->Add(dynamic_cast<Object*>((__gc new System::String(statusModName.c_str()))));
	Object* param[] =
		dynamic_cast<Object*[]>(StatusFormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	StatusFormAssembly::StatusSetMenuModString()->Invoke(StatusFormAssembly::StatusForm(), param);
}

void _HideStatusPage()
{
	//System::Diagnostics::Debug::WriteLine("Candi Hide");
	//MethodInfo* methodHideWindow = CompFormAssembly::Instance()->GetType("CSharpFormLibrary.IMECompForm")->	GetMethod("HideNoActive");
	//methodHideWindow->Invoke(CompFormAssembly::CompForm(),NULL);
	StatusFormAssembly::StatusHide()->Invoke(StatusFormAssembly::StatusForm(),NULL);
}

int _GetStatusSelectedModuleIndex()
{
	Object* ret = StatusFormAssembly::StatusType()
		->GetMethod("GetSelectedModuleIndex")
		->Invoke(StatusFormAssembly::StatusForm(), NULL);
	return Convert::ToInt32(ret);
}

void _SetUserDir()
{
	AVConfig *cfg= new AVConfig();
	const char *baseDir=cfg->getBaseDir();
	const char *moduleDir=cfg->getModuleDir();
	const char *userDir=cfg->getUserDir();

	StatusFormAssembly::argCollection->Clear();

	StatusFormAssembly::argCollection
		->Add(dynamic_cast<Object*>((__gc new System::String(baseDir))));
	StatusFormAssembly::argCollection
		->Add(dynamic_cast<Object*>((__gc new System::String(moduleDir))));
	StatusFormAssembly::argCollection
		->Add(dynamic_cast<Object*>((__gc new System::String(userDir))));
	Object* param[] =
		dynamic_cast<Object*[]>(StatusFormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	
	StatusFormAssembly::StatusSetDir()
		->Invoke(StatusFormAssembly::StatusForm(), param);
}