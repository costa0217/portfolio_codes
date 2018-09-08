#pragma once

#include "afxwin.h"
#include "Include.h"

// CItemTool ��ȭ �����Դϴ�.

class CItemTool : public CDialog
{
	DECLARE_DYNAMIC(CItemTool)

public:
	CItemTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
