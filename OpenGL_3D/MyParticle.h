#pragma once
#include "IncludeHeader.h"



class CMyParticle
{
public:
	MyVector		m_vDir;
	float			m_fSpeed;
	float			m_fLifeTime;
	int				m_eType;
	int				m_eTarget;
public:
	MyVector		m_vPos;
	MyVector		m_vRot;
	MyVector		m_vColor;

	MyVector		m_vTrail[10];

	float			m_fAccTime{ 0.f };

public:
	CMyParticle();
	CMyParticle( MyVector vDir, MyVector vPos)
		:
		m_vDir(vDir),
		m_vPos(vPos)
	{
		m_fSpeed = 1000.f;
		m_fLifeTime = 1.f;

		m_vRot = m_vDir * 180.f / M_PI;
	}
	~CMyParticle();


public:
	virtual void Initialize();
	virtual int Update(float fTime);
	virtual void Render();
	virtual void Release();
};

