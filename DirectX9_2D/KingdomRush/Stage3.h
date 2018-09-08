#pragma once
#include "stage.h"

class CStage3 :
	public CStage
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CStage3(void);
	~CStage3(void);
};
