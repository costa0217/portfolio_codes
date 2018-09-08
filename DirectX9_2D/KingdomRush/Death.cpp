#include "StdAfx.h"
#include "Death.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

CDeath::CDeath(void)
{
}

CDeath::~CDeath(void)
{
}
HRESULT CDeath::Initialize(void)
{
	m_eEffectID		= EFC_DEATH;
	m_wstrObjKey	= L"UI";
	m_wstrStateKey	= L"Death";
	SetFrame(m_wstrStateKey);
	return S_OK;
}
int		CDeath::Progress(void)		
{
	if(FrameMove(1.7f))
		return 1;

	return 0;
}
void	CDeath::Render(void)		
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
void	CDeath::Release(void)		
{
}
