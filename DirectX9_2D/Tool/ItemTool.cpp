// ItemTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ItemTool.h"
#include "FileInfo.h"


// CItemTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CItemTool, CDialog)

CItemTool::CItemTool(CWnd* pParent /*=NULL*/)
	: CDialog(CItemTool::IDD, pParent)
{

}

CItemTool::~CItemTool()
{
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), DeleteOBJ());
	m_PathInfoList.clear();
}

void CItemTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CItemTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CItemTool::OnListBoxFunc)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CItemTool::OnSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CItemTool::OnLoad)
END_MESSAGE_MAP()


// CItemTool �޽��� ó�����Դϴ�.

void CItemTool::OnListBoxFunc()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CItemTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);
	UpdateData(TRUE);
	int			iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	TCHAR		szFullPath[MAX_PATH] = L"";

	for(int i = 0; i < iFileCount; ++i){
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);
		CFileInfo		FileInfo;

		FileInfo.DirectoryInfoExtraction(szFullPath, m_PathInfoList);

		wstring		wstrCombine		= L"";
		TCHAR		szBuf[MIN_STR]	= L"";
		for(list<IMGPATH*>::iterator iter = m_PathInfoList.begin();	iter != m_PathInfoList.end(); ++iter){
			_itow_s((*iter)->iCount, szBuf, 10);	// iter�� ���� icount ������ 10������ �ٲ㼭 szBuf ����

			 wstrCombine = (*iter)->wstrObjKey + L"|" + (*iter)->wstrStateKey + L"|";
			 wstrCombine += szBuf;
			 wstrCombine += L"|" + (*iter)->wstrPath;

			 m_ListBox.AddString(wstrCombine.c_str());
		}
	}
	UpdateData(FALSE);
}

void CItemTool::OnSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	wofstream			SaveFile;

	SaveFile.open(L"../Data/ImgPath.txt", ios::out);

	int			iCount = 0;

	for(list<IMGPATH*>::iterator iter = m_PathInfoList.begin();
		iter != m_PathInfoList.end(); ++iter)
	{
		++iCount;
		
		SaveFile << (*iter)->wstrObjKey << L"|";
		SaveFile << (*iter)->wstrStateKey << L"|";
		SaveFile << (*iter)->iCount << L"|";

		if(iCount == m_PathInfoList.size())
			SaveFile << (*iter)->wstrPath.c_str() << flush;

		else
			SaveFile << (*iter)->wstrPath.c_str() << endl;

	}

	SaveFile.close();

	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CItemTool::OnLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	wifstream			LoadFile;

	LoadFile.open(L"../Data/ImgPath.txt", ios::in);

	TCHAR		szObjKey[MIN_STR]		= L"";
	TCHAR		szStateKey[MIN_STR]		= L"";
	TCHAR		szCount[MIN_STR]		= L"";
	TCHAR		szImgPath[MAX_PATH]		= L"";

	wstring		wstrCombine = L"";

	m_ListBox.ResetContent();

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);

		wstrCombine = wstring(szObjKey) + L"|" + wstring(szStateKey) + L"|";
		wstrCombine += szCount;
		wstrCombine += L"|" + wstring(szImgPath);

		m_ListBox.AddString(wstrCombine.c_str());
	}

	LoadFile.close();

	UpdateData(FALSE);
	
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}
