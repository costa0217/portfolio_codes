#pragma once
#include "ui.h"

class CTowerWnd2 :
	public CUI
{
private:
	bool			m_bRelly;		//·¤¸®Âï¾îÁÙ¶§

	float			m_fScale;
	float			fRangeScale;

	int				m_iRenderNum;
	D3DXVECTOR3 	m_vBtNum1;
	D3DXVECTOR3 	m_vBtNum2;
	D3DXVECTOR3 	m_vBtNum3;

	D3DXVECTOR3		m_vMousePos;
	D3DXVECTOR3		m_vRellyPoint;

	wstring			m_szMouseKey;

	list<CObj*>*	m_pPlayerList;
	list<CObj*>*	m_pTowerList;
	CObj*			m_pTower;
	vector<TILE*>*	m_pVecTile;

public:
	void SetTowerList(list<CObj*>*	_pTowerList) { m_pTowerList = _pTowerList; }
	void SetTower(CObj* _pTower){ m_pTower = _pTower; }
	void SetVecTile(vector<TILE*>* _pVecTile) { m_pVecTile = _pVecTile; }

	bool GetRelly(){ return m_bRelly; }

	bool KeyInput();
	void MouseInput();

private:
	bool Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	int  GetTileIndex(const D3DXVECTOR3& vPos);
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CTowerWnd2(void);
	~CTowerWnd2(void);
};
