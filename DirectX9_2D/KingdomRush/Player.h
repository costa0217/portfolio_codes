#pragma once
#include "obj.h"

class CPlayer :
	public CObj
{
private:
	CObj*	m_pStageBar;
	int		m_iLife;
	int		m_iWave;
	int		m_iMaxWave;
	int		m_iGold;

public:
	void	SetStageBar(CObj* _pStageBar){ m_pStageBar = _pStageBar; }

public:
	void	SetLife(int _iLife){ m_iLife = _iLife; }
	int		GetLife(){ return m_iLife; }

	void	SetWave(int _iWave){ m_iWave = _iWave; }
	int		GetWave(){ return m_iWave; }

	void	SetMaxWave(int _iMaxWave){ m_iMaxWave = _iMaxWave; }
	int		GetMaxWave(){ return m_iMaxWave; }

	void	SetGold(int _iGold){ m_iGold = _iGold; }
	int		GetGold(){ return m_iGold; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CPlayer(void);
	~CPlayer(void);
};
