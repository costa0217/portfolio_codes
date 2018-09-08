#pragma once
#include <fstream>

//File Exporter�� ����ϴ� ��ü
class CFileImporter {
public:
	virtual string Readstring(ifstream& in) = 0;
	virtual UINT ReadUINT(ifstream& in) = 0;
	virtual int ReadInt(ifstream& in) = 0;
	virtual float ReadFloat(ifstream& in) = 0;
	virtual bool ReadBool(ifstream& in) = 0;

	//txt file ����� ��� �� ������ ���
	virtual void ReadSpace(ifstream& in) = 0;
	virtual void ReadEnter(ifstream& in) = 0;
private:

public:
	CFileImporter() {};
	~CFileImporter() {};
};

