#pragma once
#include "ui.h"

class CActiveBar :
	public CUI
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CActiveBar(void);
	~CActiveBar(void);
};
