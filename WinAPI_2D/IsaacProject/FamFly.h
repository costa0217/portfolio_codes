#pragma once
#include "familyer.h"

class CObj;
class CFamFly :
	public CFamilyer
{
private:
	CObj*	m_pPlayer;
	INFO	m_tPlayInfo;
	DWORD	m_dwFamFlyTime;
public:
	void SetPlayer(CObj* _pPlayer){ m_pPlayer = _pPlayer; }
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CFamFly(void);
	~CFamFly(void);
};
