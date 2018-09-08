#pragma once
#include "obj.h"

class CBullet :
	public CObj
{
protected:
	//INFO	m_tCOlInfo;
	BULLETID	m_eBulletType;

public:
	BULLETID	GetBulletID() {return m_eBulletType;}
public:
	virtual void Initialize(void)PURE;
	virtual int Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;

	virtual RECT GetRect();
public:
	CBullet(void);
	virtual ~CBullet(void);
};
