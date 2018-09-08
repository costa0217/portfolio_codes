#pragma once
#include "tower.h"

class CKnightTower :
	public CTower
{
private:
	bool			m_bWark;
	SHEETFRM		m_tWarkFrame;
	DWORD			m_dwWarkTime;

	D3DXVECTOR3		m_vRellyPoint;

	D3DXVECTOR3		m_vPreRelly;

	list<CObj*>*	m_pSoldierList;
	vector<CObj*>	m_VecMySoldier;

	float			m_fRespawnTime;

public:
	void		SetRellyPoint(D3DXVECTOR3 _vRelly){ m_vRellyPoint = _vRelly; }
	void		SetSoldierList( list<CObj*>* _pSoldierList) { m_pSoldierList = _pSoldierList; }

private:
	void		FrameMove();
	void		Leveling();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CKnightTower(void);
	~CKnightTower(void);
};

