#include "StdAfx.h"
#include "Blood.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

CBlood::CBlood(void)
{
}

CBlood::~CBlood(void)
{
}
HRESULT CBlood::Initialize(void)
{
	m_eEffectID		= EFC_BLLOD;
	m_wstrObjKey	= L"UI";
	m_wstrStateKey	= L"Blood";

	m_fTheta		= float(rand()%361);
	m_fBloodScale	= 1.f + (2.f - rand()%4) / 10.f;

	SetFrame(L"Blood");

	return S_OK;
}
int		CBlood::Progress(void)		
{
	if(FrameMove(1.7f))
		return 1;
	

	return 0;
}
void	CBlood::Render(void)		
{
	D3DXMATRIX		matScale, matTrans, matRotZ;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, m_fBloodScale, m_fBloodScale, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fTheta));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void	CBlood::Release(void)		
{
}

