#pragma once
#include "GameObject.h"

class CMyParticle;
class CPartiEffect :
	public CGameObject
{
public:
	vector<CMyParticle*> m_vecParticle;
	int		m_eTarget;
public:
	CPartiEffect();
	~CPartiEffect();

public:
	virtual void Initialize();
	virtual int Update(float fTime);
	virtual void Render();
	virtual void Release();
};

