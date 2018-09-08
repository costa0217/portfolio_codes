#pragma once
#include "ui.h"

class CStartButt :
	public CUI
{
private:
	bool*			m_pbWave;
	bool*			m_pbWaveStart;
	bool*			m_pbButtDown;
	float*			m_pfNextTime;

	int				m_iSwitch;
	list<CObj*>*	m_pPlayerList;


public:
	void	SetWave(bool* _pbWave){ m_pbWave = _pbWave; }
	void	SetWaveStart(bool* _bStart){ m_pbWaveStart = _bStart; }
	void	SetButtDown(bool* _bButtDown){ m_pbButtDown = _bButtDown; }

	void	SetNextTime(float*	_pfNextTime){ m_pfNextTime = _pfNextTime; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CStartButt(void);
	~CStartButt(void);
};
