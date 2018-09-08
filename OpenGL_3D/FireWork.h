#pragma once
#include "GameObject.h"

class CMyParticle;
class CFireWork :
	public CGameObject
{

public:
	vector<CMyParticle*> m_vecParticle;
public:
	CFireWork();
	~CFireWork();

public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();
};

