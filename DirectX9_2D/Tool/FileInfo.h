#pragma once

#include "Include.h"

class CFileInfo
{
public:
	wstring		ConvertRelativePath(wstring szFullPath);	// ����η� �ٲٴ� �Լ�

	void DirectoryInfoExtraction(const wstring& wstrPath, 
		list<IMGPATH*>& rPathList);		
	// ���� ������ �����ϴ� �뵵�� �Լ�
	
	int DirectoryFileCount(const wstring& wstrPath);	 
	// ���丮 �ȿ� ���� ������ ����ϴ� �Լ�

public:
	CFileInfo(void);
	~CFileInfo(void);
};
