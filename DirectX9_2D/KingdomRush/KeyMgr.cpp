#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
: m_dwKey(0)
, m_dwTime(GetTickCount())
, m_dwClearTime(500)
, m_dwKeyPressed(0)
, m_dwDown(0)
{
	ZeroMemory(m_dwKeyPress, sizeof(DWORD) * KEYNUM_END);
}

CKeyMgr::~CKeyMgr(void)
{
}

void CKeyMgr::KeyCheck()
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_dwKey |= KEY_LBUTTON;
	}
	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_dwKey |= KEY_RBUTTON;
	}
	if(GetAsyncKeyState('1') & 0x8000)
	{
		m_dwKey |= KEY_1;
	}
	if(GetAsyncKeyState('2') & 0x8000)
	{
		m_dwKey |= KEY_2;
	}
	if(GetAsyncKeyState('3') & 0x8000)
	{
		m_dwKey |= KEY_3;
	}
	if(GetAsyncKeyState('Q') & 0x8000)
	{
		m_dwKey |= KEY_Q;
	}
}


bool CKeyMgr::KeyPressed(DWORD _dwKey)
{
	if(m_dwKey & _dwKey)
	{
		m_dwKeyPressed |= _dwKey;
		return false;
	}

	else if(m_dwKeyPressed & _dwKey)
	{
		m_dwKeyPressed = m_dwKey;
		return true;
	}

	return false;
}

bool CKeyMgr::KeyPressed(DWORD _dwKey, BYTE byKeyIndex)
{
	if(m_dwKey & _dwKey)
	{
		m_dwKeyPress[byKeyIndex] |= _dwKey;
		return false;
	}

	else if(m_dwKeyPress[byKeyIndex] & _dwKey)
	{
		m_dwKeyPress[byKeyIndex] = m_dwKey;
		return true;
	}
	return false;
}


bool CKeyMgr::KeyDown(DWORD _dwKey)
{
	if(KeyPressed(_dwKey))
		m_dwDown ^= _dwKey;
	
	if(!(m_dwDown & _dwKey))
	{
		if(m_dwKey & _dwKey)
		{
			m_dwDown |= _dwKey;
			return true;
		}
	}
	return false;
}

bool CKeyMgr::KeyCombine(DWORD _dwFirstKey, DWORD _dwSecondKey)
{
	if(m_dwKey & _dwFirstKey)
	{
		if(KeyDown(_dwSecondKey))
			return true;
	}
	return false;
}
