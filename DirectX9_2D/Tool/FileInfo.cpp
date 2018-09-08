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
	// vcproj ������ �ִ� ��ġ������ ��θ� ������ �Լ�
	
	PathRelativePathTo((LPWSTR)szRelativePath, 
		szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY,
		szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL); // �� ��ο��� �ٸ� ��η� ���� ��� ��θ� ����																			
													// ���� ����̺� �������� ����

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
		iContinue = Find.FindNextFile();		// ������ ������ �˻��ϴ� �Լ�

		if(Find.IsDots())		// ���� ���� ��� �� . or ..�� �ɷ����� ���� �Լ�
			continue;

		if(Find.IsDirectory())			// ã�� ������ �������� �������� �������ִ� �Լ�
		{
			DirectoryInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}

		else
		{
			if(Find.IsSystem())	// ���� �ȿ� ������ �ý��� ������ �˻� ��󿡼� ���ܽ�Ű�� �Լ�
				continue;

			IMGPATH*		pImgPath			= new IMGPATH;
			
			TCHAR			szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, Find.GetFilePath().operator LPCWSTR());	// ������ ��θ� ���ڿ��� ����
			
			PathRemoveFileSpec(szPathBuf);	// ����� ��θ� �߶�
			pImgPath->iCount = DirectoryFileCount(szPathBuf); 

			wstring	wstrTextureName = Find.GetFileTitle().operator LPCWSTR();	// ��θ� ������ ������ �̸����� ���� �Լ�
					wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
					// cstring delete �Լ��� ���� ���, 0������ ��ü ������ -1��ŭ�� �߶󳻰ڴٴ� �ǹ�, 

			TCHAR		szBuf[MAX_PATH]	= L"";
			lstrcpy(szBuf, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szBuf);		

			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());	// �ؽ����� �̸��� ���� ��, ������ ��ο� �����ϴ� �Լ�
			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			
			PathRemoveFileSpec(szBuf);
			pImgPath->wstrStateKey = PathFindFileName(szBuf);	// �� ��ġ�� �ִ� ���� �̸� ����

			PathRemoveFileSpec(szBuf);
			pImgPath->wstrObjKey = PathFindFileName(szBuf);	// �� ��ġ�� �ִ� ���� �̸� ����
		
			rPathList.push_back(pImgPath);
			
			Find.FindFile(Find.GetFilePath());	// �ٽ� �� Ž��, ���� ���Ͽ��� �� �ܰ� ���� ã����
			Find.FindNextFile();	// ���� ������ �̵���Ų��.

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
