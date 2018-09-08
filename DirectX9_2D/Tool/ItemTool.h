#pragma once

#include "afxwin.h"
#include "Include.h"

// CItemTool 대화 상자입니다.

class CItemTool : public CDialog
{
	DECLARE_DYNAMIC(CItemTool)

public:
	CItemTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public: // General
	list<IMGPATH*>			m_PathInfoList;

public:
	afx_msg void OnListBoxFunc();
	CListBox m_ListBox;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSave();
	afx_msg void OnLoad();
};
