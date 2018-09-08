#pragma once
#include "obj.h"

class CAlpha :
	public CObj
{
private:
	int			m_iNum;
	BLENDFUNCTION bf;
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CAlpha(void);
	~CAlpha(void);
};
