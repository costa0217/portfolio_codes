#pragma once
#include "GameObject.h"
class CSelectObj :
	public CGameObject
{

private:
	bool m_bPresentReady{ false };
	bool m_bReadyWeapon{ false };
	int	m_nAnimNum{ 0 };
public:
	void SetReady(bool bReady) { m_bReadyWeapon = bReady; }

public:
	bool Begin();
	virtual void Animate(float fTimeElapsed);

public:
	virtual void RegistToContainer();

public:
	CSelectObj(string name, tag t = tag::TAG_DEFAULT);
	~CSelectObj();
};

