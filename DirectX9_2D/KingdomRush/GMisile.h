#pragma once
#include "projectile.h"

class CGMisile :
	public CProjectile
{
private:
	list<CObj*>*	m_pEffectList;
	list<CObj*>*	m_pMonsterList;

	
	bool			m_bBoom;
	bool			m_bBoomImg;
	bool			m_bFirstBoom;

	float			m_fBoomRange;
	int				m_fBombDamage;

	CObj*			m_pMonster;

	D3DXVECTOR3		m_vTargetPos;
	D3DXVECTOR3		m_vPreTargeting;
	D3DXVECTOR3		m_vOriPos;

	bool			m_bFrist;
	bool			m_bRotate;
	bool			m_bChooseTarget;
	float			m_fRotTime;
	float			m_fImgTheta;
	int				m_iCnt;
	int				m_iMonNum;

public:
	float			GetBoomRange(){ return m_fBoomRange; }
	void			SetBombDamage(int _fBombDamage) { m_fBombDamage = _fBombDamage; }
	int				GetBombDamage(){ return m_fBombDamage; }

	bool			GetBoom(){ return m_bBoom; }
	bool			GetFirstBooom(){ return m_bFirstBoom; }
	void			SetFirstBooom(bool _bFirstBoom){ m_bFirstBoom = _bFirstBoom; }

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CGMisile(void);
	~CGMisile(void);
};
