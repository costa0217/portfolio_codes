#include "StdAfx.h"
#include "FileInfo.h"

CFileInfo::CFileInfo(void)
{
}

CFileInfo::~CFileInfo(void)
{
}

std::wstring CFileInfo::ConvertRelativePath(wstring szFullPath)
{
	TCHAR		szRelativePath[MAX_PATH]	= L"";
	TCHAR		szDirectoryPath[MAX_PATH]	= L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
	// vcproj 파일이 있는 위치까지의 경로를 얻어오는 함수
	
	PathRelativePathTo((LPWSTR)szRelativePath, 
		szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY,
		szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL); // 한 경로에서 다른 경로로 가는 상대 경로를 추출																			
													// 같은 드라이브 내에서만 가능

	return szRelativePath;
}

void CFileInfo::DirectoryInfoExtraction( const wstring& wstrPath, 
										list<IMGPATH*>& rPathList )
{
	CFileFind		Find;

	Find.FindFile(wstrPath.c_str());

	int			iContinue = 1;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();		// 파일의 유무를 검사하는 함수

		if(Find.IsDots())		// 파일 폴더 경로 중 . or ..을 걸러내기 위한 함수
			continue;

		if(Find.IsDirectory())			// 찾은 파일이 폴더인지 파일인지 구분해주는 함수
		{
			DirectoryInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}

		else
		{
			if(Find.IsSystem())	// 폴더 안에 숨겨진 시스템 파일을 검색 대상에서 제외시키는 함수
				continue;

			IMGPATH*		pImgPath			= new IMGPATH;
			
			TCHAR			szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, Find.GetFilePath().operator LPCWSTR());	// 추출한 경로를 문자열로 복사
			
			PathRemoveFileSpec(szPathBuf);	// 저장된 경로를 잘라냄
			pImgPath->iCount = DirectoryFileCount(szPathBuf); 

			wstring	wstrTextureName = Find.GetFileTitle().operator LPCWSTR();	// 경로를 제외한 파일의 이름만을 얻어내는 함수
					wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
					// cstring delete 함수와 같은 기능, 0번부터 전체 사이즈 -1만큼을 잘라내겠다는 의미, 

			TCHAR		szBuf[MAX_PATH]	= L"";
			lstrcpy(szBuf, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szBuf);		

			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());	// 텍스쳐의 이름을 얻어온 뒤, 추출한 경로와 결합하는 함수
			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			
			PathRemoveFileSpec(szBuf);
			pImgPath->wstrStateKey = PathFindFileName(szBuf);	// 현 위치에 있는 파일 이름 추출

			PathRemoveFileSpec(szBuf);
			pImgPath->wstrObjKey = PathFindFileName(szBuf);	// 현 위치에 있는 파일 이름 추출
		
			rPathList.push_back(pImgPath);
			
			Find.FindFile(Find.GetFilePath());	// 다시 또 탐색, 현재 파일에서 한 단계 위를 찾아줌
			Find.FindNextFile();	// 다음 폴더로 이동시킨다.

		}
	}


}

int CFileInfo::DirectoryFileCount( const wstring& wstrPath )
{
		wstring		wstrTemp = wstrPath + L"\\*.png";

		CFileFind		Find;
		Find.FindFile(wstrTemp.c_str());

		int			iContinue	= 1;
		int			iFileCount	= 0;

		while(iContinue)
		{
			iContinue = Find.FindNextFile();

			if(!Find.IsDirectory())
				++iFileCount;
		}

		return iFileCount;
}
