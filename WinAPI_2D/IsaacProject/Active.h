#pragma once
#include "obj.h"

class CActive :
	public CObj
{
private:
	INFO	m_tOriInfo;
	bool	m_bItemFade;
	DWORD	m_dwItemFadeTime;
public:
	void SetItemFade(bool _bItemFade) {m_bItemFade = _bItemFade;}
	bool GetItemFade() {return m_bItemFade;}
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CActive(void);
	~CActive(void);
};
