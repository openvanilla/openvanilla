//#define OV_DEBUG  
#include <vector>
//#include <windows.h>
#include "PCMan.h"
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

ref class NotifyFormAssembly{
private:
	NotifyFormAssembly()
	{
		pasm = NotifyFormAssembly::Instance();
	}

	static Assembly^ pasm;
	static bool hasPasm = false;

	static Object^ objNotifyForm;
	static bool hasObjNotifyForm = false;

	static Type^ typeNotify;
	static bool hasTypeNotify = false;

	static MethodInfo^ methodNotifyShowNoActive;
	static bool hasMethodNotifyShowNoActive = false;

	static MethodInfo^ methodNotifyMove;
	static bool hasMethodNotifyMove = false;

	static MethodInfo^ methodNotifySetString;
	static bool hasMethodNotifySetString = false;

	static MethodInfo^ methodNotifyHide;
	static bool hasMethodNotifyHide = false;

	static MethodInfo^ methodNotifyClear;
	static bool hasMethodNotifyClear = false;

	/*
	ref struct IsFormCreated
	{
		property bool IsCreated{
			bool get() { return isCreated; }
			void set(bool flag) { isCreated = flag; }
		}
	};
	*/

public:
	static ArrayList^ argCollection = gcnew ArrayList();
	//static IsFormCreated^ FormStatus = gcnew IsFormCreated();
	static Assembly^ Instance()
	{
		if(!hasPasm) 
		{
			AVConfig *cfg= new AVConfig();
			const char *baseDir=cfg->getBaseDir();
			String^ asmPath = gcnew String(baseDir);
			pasm =
				Reflection::Assembly::LoadFile(
					asmPath->Concat(
						asmPath, gcnew String("CSharpFormLibrary.dll")));
			hasPasm = true;
		}
		return pasm;
	}

	static Object^ NotifyForm()
	{
		if(!hasObjNotifyForm)
		{
			objNotifyForm =
				NotifyFormAssembly::Instance()
					->CreateInstance("CSharpFormLibrary.IMENotifyForm",true);
			hasObjNotifyForm = true;
		}
		return objNotifyForm;
	}

	static Type^ NotifyType()
	{
		if(!hasTypeNotify)
		{
			typeNotify =
				NotifyFormAssembly::Instance()
					->GetType("CSharpFormLibrary.IMENotifyForm");
			hasTypeNotify = true;
		}
		return typeNotify;
	}

	static MethodInfo^ NotifyShowNoActive()
	{
		if(!hasMethodNotifyShowNoActive)
		{
			methodNotifyShowNoActive =
				NotifyFormAssembly::NotifyType()->GetMethod("ShowNoActive");
			hasMethodNotifyShowNoActive = true;
		}
		return methodNotifyShowNoActive;
	}

	static MethodInfo^ NotifyMove()
	{
		if(!hasMethodNotifyMove)
		{
			methodNotifyMove =
				NotifyFormAssembly::NotifyType()->GetMethod("SetLocation");
			hasMethodNotifyMove = true;
		}
		return methodNotifyMove;
	}

	static MethodInfo^ NotifySetString()
	{
		if(!hasMethodNotifySetString)
		{
			methodNotifySetString =
				NotifyFormAssembly::NotifyType()->GetMethod("SetNotifyStr");
			hasMethodNotifySetString = true;
		}
		return methodNotifySetString;
	}

	static MethodInfo^ NotifyHide()
	{
		if(!hasMethodNotifyHide)
		{
			methodNotifyHide =
				NotifyFormAssembly::NotifyType()->GetMethod("HideNoActive");
			hasMethodNotifyHide = true;
		}
		return methodNotifyHide;
	}

	static MethodInfo^ NotifyClear()
	{
		if(!hasMethodNotifyClear)
		{
			methodNotifyClear =
				NotifyFormAssembly::NotifyType()->GetMethod("ClearNotifys");
			hasMethodNotifyClear = true;
		}
		return methodNotifyClear;
	}

	static MethodInfo^ NotifySetHWND()
	{
		return 	NotifyFormAssembly::NotifyType()->GetMethod("SetHWND");
	}	
	
	static MethodInfo^ ExpandNotify()
	{
		return 	NotifyFormAssembly::NotifyType()->GetMethod("SwitchNotifyMode");
	}	
	
};

HWND _CreateNotifyPage()//create
{	
	//System::Diagnostics::Debug::WriteLine("Notify Create");
	//MethodInfo* methodCreateWindow =
	//	NotifyFormAssembly::NotifyType()->GetMethod("IMENotifyForm");
	PropertyInfo^ propertyHandle =
		NotifyFormAssembly::NotifyType()->GetProperty("Handle");

	return
		(HWND)(
			safe_cast<IntPtr^>(
				propertyHandle->GetValue(NotifyFormAssembly::NotifyForm(), nullptr))
		)->ToPointer();

}

HWND _CreateNotifyPageWithHandle(HWND hwnd)//create
{	
	//System::Diagnostics::Debug::WriteLine("Notify Create");
	//MethodInfo* methodCreateWindow =
	//	NotifyFormAssembly::NotifyType()->GetMethod("IMENotifyForm");
	PropertyInfo^ propertyHandle =
		NotifyFormAssembly::NotifyType()->GetProperty("Handle");

	NotifyFormAssembly::argCollection->Clear();
	try{
		UInt64 handle = (UInt64)hwnd;
		//int h=(int)handle;
		murmur("%d",handle);
		NotifyFormAssembly::argCollection->Add(handle);
		array<Object^>^ param =
			NotifyFormAssembly::argCollection
				->ToArray();
		NotifyFormAssembly::NotifySetHWND()->Invoke(NotifyFormAssembly::NotifyForm(), param);
	} catch(System::Exception^ e) {
		Debug::WriteLine(e->StackTrace);
	}

	return
		(HWND)(
			safe_cast<IntPtr^>(
				propertyHandle->GetValue(NotifyFormAssembly::NotifyForm(), nullptr))
		)->ToPointer();

}

void _SetNotifyString(const std::wstring& Notify)
{
	//System::Diagnostics::Debug::WriteLine("Notify SetString");
	NotifyFormAssembly::argCollection->Clear();
	NotifyFormAssembly::argCollection
		->Add(gcnew System::String(Notify.c_str()));
	array<Object^>^ param =
		NotifyFormAssembly::argCollection
			->ToArray();
	NotifyFormAssembly::NotifySetString()->Invoke(NotifyFormAssembly::NotifyForm(), param);
}

void _ShowNotifyPage()
{	
	//System::Diagnostics::Debug::WriteLine("Notify Show");
	//try 
	//{
		//System::Reflection::Assembly* foo = NotifyFormAssembly::Instance();	
		//System::Type* bar = foo->GetType("CSharpFormLibrary.IMENotifyForm");
		//MethodInfo* methodShowWindow = bar->GetMethod("ShowNoActive");
		//methodShowWindow->Invoke(NotifyFormAssembly::NotifyForm(), NULL);
		//Object^ ret = NotifyFormAssembly::NotifyShowNoActive()
	Object^ ret = NotifyFormAssembly::NotifyShowNoActive()
			->Invoke(NotifyFormAssembly::NotifyForm(), nullptr);
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

void _MoveNotifyPage(int x,int y)
{	
	//System::Diagnostics::Debug::WriteLine("Notify Move");
	NotifyFormAssembly::argCollection->Clear();
	NotifyFormAssembly::argCollection->Add(x);
	NotifyFormAssembly::argCollection->Add(y);	

	array<Object^>^ param =
		NotifyFormAssembly::argCollection
			->ToArray();
	Object^ ret =
		NotifyFormAssembly::NotifyMove()->Invoke(NotifyFormAssembly::NotifyForm(), param);
}

void _HideNotifyPage()
{
	//System::Diagnostics::Debug::WriteLine("Notify Hide");
	//MethodInfo* methodHideWindow = NotifyFormAssembly::Instance()->GetType("CSharpFormLibrary.IMENotifyForm")->	GetMethod("HideNoActive");
	//methodHideWindow->Invoke(NotifyFormAssembly::NotifyForm(),NULL);
	NotifyFormAssembly::NotifyHide()->Invoke(NotifyFormAssembly::NotifyForm(), nullptr);
}

void _ClearNotifyPage()
{
	//System::Diagnostics::Debug::WriteLine("Notify Clear");
	//MethodInfo* methodClearWindow = NotifyFormAssembly::Instance()->GetType("CSharpFormLibrary.IMENotifyForm")->GetMethod("ClearNotifys");
	//methodClearWindow->Invoke(NotifyFormAssembly::NotifyForm(),NULL);
	NotifyFormAssembly::NotifyClear()->Invoke(NotifyFormAssembly::NotifyForm(), nullptr);
}

void _EndNotifyPage()
{
	//°õ¦æDisposeForm
	//System::Diagnostics::Debug::WriteLine("Notify End");
	MethodInfo^ methodDisposeWindow = NotifyFormAssembly::Instance()->GetType("CSharpFormLibrary.IMENotifyForm")->GetMethod("DisposeForm");
	methodDisposeWindow->Invoke(NotifyFormAssembly::NotifyForm(), nullptr);
}

void _ExpandNotify() //®i¶} Notify window
{
	Object^ ret = NotifyFormAssembly::ExpandNotify()
			->Invoke(NotifyFormAssembly::NotifyForm(), nullptr);
}
