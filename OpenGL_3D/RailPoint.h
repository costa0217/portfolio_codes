#pragma once
#include "GameObject.h"


class CRailPoint :
	public CGameObject
{
public:
	CRailPoint();
	~CRailPoint();
public:
	vector<CGameObject*>*	m_pVecRailPoint;
	vector<CGameObject*>	m_vecRail;

	
	int	m_iMyNum{ 0 };
public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();
};

