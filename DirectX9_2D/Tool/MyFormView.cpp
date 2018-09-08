// MyFormView.cpp : ���� �����Դϴ�.
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


// CMyFormView �����Դϴ�.

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


// CMyFormView �޽��� ó�����Դϴ�.

void CMyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_Font.CreatePointFont(130, L"����");
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON3)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON4)->SetFont(&m_Font);
}

void CMyFormView::OnMapToolButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_MapTool.GetSafeHwnd() == NULL)
	{
		m_MapTool.Create(IDD_MAPTOOL);
	}

	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyFormView::OnItemToolButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_ItemTool.GetSafeHwnd() == NULL)
	{
		m_ItemTool.Create(IDD_ITEMTOOL);
	}

	m_ItemTool.ShowWindow(SW_SHOW);
}

void CMyFormView::OnObjectToolButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_ObjTool.GetSafeHwnd() == NULL)
	{
		m_ObjTool.Create(IDD_OBJECTTOOL);
	}

	m_ObjTool.ShowWindow(SW_SHOW);
}

void CMyFormView::OnUnitToolButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_UnitTool.GetSafeHwnd() == NULL)
	{
		m_UnitTool.Create(IDD_UNITTOOL);
	}

	m_UnitTool.ShowWindow(SW_SHOW);
}
