#include "StdAfx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInst = NULL;

CKeyMgr::CKeyMgr(void)
: m_dwKey(0)
{
}

CKeyMgr::~CKeyMgr(void)
{
}

void CKeyMgr::KeyCheck(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_LEFT;

	if(GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_RIGHT;

	if(GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_UP;

	if(GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_DOWN;

}

DWORD CKeyMgr::GetKey(void)
{
	return m_dwKey;
}
