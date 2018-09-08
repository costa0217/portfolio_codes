#include "StdAfx.h"
#include "BossSkill.h"

#include "Device.h"
CBossSkill::CBossSkill(void)
:m_iDeleteLeg(0)
{
	Release();
}

CBossSkill::~CBossSkill(void)
{
}

void CBossSkill::Initialize(void)
{
	m_eBulletType	= BUL_BOSSSKILL;
	m_iHP			= 6;		//°ø°Ý·Â

	m_fSpeedX		= 10;		//¼Óµµ
	m_fSpeedY		= 10;
	m_tInfo.fCX		= 160.f;
	m_tInfo.fCY		= 192.f;

	m_tExpFrame		= FRAME(0, 15, 0, 100);
	m_tTargetInfo	= INFO(m_tInfo.fX, m_tInfo.fY, 50.f, 50.f);
	m_tLegInfo		= INFO(m_tInfo.fX, m_tInfo.fY - 2000.f, m_tInfo.fCX, m_tInfo.fCY);

	m_dwExpTime		= GetTickCount();

	m_bExplosion	= false;

}
int CBossSkill::Progress(void)
{
	if(m_bExplosion)
	{
		if(m_dwExpTime + m_tExpFrame.dwTime < GetTickCount())
		{
			m_dwExpTime = GetTickCount();
			if(m_tExpFrame.iStart == 0)
				CDevice::GetInstance()->SoundPlay(BOOM, 0);

			++m_tExpFrame.iStart;
			if( m_tExpFrame.iStart > m_tExpFrame.iLast)
				return 1;
		}
	}
	if(m_tLegInfo.fY + 100.f < m_tTargetInfo.fY)
	{
		m_fSpeedY += 2.f;
		m_tLegInfo.fY += m_fSpeedY;
	}
	else
	{
		m_bExplosion = true;
		/*++m_iDeleteLeg;
		if(m_iDeleteLeg > 50)
			return 1;*/
	}

	return 0;
}
void CBossSkill::Render(HDC hdc)
{
	/*Rectangle(hdc,
		int(m_tInfo.fX - m_tTargetInfo.fCX),
		int(m_tInfo.fY - m_tTargetInfo.fCY / 2.f),
		int(m_tInfo.fX + m_tTargetInfo.fCX / 1.4f),
		int(m_tInfo.fY + m_tTargetInfo.fCY / 2.f));*/

	TransparentBlt(hdc,								//Å¸°Ù
		int(m_tTargetInfo.fX - m_tTargetInfo.fCX / 2.f),
		int(m_tTargetInfo.fY - m_tTargetInfo.fCY / 2.f),
		int(m_tTargetInfo.fCX), int(m_tTargetInfo.fCY),
		(*m_pBitMap)["target"]->GetMemDC(),
		0,	0,
		32, 32,
		RGB(61, 16, 123));

	TransparentBlt(hdc,								//ÃÑ¾Ë
		int(m_tLegInfo.fX - m_tLegInfo.fCX / 2.f),
		int(m_tLegInfo.fY - m_tLegInfo.fCY / 2.f),
		int(m_tLegInfo.fCX), int(m_tLegInfo.fCY),
		(*m_pBitMap)["bossSK"]->GetMemDC(),
		0,	0,
		int(m_tLegInfo.fCX), int(m_tLegInfo.fCY),
		RGB(61, 16, 123));

	if(m_bExplosion)
	{
		TransparentBlt(hdc,						//ÆøÆÈÀÌÆåÆ®
			int(m_tInfo.fX - 90.f),
			int(m_tInfo.fY - 150.f),
			200, 200,
			(*m_pBitMap)["explosion"]->GetMemDC(),
			int(96 * m_tExpFrame.iStart),			//ÀÌ¹ÌÁöÅ©±â 96x96
			int(96 * m_tExpFrame.iScene),
			96, 96,
			RGB(61, 16, 123));
	}
}
void CBossSkill::Release(void)
{
}
RECT CBossSkill::GetCOlRect()
{
	if(m_bExplosion)
	{
		RECT rc = {
			int(m_tInfo.fX - m_tTargetInfo.fCX),
			int(m_tInfo.fY - m_tTargetInfo.fCY / 2.f),
			int(m_tInfo.fX + m_tTargetInfo.fCX / 1.4f),
			int(m_tInfo.fY + m_tTargetInfo.fCY / 2.f)};
	
		return rc;
	}
	else
	{
		RECT rc = {
			int(m_tInfo.fX),
			int(m_tInfo.fY),
			int(m_tInfo.fX),
			int(m_tInfo.fY)};
	
		return rc;
	}
}