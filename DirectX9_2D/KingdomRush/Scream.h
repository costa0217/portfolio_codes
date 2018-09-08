#pragma once
#include "effect.h"

class CScream :
	public CEffect
{
private:
	SCREAM		m_eScream;
	float		m_fScreaTime;

	int			m_iRed;
	int			m_iGreen;
	int			m_iBlue;

	int			m_iRandom;

public:
	void SetScream(SCREAM _eScream){ m_eScream = _eScream; }

	void SetRGB(int _R, int _G, int _B);
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CScream(void);
	~CScream(void);
};
