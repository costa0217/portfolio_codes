#pragma once
#include "obj.h"

class CObj;
class CMonster :
	public CObj
{
protected:
	INFO		m_tColInfo;
	DWORD		m_dwPoofTime;
	FRAME		m_tPoofFrame;
	FRAME		m_tMonDead;
	bool		m_appear;

	MONSTERID	m_eMonsterType;

public:
	RECT		GetColRect();
	MONSTERID	GetMonsterID() { return m_eMonsterType; }
	void		SetColSize(float _fCX, float _fCY);
public:
	virtual void Initialize(void)PURE;
	virtual int Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	CMonster(void);
	virtual ~CMonster(void);
};
