
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(Engine::CKeyMgr)

Engine::CKeyMgr::CKeyMgr(void)
: m_dwKey(0)
, m_dwKeyPressed(0)
, m_dwDown(0)
{
	ZeroMemory(m_dwKeyPress, sizeof(DWORD) * 3);
}

Engine::CKeyMgr::~CKeyMgr(void)
{
}

void Engine::CKeyMgr::KeyCheck(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;

	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;

	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;

	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;

	if(GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;

	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;

	if(GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_ENTER;

	if(GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1;

	if(GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2;

	if(GetAsyncKeyState('3') & 0x8000)
		m_dwKey |= KEY_3;

	if(GetAsyncKeyState('4') & 0x8000)
		m_dwKey |= KEY_4;


}

bool Engine::CKeyMgr::KeyPressed(DWORD _dwKey)
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

bool Engine::CKeyMgr::KeyPressed(DWORD _dwKey, BYTE byKeyIndex)
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


bool Engine::CKeyMgr::KeyDown(DWORD _dwKey)
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

bool Engine::CKeyMgr::KeyCombine(DWORD _dwFirstKey, DWORD _dwSecondKey)
{
	if(m_dwKey & _dwFirstKey)
	{
		if(KeyDown(_dwSecondKey))
			return true;
	}

	return false;
}
