// MyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyFormView.h"


// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(CMyFormView::IDD)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyFormView::OnMapToolButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyFormView::OnItemToolButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyFormView::OnObjectToolButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyFormView::OnUnitToolButton)
END_MESSAGE_MAP()


// CMyFormView 진단입니다.

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView 메시지 처리기입니다.

void CMyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_Font.CreatePointFont(130, L"굴림");
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON3)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON4)->SetFont(&m_Font);
}

void CMyFormView::OnMapToolButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_MapTool.GetSafeHwnd() == NULL)
	{
		m_MapTool.Create(IDD_MAPTOOL);
	}

	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyFormView::OnItemToolButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_ItemTool.GetSafeHwnd() == NULL)
	{
		m_ItemTool.Create(IDD_ITEMTOOL);
	}

	m_ItemTool.ShowWindow(SW_SHOW);
}

void CMyFormView::OnObjectToolButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_ObjTool.GetSafeHwnd() == NULL)
	{
		m_ObjTool.Create(IDD_OBJECTTOOL);
	}

	m_ObjTool.ShowWindow(SW_SHOW);
}

void CMyFormView::OnUnitToolButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_UnitTool.GetSafeHwnd() == NULL)
	{
		m_UnitTool.Create(IDD_UNITTOOL);
	}

	m_UnitTool.ShowWindow(SW_SHOW);
}
