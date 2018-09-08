#pragma once
#include "projectile.h"

class CElecRazor :
	public CProjectile
{
private:
	CObj*			m_pMonster;
	float			m_fRazorLength;
	bool			m_bChain;
	int				m_iChainNum;

	list<CObj*>*	m_pMonsterList;
	list<CObj*>*	m_pBulletList;
	list<CObj*>*	m_pEffectList;

	int				m_iDamage;
public:
	void			SetMonTarget(CObj* _pMonster){ m_pMonster = _pMonster; }

	void			SetChain(bool	_bChain){ m_bChain = _bChain; }
	bool			GetChain(){ return m_bChain; }

	void			SetChainNum(int _iChainNum){ m_iChainNum = _iChainNum; }
	int				GetChainNum(){ return m_iChainNum; }

	void			SetBombDamage(int _iDamage){ m_iDamage = _iDamage; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CElecRazor(void);
	~CElecRazor(void);
};
