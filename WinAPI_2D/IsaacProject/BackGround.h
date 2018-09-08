#pragma once
#include "obj.h"

class CBackGround :
	public CObj
{
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);

public:
	CBackGround(void);
	~CBackGround(void);
};
