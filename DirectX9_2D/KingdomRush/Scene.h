#pragma once
#include "Include.h"

class CScene
{
public:
	virtual HRESULT Initialize(void)PURE;
	virtual int		Progress(void)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;
public:
	CScene(void);
	virtual ~CScene(void);
};
