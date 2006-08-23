#include <vector>
#include <windows.h>
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
	static Object* objCandidateForm;
	
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
	static Assembly* Instance(){
		if(pasm == NULL)
			pasm = Reflection::Assembly::LoadFile("C:\\WINDOWS\\OpenVanilla\\CSharpFormLibrary.dll");
		return pasm;
	}

	static Object* CandidateForm()
	{
		if(objCandidateForm==NULL)
		{
			objCandidateForm = pasm->CreateInstance("CSharpFormLibrary.IMECandidateForm",true);
		}
		return objCandidateForm;
	}

};

void _ShowCandPageArray(const std::vector<std::wstring>& candidates)
{
	ArrayList* arrayCandidates = __gc new ArrayList();
	for(int i=0;i<candidates.size();i++)
	{
		std::wstring candidate = candidates.at(i);
		arrayCandidates->Add(__gc new System::String(candidate.c_str()));
	}
	ArrayList* argCollection = __gc new ArrayList();
	argCollection->Add((Object*)arrayCandidates);
	MethodInfo* methodShowWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("ShowCandidates");
	methodShowWindow->Invoke(FormAssembly::CandidateForm(),(Object*[])argCollection->ToArray(System::Type::GetType("System.Object")));
}

void _SetCandString(const std::wstring& candidate)
{
	ArrayList* arrayCandidates = __gc new ArrayList();
	//for(int i=0;i<candidates.size();i++)
	//{
	//	std::wstring candidate = candidates.at(i);
		//arrayCandidates->Add(__gc new System::String(candidate.c_str()));
	//}
	ArrayList* argCollection = __gc new ArrayList();
	argCollection->Add((Object*)(__gc new System::String(candidate.c_str())));
	MethodInfo* methodShowWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("ShowCandidates");
	System::Diagnostics::Debug::WriteLine("SetString");
	System::Diagnostics::Debug::WriteLine(FormAssembly::Instance());
	Object* tmp[] = (Object*[])argCollection->ToArray(System::Type::GetType("System.Object"));
	methodShowWindow->Invoke(FormAssembly::CandidateForm(),tmp);
}
void _CreateCandPage()
{
	FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm");
	System::Diagnostics::Debug::WriteLine("Create");
	System::Diagnostics::Debug::WriteLine(FormAssembly::Instance());
}

void _EndCandPage()
{
	//°õ¦æDisposeForm
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
	System::Diagnostics::Debug::WriteLine("Hide");
	System::Diagnostics::Debug::WriteLine(FormAssembly::Instance());
	//°õ¦æHideForm
	MethodInfo* methodDisposeWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("Hide");
	methodDisposeWindow->Invoke(FormAssembly::CandidateForm(),NULL);
}
void _ShowCandPage()
{
	System::Diagnostics::Debug::WriteLine("Show");
	System::Diagnostics::Debug::WriteLine(FormAssembly::Instance());
	//°õ¦æShowForm
	
	MethodInfo* methodDisposeWindow = FormAssembly::Instance()->GetType("CSharpFormLibrary.IMECandidateForm")->GetMethod("Show");
	

	methodDisposeWindow->Invoke(FormAssembly::CandidateForm(),NULL);
}
