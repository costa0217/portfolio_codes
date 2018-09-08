#pragma once

#include "Engine_Include.h"


BEGIN(Engine)

class ENGINE_DLL CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	DWORD		m_dwKey;
	DWORD		m_dwKeyPressed;
	DWORD		m_dwKeyPress[3];

	DWORD		m_dwDown;

public:
	void KeyCheck(void);
	bool KeyPressed(DWORD _dwKey);
	bool KeyPressed(DWORD _dwKey, BYTE byKeyIndex);

	bool KeyDown(DWORD _dwKey);

	bool KeyCombine(DWORD _dwFirstKey, DWORD _dwSecondKey);

private:
	CKeyMgr(void);
	~CKeyMgr(void);
};


END