#include "StdAfx.h"
#include "Soldier.h"

CSoldier::CSoldier(void)
{
}

CSoldier::~CSoldier(void)
{
}
void	CSoldier::SetLevelup(int _iLevel)
{
	m_bLevelup		= true;
	m_bLvl[_iLevel] = true;
}