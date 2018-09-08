#pragma once


#include "ObjectTool.h"
#include "afxwin.h"
#include "Include.h"

// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();


public:
	map<CString, CImage*>		m_TilePngImage;
	map<CString, CImage*>		m_MapPngImage;

	map<int, vector<int>>		m_MapRoute;		// �� ��Ʈ ����
	vector<int>					m_VecRouteNum;

	vector<TILE*>*				m_pVecTile;
	bool			m_bStart;
	int				m_iDrawID;


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()



public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickCombo();
	afx_msg void OnBnClickedRadio1();

public:
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickedRadio2();
	afx_msg void OnClickedRadio1();
	afx_msg void OnClickedRadio3();
	afx_msg void OnClickedRadio4();
	afx_msg void OnDeleteTile();
	afx_msg void OnSaveTile();
	afx_msg void OnLoadTile();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnSaveRoute();
	afx_msg void OnLoadRoute();
public:			//control
	CComboBox m_StageCombo;
	CButton		m_TileRadio[4];

public:		//value
	int m_iRouteNum;

	afx_msg void OnRouteNum();
	afx_msg void OnReturnBack();
};
