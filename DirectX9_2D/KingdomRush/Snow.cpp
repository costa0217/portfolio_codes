#include "StdAfx.h"
#include "Snow.h"

#include "Device.h"
#include "TextureMgr.h"


CSnow::CSnow(void)
{
}

CSnow::~CSnow(void)
{
}
HRESULT CSnow::Initialize(void)	
{
	m_tInfo.vPos = D3DXVECTOR3(382.f, -4.f, 0.f);
	return S_OK;
}
int		CSnow::Progress(void)		
{
	return 0;
}
void	CSnow::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Snow");
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	RECT rc = { LONG(fX / 1.3f), 0, 386, 372};

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, &rc, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void	CSnow::Release(void)		
{
}

