#pragma once
#include "GameObject.h"
class CTernel :
	public CGameObject
{
public:
	int		m_iRailCnt{ 0 };
public:
	CTernel();
	~CTernel();
public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();
};

