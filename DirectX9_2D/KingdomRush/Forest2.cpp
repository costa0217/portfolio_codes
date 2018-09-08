#include "StdAfx.h"
#include "Forest2.h"

#include "Device.h"
#include "TextureMgr.h"

CForest2::CForest2(void)
{
}

CForest2::~CForest2(void)
{
}
HRESULT CForest2::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(355.f, 66.f, 0.f);
	return S_OK;
}
int		CForest2::Progress(void)
{
	return 0;
}
void	CForest2::Render(void)	
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Forest2");
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void	CForest2::Release(void)	
{
}

