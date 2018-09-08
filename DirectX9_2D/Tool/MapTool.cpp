// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "ToolView.h"

#include "BackGround.h"


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
	, m_bStart(true)
	, m_iDrawID(0)
	, m_iRouteNum(0)
{
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_StageCombo);

	DDX_Control(pDX, IDC_RADIO1, m_TileRadio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_TileRadio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_TileRadio[2]);
	DDX_Control(pDX, IDC_RADIO4, m_TileRadio[3]);

	if(m_bStart)
	{
		m_TileRadio[0].SetCheck(TRUE);
		m_bStart = false;
	}
	DDX_Text(pDX, IDC_EDIT4, m_iRouteNum);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMapTool::OnClickCombo)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CMapTool::OnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CMapTool::OnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnDeleteTile)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnSaveTile)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnLoadTile)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveRoute)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnLoadRoute)
	ON_EN_CHANGE(IDC_EDIT4, &CMapTool::OnRouteNum)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTool::OnReturnBack)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.
BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_StageCombo.AddString(L"Stage1");
	m_StageCombo.AddString(L"Stage2");
	m_StageCombo.AddString(L"Stage3");
	m_StageCombo.AddString(L"Stage4");

	CImage*		pPngImage = new CImage;
	pPngImage->Load(L"../Texture/Stage/1.png");
	m_MapPngImage.insert(make_pair(L"Stage1", pPngImage));

	pPngImage = new CImage;
	pPngImage->Load(L"../Texture/Stage/2.png");
	m_MapPngImage.insert(make_pair(L"Stage2", pPngImage));

	pPngImage = new CImage;
	pPngImage->Load(L"../Texture/Stage/3.png");
	m_MapPngImage.insert(make_pair(L"Stage3", pPngImage));

	pPngImage = new CImage;
	pPngImage->Load(L"../Texture/Stage/4.png");
	m_MapPngImage.insert(make_pair(L"Stage4", pPngImage));


	m_pVecTile = &((CBackGround*)((CMainFrame*)AfxGetMainWnd())->m_MainView->m_pBackGround)->m_VecTile;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	for(map<CString, CImage*>::iterator iter = m_MapPngImage.begin();
		iter != m_MapPngImage.end(); ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_MapPngImage.clear();
}

void CMapTool::OnClickCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	UpdateData(TRUE);

	CString		strSelectName;

	int iSelectIndex = m_StageCombo.GetCurSel();

	if(iSelectIndex < 0)
		return;

	m_StageCombo.GetLBText(iSelectIndex, strSelectName);

	map<CString, CImage*>::iterator	iter = m_MapPngImage.find(strSelectName);

	((CMainFrame*)AfxGetMainWnd())->m_MainView->m_Map = true;
	((CMainFrame*)AfxGetMainWnd())->m_MainView->Invalidate(FALSE);
	
	UpdateData(FALSE);

}



void CMapTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);

	for(int i = 0; i < 4; ++i)
	{
		if(m_TileRadio[i].GetCheck())
		{
			m_iDrawID = i;
			break;
		}
	}
}

void CMapTool::OnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for(int i = 0; i < 4; ++i)
	{
		if(m_TileRadio[i].GetCheck())
		{
			m_iDrawID = i;
			break;
		}
	}
}

void CMapTool::OnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for(int i = 0; i < 4; ++i)
	{
		if(m_TileRadio[i].GetCheck())
		{
			m_iDrawID = i;
			break;
		}
	}
}

void CMapTool::OnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for(int i = 0; i < 4; ++i)
	{
		if(m_TileRadio[i].GetCheck())
		{
			m_iDrawID = i;
			break;
		}
	}
}

void CMapTool::OnClickedRadio4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for(int i = 0; i < 4; ++i)
	{
		if(m_TileRadio[i].GetCheck())
		{
			m_iDrawID = i;
			break;
		}
	}
}

void CMapTool::OnDeleteTile()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for(int i = 0; i < TILEX * TILEY; ++i)
	{
		(*m_pVecTile)[i]->byDrawID = 0;
	}
	((CMainFrame*)AfxGetMainWnd())->m_MainView->Invalidate(FALSE);
	UpdateData(FALSE);
}

void CMapTool::OnSaveTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR	szDirectoryPath[MAX_PATH];

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
	SetCurrentDirectory(szDirectoryPath);
	PathRemoveFileSpec(szDirectoryPath);
	lstrcat(szDirectoryPath, L"..\\data");

	Dlg.m_ofn.lpstrInitialDir = szDirectoryPath;

	
	
	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	for(vector<TILE*>::iterator iter = m_pVecTile->begin();
		iter != m_pVecTile->end(); ++iter)
	{
		WriteFile(hFile, (*iter), sizeof(TILE), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CMapTool::OnLoadTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	for(vector<TILE*>::iterator iter = m_pVecTile->begin();
		iter != m_pVecTile->end(); ++iter)
	{
		::Safe_Delete(*iter);
	}
	m_pVecTile->clear();

	TCHAR	szDirectoryPath[MAX_PATH];

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
	SetCurrentDirectory(szDirectoryPath);
	PathRemoveFileSpec(szDirectoryPath);
	lstrcat(szDirectoryPath, L"..\\data");

	Dlg.m_ofn.lpstrInitialDir = szDirectoryPath;

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	while(true)
	{
		TILE*		pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			::Safe_Delete(pTile);
			break;
		}
		m_pVecTile->push_back(pTile);
	}
	CloseHandle(hFile);
}

void CMapTool::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 루트 맵에 넣기
	UpdateData(TRUE);
	OnRouteNum();

	vector<int>		SaveVecRoute;

	for(vector<int>::reverse_iterator riter = m_VecRouteNum.rbegin();
		riter != m_VecRouteNum.rend(); ++riter)
	{
		SaveVecRoute.push_back((*riter));
	}
	m_MapRoute.insert(make_pair(m_iRouteNum, SaveVecRoute));

	m_VecRouteNum.clear();

	for(int i = 0; i < TILEX * TILEY; ++i)
	{
		if((*m_pVecTile)[i]->byDrawID == 3)
			(*m_pVecTile)[i]->byDrawID = 0;
	}
	((CMainFrame*)AfxGetMainWnd())->m_MainView->Invalidate(FALSE);
	UpdateData(FALSE);

}

void CMapTool::OnSaveRoute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR	szDirectoryPath[MAX_PATH];

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
	SetCurrentDirectory(szDirectoryPath);
	PathRemoveFileSpec(szDirectoryPath);
	lstrcat(szDirectoryPath, L"..\\data");

	Dlg.m_ofn.lpstrInitialDir = szDirectoryPath;

	
	
	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	for(map<int, vector<int>>::iterator Fstiter = m_MapRoute.begin();
		Fstiter != m_MapRoute.end(); ++Fstiter)
	{
		int iSize  = Fstiter->second.size();
		WriteFile(hFile, &(Fstiter->first), sizeof(int), &dwByte, NULL);	// 키값
		WriteFile(hFile, &iSize, sizeof(int), &dwByte, NULL);				// 사이즈
		for(vector<int>::iterator Snditer = Fstiter->second.begin();
			Snditer != Fstiter->second.end(); ++Snditer)
		{
			WriteFile(hFile, &(*Snditer), sizeof(int), &dwByte, NULL);
		}
	}
	//LPCVOID
	CloseHandle(hFile);
}

void CMapTool::OnLoadRoute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMapTool::OnRouteNum()
{
	UpdateData(TRUE);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
}

void CMapTool::OnReturnBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if(!m_VecRouteNum.empty())
	{
		(*m_pVecTile)[m_VecRouteNum.back()]->byDrawID = 0;
		m_VecRouteNum.pop_back();
	}
	((CMainFrame*)AfxGetMainWnd())->m_MainView->Invalidate(FALSE);

	UpdateData(FALSE);
}
