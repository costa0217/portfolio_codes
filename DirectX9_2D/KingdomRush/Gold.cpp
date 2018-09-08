#include "StdAfx.h"
#include "Gold.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"


CGold::CGold(void)
{
}

CGold::~CGold(void)
{
	Release();
}
HRESULT CGold::Initialize(void)
{
	m_eEffectID		= EFC_GOLD;
	m_fStayTime		= 0.f;
	m_fAlphaTime	= 0.f;
	m_tInfo.vDir	= D3DXVECTOR3(0.f, -1.f, 0.f);
	m_iAlpha		= 255;

	return S_OK;
}
int		CGold::Progress(void)	
{
	m_fStayTime		+= GET_TIME;
	m_fAlphaTime	+= GET_TIME;
	
	if(m_fAlphaTime > 0.01f)
	{
		m_iAlpha -= 2;
		m_fAlphaTime	= 0.f;
	}
	
	m_tInfo.vPos.y += m_tInfo.vDir.y * 10 * GET_TIME;
	if(m_fStayTime > 1.f)
		return 1;
	return 0;
}
void	CGold::Render(void)	
{
	D3DXMATRIX	 matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"PGold");
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 13, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	/* µ· ¾Õ¼ýÀÚ */
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"GoldNumber");
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 16.f;

	RECT rc = {0, (m_iGold / 10) * 24, 16, (m_iGold / 10 + 1) * 24};

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, &rc, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	/* µ· µÞ¼ýÀÚ */
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"GoldNumber");
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 6, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 16.f;

	RECT rc2 = {0, (m_iGold % 10) * 24, 16, (m_iGold % 10 + 1) * 24};

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, &rc2, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}
void	CGold::Release(void)	
{
}
