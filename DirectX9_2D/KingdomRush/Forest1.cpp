#include "StdAfx.h"
#include "Forest1.h"

#include "Device.h"
#include "TextureMgr.h"

CForest1::CForest1(void)
{
}

CForest1::~CForest1(void)
{
}
HRESULT CForest1::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(286.f, 465.f, 0.f);
	return S_OK;
}
int		CForest1::Progress(void)
{
	return 0;
}
void	CForest1::Render(void)	
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Forest1");
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void	CForest1::Release(void)	
{
}

