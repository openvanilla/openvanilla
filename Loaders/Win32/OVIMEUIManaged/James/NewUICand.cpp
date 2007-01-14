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

ref class CandFormAssembly{
private:
	CandFormAssembly(){}
	static Assembly^ pasm;
	static bool hasPasm = false;

	static Object^ objCandidateForm;
	static bool hasObjCandidateForm = false;

	static Type^ typeCandi;
	static bool hasTypeCandi = false;

	static MethodInfo^ methodCandiShowNoActive;
	static bool hasMethodCandiShowNoActive = false;

	static MethodInfo^ methodCandiMove;
	static bool hasMethodCandiMove = false;

	static MethodInfo^ methodCandiSetString;
	static bool hasMethodCandiSetString = false;

	static MethodInfo^ methodCandiHide;
	static bool hasMethodCandiHide = false;

	static MethodInfo^ methodCandiClear;
	static bool hasMethodCandiClear = false;

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

	static Object^ CandidateForm()
	{
		if(!hasObjCandidateForm)
		{
			objCandidateForm =
				pasm->CreateInstance("CSharpFormLibrary.IMECandidateForm",true);
			hasObjCandidateForm = true;
		}
		return objCandidateForm;
	}

	static Type^ CandiType()
	{
		if(!hasTypeCandi)
		{
			typeCandi =
				CandFormAssembly::Instance()
					->GetType("CSharpFormLibrary.IMECandidateForm");
			hasTypeCandi = true;
		}
		return typeCandi;
	}

	static MethodInfo^ CandiShowNoActive()
	{
		if(!hasMethodCandiShowNoActive)
		{
			methodCandiShowNoActive =
				CandFormAssembly::CandiType()->GetMethod("ShowNoActive");
			hasMethodCandiShowNoActive = true;
		}
		return methodCandiShowNoActive;
	}

	static MethodInfo^ CandiMove()
	{
		if(!hasMethodCandiMove)
		{
			methodCandiMove =
				CandFormAssembly::CandiType()->GetMethod("SetLocation");
			hasMethodCandiMove = true;
		}
		return methodCandiMove;
	}

	static MethodInfo^ CandiSetString()
	{
		if(!hasMethodCandiSetString)
		{
			methodCandiSetString =
				CandFormAssembly::CandiType()->GetMethod("SetCandidates");
			hasMethodCandiSetString = true;
		}
		return methodCandiSetString;
	}

	static MethodInfo^ CandiHide()
	{
		if(!hasMethodCandiHide)
		{
			methodCandiHide =
				CandFormAssembly::CandiType()->GetMethod("HideNoActive");
			hasMethodCandiHide = true;
		}
		return methodCandiHide;
	}

	static MethodInfo^ CandiClear()
	{
		if(!hasMethodCandiClear)
		{
			methodCandiClear =
				CandFormAssembly::CandiType()->GetMethod("ClearCandidates");
			hasMethodCandiClear = true;
		}
		return methodCandiClear;
	}

	static MethodInfo^ CandiSetHWND()
	{
		return 	CandFormAssembly::CandiType()->GetMethod("SetHWND");
	}	
	
	static MethodInfo^ ExpandCandi()
	{
		return 	CandFormAssembly::CandiType()->GetMethod("SwitchCandiMode");
	}	
	
};

HWND _CreateCandPage()//create
{	
	//System::Diagnostics::Debug::WriteLine("Candi Create");
	//MethodInfo* methodCreateWindow =
	//	CandFormAssembly::CandiType()->GetMethod("IMECandidateForm");
	PropertyInfo^ propertyHandle =
		CandFormAssembly::CandiType()->GetProperty("Handle");

	return
		(HWND)(
			safe_cast<IntPtr^>(
				propertyHandle->GetValue(CandFormAssembly::CandidateForm(), nullptr))
		)->ToPointer();

}

HWND _CreateCandPageWithHandle(HWND hwnd)//create
{	
	//System::Diagnostics::Debug::WriteLine("Candi Create");
	//MethodInfo* methodCreateWindow =
	//	CandFormAssembly::CandiType()->GetMethod("IMECandidateForm");
	PropertyInfo^ propertyHandle =
		CandFormAssembly::CandiType()->GetProperty("Handle");

	CandFormAssembly::argCollection->Clear();
	try{
		UInt64 handle = (UInt64)hwnd;
		//int h=(int)handle;
		murmur("%d",handle);
		CandFormAssembly::argCollection->Add(handle);
		array<Object^>^ param =
			CandFormAssembly::argCollection
				->ToArray();
		CandFormAssembly::CandiSetHWND()->Invoke(CandFormAssembly::CandidateForm(), param);
	} catch(System::Exception^ e) {
		Debug::WriteLine(e->StackTrace);
	}

	return
		(HWND)(
			safe_cast<IntPtr^>(
				propertyHandle->GetValue(CandFormAssembly::CandidateForm(), nullptr))
		)->ToPointer();

}

void _SetCandString(const std::wstring& candidate)
{
	//System::Diagnostics::Debug::WriteLine("Candi SetString");
	CandFormAssembly::argCollection->Clear();
	CandFormAssembly::argCollection
		->Add(gcnew System::String(candidate.c_str()));
	array<Object^>^ param =
		CandFormAssembly::argCollection
			->ToArray();
	CandFormAssembly::CandiSetString()->Invoke(CandFormAssembly::CandidateForm(), param);
}

void _ShowCandPage()
{	
	//System::Diagnostics::Debug::WriteLine("Candi Show");
	//try 
	//{
		//System::Reflection::Assembly* foo = CandFormAssembly::Instance();	
		//System::Type* bar = foo->GetType("CSharpFormLibrary.IMECandidateForm");
		//MethodInfo* methodShowWindow = bar->GetMethod("ShowNoActive");
		//methodShowWindow->Invoke(CandFormAssembly::CandidateForm(), NULL);
		//Object^ ret = CandFormAssembly::CandiShowNoActive()
	Object^ ret = CandFormAssembly::CandiShowNoActive()
			->Invoke(CandFormAssembly::CandidateForm(), nullptr);
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

void _MoveCandPage(int x,int y)
{	
	//System::Diagnostics::Debug::WriteLine("Candi Move");
	CandFormAssembly::argCollection->Clear();
	CandFormAssembly::argCollection->Add(x);
	CandFormAssembly::argCollection->Add(y);	

	array<Object^>^ param =
		CandFormAssembly::argCollection
			->ToArray();
	Object^ ret =
		CandFormAssembly::CandiMove()->Invoke(CandFormAssembly::CandidateForm(), param);
}

void _HideCandPage()
{
	//System::Diagnostics::Debug::WriteLine("Candi Hide");
	//MethodInfo* methodHideWindow = CandFormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->	GetMethod("HideNoActive");
	//methodHideWindow->Invoke(CandFormAssembly::CandidateForm(),NULL);
	CandFormAssembly::CandiHide()->Invoke(CandFormAssembly::CandidateForm(), nullptr);
}

void _ClearCandPage()
{
	//System::Diagnostics::Debug::WriteLine("Candi Clear");
	//MethodInfo* methodClearWindow = CandFormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("ClearCandidates");
	//methodClearWindow->Invoke(CandFormAssembly::CandidateForm(),NULL);
	CandFormAssembly::CandiClear()->Invoke(CandFormAssembly::CandidateForm(), nullptr);
}

void _EndCandPage()
{
	//°õ¦æDisposeForm
	//System::Diagnostics::Debug::WriteLine("Candi End");
	MethodInfo^ methodDisposeWindow = CandFormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("DisposeForm");
	methodDisposeWindow->Invoke(CandFormAssembly::CandidateForm(), nullptr);
}

void _ExpandCandi() //®i¶} cand window
{
	Object^ ret = CandFormAssembly::ExpandCandi()
			->Invoke(CandFormAssembly::CandidateForm(), nullptr);
}
