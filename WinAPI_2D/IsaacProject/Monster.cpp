#include "StdAfx.h"
#include "Monster.h"

CMonster::CMonster(void)
{
}

CMonster::~CMonster(void)
{
}
RECT CMonster::GetColRect()
{
	RECT rc = {
		int(m_tInfo.fX - m_tColInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tColInfo.fCY / 2.f),
		int(m_tInfo.fX + m_tColInfo.fCX / 2.f),
		int(m_tInfo.fY + m_tColInfo.fCY / 2.f)};

		return rc;
}
void CMonster::SetColSize(float _fCX, float _fCY)
{
	m_tColInfo.fCX = _fCX;
	m_tColInfo.fCY = _fCY;
}