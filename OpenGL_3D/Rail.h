#pragma once
#include "GameObject.h"
class CRail :
	public CGameObject
{
public:
	float	fTerm{ 0.f };
public:
	CRail();
	~CRail();

public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();
};

