#pragma once
#include "scene.h"

class CMoveScene :
	public CScene
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CMoveScene(void);
	~CMoveScene(void);
};
