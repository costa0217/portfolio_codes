#include "StdAfx.h"
#include "DeathPing.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"
#include "Death.h"

#define RazorWidth 68
CDeathPing::CDeathPing(void)
:m_pMonster(NULL)
{
}

CDeathPing::~CDeathPing(void)
{
}
HRESULT CDeathPing::Initialize(void)
{
	m_eProID		= PRO_MRAZOR;
	m_wstrObjKey	= L"Projectile";
	m_wstrStateKey	= L"Razor";
	m_fRazorLength	= 0.f;
	m_fTheta		= 0.f;

	m_bDeath		= false;

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	SetFrame(m_wstrStateKey);
	return S_OK;
}
int CDeathPing::Progress(void)		
{
	if(((CMonster*)m_pMonster)->GetMonsterDie())
		return 1;


	if(FrameMove(2.5f))
	{
		((CMonster*)m_pMonster)->SetMonsterDie(true);

		CObj* pDeathEffect = CObjFactory<CDeath>::CreateObj(m_pMonster->GetInfo().vPos);
		m_pEffectList->push_back(pDeathEffect);
		
		return 1;
	}

	m_tInfo.vDir = m_pMonster->GetInfo().vPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_fTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook));
	if(m_tInfo.vDir.y < m_tInfo.vLook.y)
		m_fTheta = (D3DX_PI * 2) - m_fTheta;


	m_fRazorLength = D3DXVec3Length( &( m_pMonster->GetInfo().vPos - m_tInfo.vPos)) / RazorWidth;

	return 0;
}
void CDeathPing::Render(void)		
{
	D3DXMATRIX		matScale, matRotZ, matTrans;


	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, m_fRazorLength, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = /*pTexture->tImgInfo.Width  / 2.f*/0.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CDeathPing::Release(void)		
{
}
