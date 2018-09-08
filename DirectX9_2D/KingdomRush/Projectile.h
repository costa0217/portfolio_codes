#pragma once
#include "obj.h"

class CProjectile :
	public CObj
{
protected:
	PROJECTILEID	m_eProID;

public:
	PROJECTILEID	GetProjectileID() { return m_eProID; }

public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Progress(void)		PURE;
	virtual void Render(void)		PURE;
	virtual void Release(void)		PURE;
public:
	CProjectile(void);
	virtual ~CProjectile(void);
};
