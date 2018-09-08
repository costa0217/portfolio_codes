#pragma once
#include "projectile.h"

class CBomb :
	public CProjectile
{
private:
	list<CObj*>*	m_pEffectList;
	float			m_fTime;
	bool			m_bBoom;
	bool			m_bBoomImg;
	bool			m_bFirstBoom;

	float			m_fBoomRange;
	int				m_fBombDamage;

	CObj*			m_pMonster;

	D3DXVECTOR3		m_vTargetPos;
	D3DXVECTOR3		m_vPreTargeting;
public:
	void			SetTargetPos(D3DXVECTOR3 _vTargetPos) { m_vTargetPos = _vTargetPos; }

	void			SetBombObjKey(wstring _ObjKey){ m_wstrObjKey = _ObjKey; }
	void			SetMonTarget(CObj* _pMonster){ m_pMonster = _pMonster; }

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
	CBomb(void);
	~CBomb(void);
};
