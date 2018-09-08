#pragma once
#include "block.h"
#include "Include.h"

class CNone :
	public CBlock
{
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CNone(void);
	~CNone(void);
};
