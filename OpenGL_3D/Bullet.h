#pragma once
#include "GameObject.h"
class CBullet :
	public CGameObject
{

public:
	CBullet();
	~CBullet();

public:
	virtual void Initialize();
	virtual int Update(float fTime);
	virtual void Render();
	virtual void Release();
};

