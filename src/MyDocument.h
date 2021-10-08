#pragma once
#include <Standard_Transient.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Type.hxx>

class MyDocument
	:public Standard_Transient
{
	DEFINE_STANDARD_RTTIEXT(MyDocument, Standard_Transient)
public:
	MyDocument();
	virtual ~MyDocument();

	void setModified(bool) {}
	bool isModified() { return false; }

	//virtual void SaveData(ByteOStream & out) const;
	//virtual void LoadData(ByteIStream & in);
private:
};
DEFINE_STANDARD_HANDLE(MyDocument, Standard_Transient)



