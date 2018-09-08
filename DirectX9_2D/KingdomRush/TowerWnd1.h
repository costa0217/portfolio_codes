#pragma once
#include "ui.h"

class CTowerWnd1 :
	public CUI
{
private:
	float			m_fScale;
	int				m_iRenderNum;
	D3DXVECTOR3 	m_vBtNum1;
	D3DXVECTOR3 	m_vBtNum2;
	D3DXVECTOR3 	m_vBtNum3;
	D3DXVECTOR3 	m_vBtNum4;

	float			m_fRange;
	BYTE			m_byStyle;	

	list<CObj*>*	m_pPlayerList;
	list<CObj*>*	m_pSoldierList;
	list<CObj*>*	m_pTowerList;
	CObj*			m_pTower;

public:
	void SetSoldierList(list<CObj*>*	_pSoldierList) { m_pSoldierList = _pSoldierList; }
	void SetTowerList(list<CObj*>*	_pTowerList) { m_pTowerList = _pTowerList; }
	void SetTower(CObj* _pTower){ m_pTower = _pTower; }

public:
	void	MouseInput();
	bool	KeyInput();
	void	TowerCreate();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CTowerWnd1(void);
	~CTowerWnd1(void);
};
