#pragma once
#include "obj.h"

class CHero :
	public CObj
{
public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Progress(void)		PURE;
	virtual void Render(void)		PURE;
	virtual void Release(void)		PURE;
public:
	CHero(void);
	virtual ~CHero(void);
};
