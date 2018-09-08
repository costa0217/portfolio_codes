#include "StdAfx.h"
#include "ActiveBar.h"
#include "Device.h"
#include "TextureMgr.h"

CActiveBar::CActiveBar(void)
{
}

CActiveBar::~CActiveBar(void)
{
	
}
HRESULT CActiveBar::Initialize(void)
{
	m_eUIType		= UI_ACTIVEBAR;
	m_tInfo.vPos	= D3DXVECTOR3(350.f, 572.f, 0.f);
	return S_OK;
}
int CActiveBar::Progress(void)		
{
	return 0;
}
void CActiveBar::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*	pTexture = CTextureMgr::GetInstance()->GetTexture(L"Activebar");
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	m_tInfo.matWorld = matScale * matTrans;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CActiveBar::Release(void)
{
}