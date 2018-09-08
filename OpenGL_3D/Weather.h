#pragma once
#include "GameObject.h"
class CWeather :
	public CGameObject
{
public:
	MyVector	m_vInit;

	float		m_fXZ{ 0.f };
public:
	CWeather();
	~CWeather();

public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();
};

