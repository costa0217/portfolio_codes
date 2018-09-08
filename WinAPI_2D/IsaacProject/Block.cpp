#include "StdAfx.h"
#include "Block.h"

CBlock::CBlock(void)
{
}

CBlock::~CBlock(void)
{
}

void CBlock::SetTile(TILE* pTile)
{
	m_pTile = pTile;
}
void CBlock::SetTileSize(float _fCX, float _fCY)
{
	m_tCOlInfo.fCX = _fCX;
	m_tCOlInfo.fCY = _fCY;
}
RECT CBlock::GetColRect()
{
	RECT rc = {
			int(m_tCOlInfo.fX - m_tCOlInfo.fCX / 2.f),
			int(m_tCOlInfo.fY - m_tCOlInfo.fCY / 2.f),
			int(m_tCOlInfo.fX + m_tCOlInfo.fCX / 2.f),
			int(m_tCOlInfo.fY + m_tCOlInfo.fCY / 2.f)};

			return rc;
}