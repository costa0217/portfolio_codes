#include "StdAfx.h"
#include "BrokeIce.h"

#include "SDevice.h"

CBrokeIce::CBrokeIce(void)
{
}

CBrokeIce::~CBrokeIce(void)
{
}
HRESULT CBrokeIce::Initialize(void)	
{
	m_eEffectID		= EFC_DEATH;

	m_wstrObjKey	= L"IceAge";
	m_wstrStateKey	= L"Destroy";

	SetFrame(m_wstrStateKey);

	CSDevice::GetInstance()->SoundPlay(SD_BROKEICE, 0);
	return S_OK;
}
int		CBrokeIce::Progress(void)		
{
	if(FrameMove())
		return 1;

	return 0;
}
void	CBrokeIce::Render(void)		
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
void	CBrokeIce::Release(void)		
{
}
