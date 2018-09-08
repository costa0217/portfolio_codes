#include "StdAfx.h"
#include "Tower.h"

CTower::CTower(void)
{
}

CTower::~CTower(void)
{
}
void	CTower::SetLevelup(int _iLevel)
{
	m_bLevelup		= true;
	m_bLvl[_iLevel] = true;
}