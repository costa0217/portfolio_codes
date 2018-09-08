#include "StdAfx.h"
#include "ElecShock.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "Monster.h"

CElecShock::CElecShock(void)
:m_pMonster(NULL)
{
}

CElecShock::~CElecShock(void)
{
}
HRESULT CElecShock::Initialize(void)	
{
	m_eEffectID		= EFC_ELECSHK;
	m_wstrObjKey	= L"Bomb4-1T";
	m_wstrStateKey	= L"Effect";
	SetFrame(m_wstrStateKey);

	return S_OK;
}
int		CElecShock::Progress(void)		
{
	if(((CMonster*)m_pMonster)->GetMonsterDie())
		return 1;

	m_tInfo.vPos = m_pMonster->GetInfo().vPos;

	if(FrameMove(2.f))
		return 1;

	return 0;
}
void	CElecShock::Render(void)		
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
void	CElecShock::Release(void)		
{
}
