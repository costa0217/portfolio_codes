#pragma once
#include "ui.h"

class CTowerWnd5 :
	public CUI
{
private:
	bool			m_bRelly;		//·¤¸®Âï¾îÁÙ¶§

	float			m_fScale;
	float			fRangeScale;

	float			m_fRange;
	int				m_iRenderNum;
	int				m_iTowerLvl;

	D3DXVECTOR3 	m_vBtNum1;
	D3DXVECTOR3 	m_vBtNum2;
	D3DXVECTOR3 	m_vBtNum3;
	D3DXVECTOR3 	m_vBtNum4;

	D3DXVECTOR3 	m_vSkillOK[2];

	D3DXVECTOR3		m_vMousePos;
	D3DXVECTOR3		m_vRellyPoint;

	wstring			m_szMouseKey;

	list<CObj*>*	m_pPlayerList;
	list<CObj*>*	m_pTowerList;
	CObj*			m_pTower;
	vector<TILE*>*	m_pVecTile;

	int				m_iSkillCnt;

	int				m_iTowerSkillOn[2];

public:
	void SetTowerList(list<CObj*>*	_pTowerList) { m_pTowerList = _pTowerList; }
	void SetTower(CObj* _pTower){ m_pTower = _pTower; }
	void SetTowerLvl(int _iTowerLvl){ m_iTowerLvl = _iTowerLvl; }
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
	CTowerWnd5(void);
	~CTowerWnd5(void);
};
