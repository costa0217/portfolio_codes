#pragma once
#include "block.h"

class CPoop :
	public CBlock
{
private:
	bool	m_bPoopCrush;
	bool	m_bPoopFade;

public:
	void	SetPoopCrush(bool _bPoopCrush){ m_bPoopCrush = _bPoopCrush; }
	bool	GetPoopFade(){ return m_bPoopFade; }
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CPoop(void);
	~CPoop(void);
};
