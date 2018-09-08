#pragma once
#include "ui.h"

class CSkillButt :
	public CUI
{
private:
	map<int,vector<int>>*	m_pmapRoute;
	vector<TILE*>*			m_pVecTile;

	FRAME			m_tCloseFrame;
	FRAME			m_tActiveFrame;
	FRAME			m_tNormalFrame;
	int				m_iIndex;

	wstring			m_wstrSkillImg;

	bool*			m_bMeteoLV;
	bool*			m_bMillLV;

	bool			m_bMeteo;
	int				m_iMeteoNum;

	list<CObj*>*	m_pProjecList;
	list<CObj*>*	m_pSoldierList;
	D3DXVECTOR3		m_vPoint;
	
	bool			m_bMeteoCool;
	bool			m_bMillCool;
	float			m_fMeteoCoolTime;
	float			m_fMillCoolTime;
public:
	void	SetMapRoute(map<int,vector<int>>* _pmapRoute){ m_pmapRoute = _pmapRoute; }
	void	SetVecTile(vector<TILE*>*	_pVecTile){ m_pVecTile = _pVecTile; }
public:
	void	KeyInput();
	
	void	SetMeteoLV(bool* _bMeteoLV){ m_bMeteoLV = _bMeteoLV; }
	void	SetMillLV(bool* _bMillLV){ m_bMillLV = _bMillLV; }

public:
	void	SetIndex(int _iIndex){ m_iIndex = _iIndex; }
	int		GetIndex(){ return m_iIndex; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CSkillButt(void);
	~CSkillButt(void);
};
