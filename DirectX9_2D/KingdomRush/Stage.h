#pragma once
#include "scene.h"

class CStage :
	public CScene
{
public:
	virtual HRESULT Initialize(void)	;
	virtual int Progress(void)		PURE;
	virtual void Render(void)		PURE;
	virtual void Release(void)		PURE;
public:
	CStage(void);
	virtual ~CStage(void);
};
