#include "StdAfx.h"
#include "Slow.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "Monster.h"

CSlow::CSlow(void)
:m_pMonster(NULL)
{
}

CSlow::~CSlow(void)
{
}
HRESULT CSlow::Initialize(void)
{
	m_eEffectID		= EFC_SLOW;
	m_wstrObjKey	= L"UI";
	m_wstrStateKey	= L"Slow";
	SetFrame(m_wstrStateKey);
	return S_OK;
}
int		CSlow::Progress(void)	
{
	if(((CMonster*)m_pMonster)->GetMonsterDie())
		return 1;

	m_tInfo.vPos = m_pMonster->GetInfo().vPos;

	FrameMove(2.f);

	return 0;
}
void	CSlow::Render(void)	
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
void	CSlow::Release(void)	
{
}

