#include <vector>
//#include <windows.h>
#include "PCMan.h"


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

__gc class FormAssembly{
private:
	FormAssembly(){}
	static Assembly* pasm;
	static bool hasPasm = false;

	static Object* objCandidateForm;
	static bool hasObjCandidateForm = false;

	static Type* typeCandi;
	static bool hasTypeCandi = false;

	static MethodInfo* methodCandiShowNoActive;
	static bool hasMethodCandiShowNoActive = false;

	static MethodInfo* methodCandiMove;
	static bool hasMethodCandiMove = false;

	static MethodInfo* methodCandiSetString;
	static bool hasMethodCandiSetString = false;

	static MethodInfo* methodCandiHide;
	static bool hasMethodCandiHide = false;

	static MethodInfo* methodCandiClear;
	static bool hasMethodCandiClear = false;

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

	static Object* CandidateForm()
	{
		if(!hasObjCandidateForm)
		{
			objCandidateForm =
				pasm->CreateInstance("CSharpFormLibrary.IMECandidateForm",true);
			hasObjCandidateForm = true;
		}
		return objCandidateForm;
	}

	static Type* CandiType()
	{
		if(!hasTypeCandi)
		{
			typeCandi =
				FormAssembly::Instance()
					->GetType("CSharpFormLibrary.IMECandidateForm");
			hasTypeCandi = true;
		}
		return typeCandi;
	}

	static MethodInfo* CandiShowNoActive()
	{
		if(!hasMethodCandiShowNoActive)
		{
			methodCandiShowNoActive =
				FormAssembly::CandiType()->GetMethod("ShowNoActive");
			hasMethodCandiShowNoActive = true;
		}
		return methodCandiShowNoActive;
	}

	static MethodInfo* CandiMove()
	{
		if(!hasMethodCandiMove)
		{
			methodCandiMove =
				FormAssembly::CandiType()	->GetMethod("SetLocation");
			hasMethodCandiMove = true;
		}
		return methodCandiMove;
	}

	static MethodInfo* CandiSetString()
	{
		if(!hasMethodCandiSetString)
		{
			methodCandiSetString =
				FormAssembly::CandiType()	->GetMethod("SetCandidates");
			hasMethodCandiSetString = true;
		}
		return methodCandiSetString;
	}

	static MethodInfo* CandiHide()
	{
		if(!hasMethodCandiHide)
		{
			methodCandiHide =
				FormAssembly::CandiType()	->GetMethod("HideNoActive");
			hasMethodCandiHide = true;
		}
		return methodCandiHide;
	}

	static MethodInfo* CandiClear()
	{
		if(!hasMethodCandiClear)
		{
			methodCandiClear =
				FormAssembly::CandiType()	->GetMethod("ClearCandidates");
			hasMethodCandiClear = true;
		}
		return methodCandiClear;
	}
};

HWND _CreateCandPage()//create
{	
	//System::Diagnostics::Debug::WriteLine("Candi Create");
	//MethodInfo* methodCreateWindow =
	//	FormAssembly::CandiType()->GetMethod("IMECandidateForm");
	PropertyInfo* propertyHandle =
		FormAssembly::CandiType()->GetProperty("Handle");
	return
		(HWND)(
			dynamic_cast<IntPtr*>(
				propertyHandle->GetValue(FormAssembly::CandidateForm(), NULL))
		)->ToPointer();
}

void _SetCandString(const std::wstring& candidate)
{
	//System::Diagnostics::Debug::WriteLine("Candi SetString");
	FormAssembly::argCollection->Clear();
	FormAssembly::argCollection
		->Add(dynamic_cast<Object*>((__gc new System::String(candidate.c_str()))));
	Object* param[] =
		dynamic_cast<Object*[]>(FormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	FormAssembly::CandiSetString()->Invoke(FormAssembly::CandidateForm(), param);
}

void _ShowCandPage()
{	
	//System::Diagnostics::Debug::WriteLine("Candi Show");
	//try 
	//{
		//System::Reflection::Assembly* foo = FormAssembly::Instance();	
		//System::Type* bar = foo->GetType("CSharpFormLibrary.IMECandidateForm");
		//MethodInfo* methodShowWindow = bar->GetMethod("ShowNoActive");
		//methodShowWindow->Invoke(FormAssembly::CandidateForm(), NULL);
		//Object* ret = FormAssembly::CandiShowNoActive()
	Object* ret = FormAssembly::CandiShowNoActive()
			->Invoke(FormAssembly::CandidateForm(), NULL);
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
	FormAssembly::argCollection->Clear();
	FormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(x)));
	FormAssembly::argCollection->Add(dynamic_cast<Object*>(__box(y)));	

	Object* param[] =
		dynamic_cast<Object*[]>(FormAssembly::argCollection
			->ToArray(System::Type::GetType("System.Object")));
	Object* ret =
		FormAssembly::CandiMove()->Invoke(FormAssembly::CandidateForm(), param);
}

void _HideCandPage()
{
	//System::Diagnostics::Debug::WriteLine("Candi Hide");
	//MethodInfo* methodHideWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->	GetMethod("HideNoActive");
	//methodHideWindow->Invoke(FormAssembly::CandidateForm(),NULL);
	FormAssembly::CandiHide()->Invoke(FormAssembly::CandidateForm(),NULL);
}

void _ClearCandPage()
{
	//System::Diagnostics::Debug::WriteLine("Candi Clear");
	//MethodInfo* methodClearWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("ClearCandidates");
	//methodClearWindow->Invoke(FormAssembly::CandidateForm(),NULL);
	FormAssembly::CandiClear()->Invoke(FormAssembly::CandidateForm(),NULL);
}

void _EndCandPage()
{
	//°õ¦æDisposeForm
	//System::Diagnostics::Debug::WriteLine("Candi End");
	MethodInfo* methodDisposeWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("DisposeForm");
	methodDisposeWindow->Invoke(FormAssembly::CandidateForm(),NULL);
}

int _GetCandidateValue()
{
	/*
	MethodInfo* methodGetValue = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("GetValue");
	Object* rtnValue = methodGetValue->Invoke(FormAssembly::CandidateForm(),NULL);
	return Convert::ToInt32(rtnValue);
	*/
	return 0;
}

void _ShowCandPageArray(const std::vector<std::wstring>& candidates)
{
	/*
	ArrayList* arrayCandidates = __gc new ArrayList();
	for(size_t i=0;i<candidates.size();i++)
	{
		std::wstring candidate = candidates.at(i);
		arrayCandidates->Add(__gc new System::String(candidate.c_str()));
	}
	ArrayList* argCollection = __gc new ArrayList();
	argCollection->Add(dynamic_cast<Object*>(arrayCandidates));
	MethodInfo* methodShowWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("ShowCandidates");
	methodShowWindow->Invoke(FormAssembly::CandidateForm(),(Object*[])argCollection->ToArray(System::Type::GetType("System.Object")));
	*/
}

/*
void _CreateCandAndSetLocation(int x ,int y) //create and init as (x,y)
{
	MethodInfo* methodCreateWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("IMECandidateForm");
	ArrayList* argCollection = __gc new ArrayList();
	System::Diagnostics::Debug::WriteLine("Create and Set Location");
	argCollection->Add(dynamic_cast<Object*>(__box(x)));
	argCollection->Add(dynamic_cast<Object*>(__box(y)));
	Object* tmp[] = (Object*[])argCollection->ToArray(System::Type::GetType("System.Object"));
	methodShowWindow->Invoke(FormAssembly::CandidateForm(),tmp);
}
*/
