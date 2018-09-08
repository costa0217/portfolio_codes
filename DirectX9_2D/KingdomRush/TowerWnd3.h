#pragma once
#include "ui.h"

class CTowerWnd3 :
	public CUI
{
private:
	bool			m_bMouseOn;

	float			m_fScale;
	float			m_fRange;
	int				m_iRenderNum;
	D3DXVECTOR3 	m_vBtNum1;
	D3DXVECTOR3 	m_vBtNum2;

	list<CObj*>*	m_pPlayerList;
	list<CObj*>*	m_pTowerList;
	CObj*			m_pTower;
public:
	void SetTowerList(list<CObj*>*	_pTowerList) { m_pTowerList = _pTowerList; }
	void SetTower(CObj* _pTower){ m_pTower = _pTower; }

	bool KeyInput();
	void MouseInput();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CTowerWnd3(void);
	~CTowerWnd3(void);
};
