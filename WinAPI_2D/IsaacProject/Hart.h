#pragma once
#include "ui.h"

class CHart :
	public CUI
{
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CHart(void);
	~CHart(void);
};
