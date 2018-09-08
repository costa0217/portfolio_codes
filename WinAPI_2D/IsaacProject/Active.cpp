#include "StdAfx.h"
#include "Active.h"

#include "Device.h"

CActive::CActive(void)
{
}

CActive::~CActive(void)
{
}
void CActive::Initialize(void)
{
	m_eItemType	= ITEM_ACTIVE;
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;
	m_tOriInfo	= INFO(m_tInfo.fX, m_tInfo.fY,  m_tInfo.fCX, m_tInfo.fCY);

	m_bItemFade	= false;
	m_tFrame	= FRAME(0, 7, 0, 50);
	m_dwItemFadeTime = GetTickCount();
}
int CActive::Progress(void)
{
	if(m_bItemFade)
	{
		if(m_dwItemFadeTime + m_tFrame.dwTime < GetTickCount())
		{
			m_dwItemFadeTime = GetTickCount();
			if(m_tFrame.iStart == 1)
				CDevice::GetInstance()->SoundPlay(ITEMEAT, 0);

			++m_tFrame.iStart;
			if(m_tFrame.iStart >= m_tFrame.iLast)
				return 1;
		}
	}

	return 0;
}
void CActive::Render(HDC hdc)
{
	//Rectangle(hdc,
	//	int(m_tInfo.fX - m_tInfo.fCX / 2.f),
	//	int(m_tInfo.fY - m_tInfo.fCY / 2.f),
	//	int(m_tInfo.fX + m_tInfo.fCX / 2.f),
	//	int(m_tInfo.fY + m_tInfo.fCY / 2.f));

	
	if(!m_bItemFade)
	{
		TransparentBlt(hdc,								//¾ÆÀÌÅÛ
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			int(m_tInfo.fCX), int(m_tInfo.fCX),
			(*m_pBitMap)[m_strKey]->GetMemDC(),
			0,	0,
			32, 32,
			RGB(63, 16, 123));
	}
	else
	{
		TransparentBlt(hdc,								//¸Ô´ÂÀÌÆåÆ®
			int(m_tInfo.fX - m_tInfo.fCX),
			int(m_tInfo.fY - m_tInfo.fCY),
			int(m_tInfo.fCX * 2.f), int(m_tInfo.fCX * 2.f),
			(*m_pBitMap)["ItemEffect"]->GetMemDC(),
			int(64 * m_tFrame.iStart),	
			0,
			64, 64,
			RGB(61, 16, 123));
	}
}
void CActive::Release(void)
{
}