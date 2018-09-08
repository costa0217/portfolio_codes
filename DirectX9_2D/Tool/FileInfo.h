#pragma once

#include "Include.h"

class CFileInfo
{
public:
	wstring		ConvertRelativePath(wstring szFullPath);	// 상대경로로 바꾸는 함수

	void DirectoryInfoExtraction(const wstring& wstrPath, 
		list<IMGPATH*>& rPathList);		
	// 폴더 정보를 추출하는 용도의 함수
	
	int DirectoryFileCount(const wstring& wstrPath);	 
	// 디렉토리 안에 파일 개수를 계산하는 함수

public:
	CFileInfo(void);
	~CFileInfo(void);
};
