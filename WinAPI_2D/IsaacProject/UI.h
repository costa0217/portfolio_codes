#pragma once
#include "obj.h"

class CObj;
class CUI :
	public CObj
{
private:
	CObj*	m_pPlayer;
public:
	void SetPlayer(CObj* _pPlayer);
	
	virtual RECT GetRect();
public:
	virtual void Initialize(void)PURE;
	virtual int Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	CUI(void);
	virtual ~CUI(void);
};
