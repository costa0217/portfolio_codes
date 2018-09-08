#pragma once
#include "obj.h"

class CPlayer :
	public CObj
{

public:
	virtual void Initialize(void)PURE;
	virtual int Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	CPlayer(void);
	virtual ~CPlayer(void);
};
