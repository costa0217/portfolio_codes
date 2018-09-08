#include "StdAfx.h"
#include "Teleport.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "Monster.h"
#include "SDevice.h"
CTeleport::CTeleport(void)
:m_pMonster(NULL)
{
}

CTeleport::~CTeleport(void)
{
}
HRESULT CTeleport::Initialize(void)
{
	m_eEffectID		= EFC_TELPO;
	m_wstrObjKey	= L"UI";
	m_wstrStateKey	= L"Teleport";
	SetFrame(m_wstrStateKey);

	CSDevice::GetInstance()->SoundPlay(SD_TELEPORT, 0);


	return S_OK;
}
int		CTeleport::Progress(void)	
{
	if(FrameMove(2.f))
		return 1;

	if(m_pMonster != NULL)
		m_tInfo.vPos = m_pMonster->GetInfo().vPos;

	return 0;
}
void	CTeleport::Render(void)	
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
void	CTeleport::Release(void)	
{
}
