#pragma once
#include "obj.h"

class CFamilyer :
	public CObj
{
public:
	virtual void Initialize(void)PURE;
	virtual int Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	CFamilyer(void);
	virtual ~CFamilyer(void);
};
