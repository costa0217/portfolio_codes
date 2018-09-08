#pragma once
#include "FileImporter.h"

//File Exporter�� ����ϴ� ��ü
class CTxtImporter :public CFileImporter {
public:
	virtual string Readstring(ifstream& in);
	virtual UINT ReadUINT(ifstream& in);
	virtual int ReadInt(ifstream& in);
	virtual float ReadFloat(ifstream& in);
	virtual bool ReadBool(ifstream& in);

	//txt file ����� ��� �� ������ ���
	virtual void ReadSpace(ifstream& in);
	virtual void ReadEnter(ifstream& in);
private:

public:
	CTxtImporter();
	~CTxtImporter();
};
