#pragma once

#include "MapTool.h"
#include "UnitTool.h"
#include "ItemTool.h"
#include "ObjectTool.h"

// CMyFormView �� ���Դϴ�.

class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyFormView();

public:
	CFont			m_Font;
	CMapTool		m_MapTool;
	CUnitTool		m_UnitTool;
	CItemTool		m_ItemTool;
	CObjectTool		m_ObjTool;
	

public:
	enum { IDD = IDD_MYFORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMapToolButton();
	afx_msg void OnItemToolButton();
	afx_msg void OnObjectToolButton();
	afx_msg void OnUnitToolButton();
};