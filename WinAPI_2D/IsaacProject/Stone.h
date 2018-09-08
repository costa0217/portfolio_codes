#pragma once
#include "block.h"
#include "Include.h"

class CStone :
	public CBlock
{
private:
	bool	m_bStoneFade;
	bool	m_bStoneCrush;
public:
	void	SetStoneCrush(bool _bStoneCrush){ m_bStoneCrush = _bStoneCrush; }
	bool	GetStoneFade() { return m_bStoneFade; }
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CStone(void);
	~CStone(void);
};
