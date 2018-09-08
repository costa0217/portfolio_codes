#pragma once
#include "projectile.h"

class CMeteo :
	public CProjectile
{
private:
	bool			m_bLevel;
	D3DXVECTOR3		m_vTarget;

	float			m_fAccelTime;

	float			m_fImgTheta;

	bool			m_bFirstBoom;
	bool			m_bBoomImg;
	bool			m_bBoom;
	float			m_fBoomRange;
	int				m_iBoomDamage;

	bool			m_bCrator;

	list<CObj*>*	m_pProjectileList;

public:
	void	SetCrator(bool _bCrator){ m_bCrator = _bCrator; }
	void	SetTarget(D3DXVECTOR3 _vTarget){ m_vTarget = _vTarget; }
public:
	bool	GetFirstBooom(){ return m_bFirstBoom; }
	void	SetFirstBooom(bool _bFirstBoom) { m_bFirstBoom = _bFirstBoom;}

	bool	GetBoomImg(){return m_bBoomImg;}
	bool	GetBoom(){return m_bBoom;}
	float	GetBoomRange(){return m_fBoomRange; }
	int		GetBoomDamage(){return m_iBoomDamage; }

public:
	
	
	bool	GetLevel(){ return m_bLevel; }
	void	SetLevel(bool _bLevel){ m_bLevel = _bLevel; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CMeteo(void);
	~CMeteo(void);
};