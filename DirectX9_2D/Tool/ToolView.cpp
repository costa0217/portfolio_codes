
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "ToolDoc.h"
#include "ToolView.h"

#include "MainFrm.h"
#include "Device.h"
#include "TextureMgr.h"

#include "MainFrm.h"
#include "MapTool.h"
#include "MyFormView.h"

#include "BackGround.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CToolView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
:m_Map(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CString		strSelectName;
	int			iSelectIndex = -1;
	D3DXMATRIX	matTrans;

	if(m_Map)
		iSelectIndex = ((CMainFrame*)AfxGetMainWnd())->m_FormView->m_MapTool.m_StageCombo.GetCurSel();

	if(iSelectIndex != -1)
		((CMainFrame*)AfxGetMainWnd())->m_FormView->m_MapTool.m_StageCombo.GetLBText(iSelectIndex, strSelectName);



	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	CDevice::GetInstance()->GetDevice()->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	CDevice::GetInstance()->Render_Begin();

	if(iSelectIndex != -1)
	{
		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture((wstring)strSelectName);

		D3DXMatrixTranslation(&matTrans, - (FLOAT)GetScrollPos(0), - (FLOAT)GetScrollPos(1), 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
		((CBackGround*)m_pBackGround)->Render();
	}

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->GetDevice()->Present(NULL, NULL, NULL, NULL);

	//Invalidate(FALSE);
}
// CToolView 인쇄


void CToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

void CToolView::OnInitialUpdate()
{
	SetScrollSizes(MM_TEXT, CSize(2 * WINCX, 2 * WINCY));

	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);	

	SetRect(&rcWindow, 0, 0, 
		rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);		

	float fRowFrm = float(rcWindow.right - rcMainView.right);
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrm->SetWindowPos(NULL, 
		0,0, 
		int(WINCX + fRowFrm), 
		int(WINCY + fColFrm), 
		SWP_NOZORDER);

	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_hWnd = m_hWnd;

	if(FAILED(CDevice::GetInstance()->InitDevice()))
	{
		AfxMessageBox(L"디바이스 장치 초기화 실패");
		return;
	}

	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Stage/1.png", 
		L"Stage1", TEX_SINGLE )))
	{
		AfxMessageBox(L"Map 텍스쳐 생성 실패");
		return;
	}

	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Stage/2.png", 
		L"Stage2", TEX_SINGLE )))
	{
		AfxMessageBox(L"Map 텍스쳐 생성 실패");
		return;
	}

	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Stage/3.png", 
		L"Stage3", TEX_SINGLE )))
	{
		AfxMessageBox(L"Map 텍스쳐 생성 실패");
		return;
	}

	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Stage/4.png", 
		L"Stage4", TEX_SINGLE )))
	{
		AfxMessageBox(L"Map 텍스쳐 생성 실패");
		return;
	}

	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Tile/%d.png", 
		L"BACK", TEX_MULTI, L"Tile", 4)))
	{
		AfxMessageBox(L"tile 타일 텍스쳐 생성 실패");
		return;
	}

	m_pBackGround = new CBackGround;
	m_pBackGround->Initialize();
	((CBackGround*)m_pBackGround)->SetMainView(this);
}

BOOL CToolView::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	


	return CScrollView::DestroyWindow();
}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	CDevice::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();

	::Safe_Delete(m_pBackGround);
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	D3DXVECTOR3	vMousePos = D3DXVECTOR3( 
		GetMouse().x + GetScrollPos(0), 
		GetMouse().y + GetScrollPos(1), 0.f);

	CMapTool*		pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_FormView->m_MapTool;

	if(pMapTool->m_iDrawID == 3)
	{
		int iTempIndex = ((CBackGround*)m_pBackGround)->GetTileIndex(vMousePos);
		if(iTempIndex < 0)
			return;
		if( (*(pMapTool->m_pVecTile))[iTempIndex]->byDrawID != 3 )
		{
			pMapTool->m_VecRouteNum.push_back(iTempIndex);
		}
	}
	((CBackGround*)m_pBackGround)->TileChange(vMousePos, pMapTool->m_iDrawID);

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);
	CMapTool*		pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_FormView->m_MapTool;
	if(pMapTool->m_iDrawID == 0 || pMapTool->m_iDrawID == 1 )
	{
		if(GetAsyncKeyState(VK_LBUTTON))
		{
			OnLButtonDown(nFlags, point);
		}
	}
}
