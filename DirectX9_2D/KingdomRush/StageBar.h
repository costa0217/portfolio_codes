#pragma once
#include "ui.h"

class CStageBar :
	public CUI
{
private:
	list<CObj*>*	m_pPlayerList;
	CObj*			m_pPlayer;

	int				m_iLife;
	int				m_iWave;
	int				m_iMaxWave;
	int				m_iGold;
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CStageBar(void);
	~CStageBar(void);
};
