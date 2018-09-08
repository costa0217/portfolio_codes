#include "StdAfx.h"
#include "Smoke.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "SDevice.h"


CSmoke::CSmoke(void)
{
}

CSmoke::~CSmoke(void)
{
}
HRESULT CSmoke::Initialize(void)	
{
	m_eEffectID		= EFC_SMOKE;
	m_wstrObjKey	= L"BTower";
	m_wstrStateKey	= L"Boom";

	SetFrame(m_wstrStateKey);

	CSDevice::GetInstance()->SoundPlay(SD_SMOKE, 0);

	return S_OK;
}
int		CSmoke::Progress(void)
{
	if(FrameMove(1.7f))
		return 1;

	return 0;
}
void	CSmoke::Render(void)
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

}
void	CSmoke::Release(void)
{
}
