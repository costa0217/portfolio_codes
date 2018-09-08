#include "StdAfx.h"
#include "FontEffect.h"

#include "Device.h"
#include "TimeMgr.h"

CFontEffect::CFontEffect(void)
{
}

CFontEffect::~CFontEffect(void)
{
}
HRESULT CFontEffect::Initialize(void)
{
	m_eEffectID		= EFC_FONTEFC;
	m_fTime	= 0.f;
	D3DXMatrixIdentity(&m_matFontTrans);

	m_wstrFont	= L"babo";
	return S_OK;
}
int		CFontEffect::Progress(void)	
{
	D3DXMatrixTranslation(&m_matFontTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	return 0;
}
void	CFontEffect::Render(void)	
{
	//wsprintf(m_wstrFont, L"%s", m_wstrEffect.c_str());
	//CDevice::GetInstance()->GetSprite()->SetTransform(&m_matFontTrans);
	//CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), 
	//	m_wstrFont, lstrlen(wstring), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}
void	CFontEffect::Release(void)	
{
}

