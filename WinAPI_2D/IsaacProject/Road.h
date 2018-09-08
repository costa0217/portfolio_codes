#pragma once
#include "block.h"

class CRoad :
	public CBlock
{
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CRoad(void);
	~CRoad(void);
};
