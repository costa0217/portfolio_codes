#pragma once
#include "IncludeHeader.h"
//#include "MyTri.h"

class CCollide;
class CGameObject
{
public:
	MyVector	m_vPos;
	MyVector	m_vRot;
	MyVector	m_vScale{ MyVector(1.f, 1.f, 1.f) };

	MyVector	m_vDir;
	MyVector	m_vColor;

	float		m_fAccTime{ 0.f };
	bool		m_bAnimate{ false };
	WORD		m_wKey{ 0 };

public:
	OBJ_TYPE	m_eType;
	bool		m_bFirst{ false };

	MATRIX		m_matWorld;
	int			m_iWeatherType{ 0 };

public:
	float		m_fRadius{ 0.f };
	float		m_fYAngle{ 0.f };
	float		m_fXAngle{ 0.f };
	float		m_fSpeed{ 0.f };

public:
	void SetPos(MyVector vec) { m_vPos = vec; }

public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();

public:
	virtual void	Animate() {};

public:
	CGameObject();
	CGameObject(MyVector pt) :m_vPos(pt){};
	virtual ~CGameObject();
};

