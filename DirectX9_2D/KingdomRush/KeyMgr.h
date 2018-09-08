#pragma once

#include "Include.h"

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	DWORD		m_dwKey;
	DWORD		m_dwTime;
	DWORD		m_dwClearTime;

	DWORD		m_dwKeyPressed;
	DWORD		m_dwKeyPress[KEYNUM_END];
	DWORD		m_dwDown;


public:
	void	KeyCheck(void);
	DWORD	GetKey(void) { return m_dwKey; }

	bool	KeyPressed(DWORD _dwKey);
	bool	KeyPressed(DWORD _dwKey, BYTE byKeyIndex);
	bool	KeyDown(DWORD _dwKey);
	bool	KeyCombine(DWORD _dwFirstKey, DWORD _dwSecondKey);

private:
	CKeyMgr(void);
	~CKeyMgr(void);
};
