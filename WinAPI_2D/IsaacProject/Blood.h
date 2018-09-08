#pragma once
#include "obj.h"

class CBlood :
	public CObj
{
private:
	int		m_iNum;
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBlood(void);
	~CBlood(void);
};
