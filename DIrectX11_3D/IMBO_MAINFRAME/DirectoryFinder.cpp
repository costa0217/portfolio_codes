#include "stdafx.h"
#include "DirectoryFinder.h"

bool CDirectoryFinder::Begin() {
	
	return true;
}

bool CDirectoryFinder::End() {

	return true;
}

void CDirectoryFinder::GetFiles(vector<tstring>& vList, tstring sPath, bool bSearchAllDirectories, bool bAllDirectories, WCHAR* sExtension,int length) {
	tstring sTmp = sPath + tstring(TEXT("\\*.*"));


	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(sTmp.c_str(), &fd);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bSearchAllDirectories)
				{
					if (fd.cFileName[0] != '.')
					{
						sTmp = sPath
							+ STR_TXT("\\")
							+ tstring(fd.cFileName);
						GetFiles(vList, sTmp, bSearchAllDirectories, bAllDirectories, sExtension,(length < 0 ? (sPath + STR_TXT("\\")).length() : length));
					}
				}
			}
			else
			{
				//�̺κп��� ���ϴ� Ȯ���ڸ� ���� �� ����
				//PathFindExtension - Ȯ������ ��ġ�� ã�Ƽ� ��ȯ�Ѵ�.
				//	"c:\abc\def.txt" -> ".txt"
				sTmp = sPath
					+ STR_TXT("\\")
					+ tstring(fd.cFileName);

				if (nullptr != sExtension) {//Ȯ���� ������ nullptr�� �ƴ϶��
					wstring fileName{ PathFindExtension(fd.cFileName) };
					if (fileName != sExtension){
						//�ش� Ȯ���ڰ� �ƴϸ�
						continue;
					}
				}
				
				if (length < 0) length = sPath.length();
				wstring s = (bAllDirectories ? sTmp : sTmp.substr(length, sTmp.length() - length));
				//s = ReplaceString(s, L"\\", L"/");
				vList.push_back(s);
			}

		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}

}

std::wstring CDirectoryFinder::ReplaceString(std::wstring subject, const std::wstring &search, const std::wstring &replace){
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::wstring::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}
