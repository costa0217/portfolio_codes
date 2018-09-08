#pragma once
#include "monster.h"

class CSpider :
	public CMonster
{
private:
	list<CObj*>*	m_pMonsterList;
	list<CObj*>*	m_pEffectList;

	float			m_fEggTime;
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CSpider(void);
	~CSpider(void);
};
