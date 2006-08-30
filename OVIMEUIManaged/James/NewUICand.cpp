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

};

void _ShowCandPageArray(const std::vector<std::wstring>& candidates)
{
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
}

void _SetCandString(const std::wstring& candidate)
{
	System::Diagnostics::Debug::WriteLine("Candi SetString");
	ArrayList* arrayCandidates = __gc new ArrayList();
	ArrayList* argCollection = __gc new ArrayList();
	argCollection->Add(dynamic_cast<Object*>((__gc new System::String(candidate.c_str()))));
	MethodInfo* methodShowWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("SetCandidates");	
	Object* tmp[] = (Object*[])argCollection->ToArray(System::Type::GetType("System.Object"));
	methodShowWindow->Invoke(FormAssembly::CandidateForm(),tmp);
}
void _CreateCandPage()//create
{	
	System::Diagnostics::Debug::WriteLine("Candi Create");
	MethodInfo* methodCreateWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("IMECandidateForm");
	
	//methodCreateWindow->Invoke(FormAssembly::CandidateForm(),NULL);

	
}
void _CreateCandAndSetLocation(int x ,int y) //create and init as (x,y)
{	/*
	MethodInfo* methodCreateWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("IMECandidateForm");
	ArrayList* argCollection = __gc new ArrayList();
	System::Diagnostics::Debug::WriteLine("Create and Set Location");
	argCollection->Add(dynamic_cast<Object*>(__box(x)));
	argCollection->Add(dynamic_cast<Object*>(__box(y)));
	Object* tmp[] = (Object*[])argCollection->ToArray(System::Type::GetType("System.Object"));
	methodShowWindow->Invoke(FormAssembly::CandidateForm(),tmp);	*/
}
void _EndCandPage()
{
	//°õ¦æDisposeForm
	System::Diagnostics::Debug::WriteLine("Candi End");
	MethodInfo* methodDisposeWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("DisposeForm");
	methodDisposeWindow->Invoke(FormAssembly::CandidateForm(),NULL);
}

int _GetCandidateValue()
{
	MethodInfo* methodGetValue = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("GetValue");
	Object* rtnValue = methodGetValue->Invoke(FormAssembly::CandidateForm(),NULL);
	return Convert::ToInt32(rtnValue);
}

void _HideCandPage()
{
	System::Diagnostics::Debug::WriteLine("Candi Hide");
	MethodInfo* methodHideWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->	GetMethod("HideNoActive");
	methodHideWindow->Invoke(FormAssembly::CandidateForm(),NULL);
}
void _ClearCandPage()
{
	System::Diagnostics::Debug::WriteLine("Candi Clear");
	MethodInfo* methodClearWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("ClearCandidates");
	methodClearWindow->Invoke(FormAssembly::CandidateForm(),NULL);
}
void _ShowCandPage()
{	
	System::Diagnostics::Debug::WriteLine("Candi Show");
	try 
	{
		System::Reflection::Assembly* foo = FormAssembly::Instance();	
		System::Type* bar = foo->GetType("CSharpFormLibrary.IMECandidateForm");
		MethodInfo* methodShowWindow = bar->GetMethod("ShowNoActive");
		methodShowWindow->Invoke(FormAssembly::CandidateForm(), NULL);
	}
	catch(System::Exception* e)
	{
		System::Diagnostics::Debug::WriteLine(e->Source);
		System::Diagnostics::Debug::WriteLine(e->TargetSite);
		System::Diagnostics::Debug::WriteLine(e->HelpLink);
		System::Diagnostics::Debug::WriteLine(e->InnerException);
		System::Diagnostics::Debug::WriteLine(e->Message);
		System::Diagnostics::Debug::WriteLine(e->StackTrace);
	}
}

void _MoveCandPage(int x,int y)
{	
	System::Diagnostics::Debug::WriteLine("Candi Move");
	ArrayList* argCollection = __gc new ArrayList();
	argCollection->Add(dynamic_cast<Object*>(__box(x)));
	argCollection->Add(dynamic_cast<Object*>(__box(y)));	

	MethodInfo* methodMoveWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->	GetMethod("SetLocation");
	Object* tmp[] = (Object*[])argCollection->ToArray(System::Type::GetType("System.Object"));
	methodMoveWindow->Invoke(FormAssembly::CandidateForm(),tmp);
}

