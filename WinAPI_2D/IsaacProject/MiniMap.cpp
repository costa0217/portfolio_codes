#include "StdAfx.h"
#include "MiniMap.h"

CMiniMap::CMiniMap(void)
:m_iRoomNum(0)
,m_bLeftMapOpen(false)
,m_bRightMapOpen(false)
{
}

CMiniMap::~CMiniMap(void)
{
}

void CMiniMap::Initialize(void)
{
	m_tInfo.fCX = 140.f;
	m_tInfo.fCY = 54.f;
	m_tFrame	= FRAME(0, 0, 2, 0);
}
int CMiniMap::Progress(void)
{
	if(m_bLeftMapOpen)
		m_tFrame.iScene = 1;
	if(m_bRightMapOpen)
		m_tFrame.iScene = 0;
	return 0;
}
void CMiniMap::Render(HDC hdc)
{
	TransparentBlt(hdc,						//¸Ê
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX) + 15, int(m_tInfo.fCY) + 5,
		(*m_pBitMap)["minimap"]->GetMemDC(),
		int(m_tInfo.fCX * m_tFrame.iStart),	
		int(m_tInfo.fCY * m_iRoomNum),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		RGB(61, 16, 123));

	if(!m_bLeftMapOpen || !m_bRightMapOpen)
	{
	TransparentBlt(hdc,						//¸Ê
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX) + 15, int(m_tInfo.fCY) + 5,
		(*m_pBitMap)["minimap_cover"]->GetMemDC(),
		int(m_tInfo.fCX * m_tFrame.iStart),	
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		RGB(61, 16, 123));
	}
}
void CMiniMap::Release(void)
{
}
RECT CMiniMap::GetRect()
{
	RECT rc = {
			0,
			0,
			0,
			-2000};

			return rc;
}