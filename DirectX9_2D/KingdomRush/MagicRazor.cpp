#include "StdAfx.h"
#include "MagicRazor.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"

#include "MagicEffect.h"
#include "Scream.h"

#define RazorWidth 66
#define RazorHeight 10
CMagicRazor::CMagicRazor(void)
:m_pMonster(NULL)
{
}

CMagicRazor::~CMagicRazor(void)
{
}
HRESULT CMagicRazor::Initialize(void)
{
	m_eProID		= PRO_MRAZOR;
	m_wstrObjKey	= L"Magic4-2T";
	m_wstrStateKey	= L"Magic";
	m_fRazorLength	= 0.f;
	m_fTheta		= 0.f;
	m_bZizizic		= false;

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	SetFrame(m_wstrStateKey);
	return S_OK;
}
int CMagicRazor::Progress(void)		
{
	if(((CMonster*)m_pMonster)->GetMonsterDie())
		return 1;

	if(D3DXVec3Length(& (m_pMonster->GetInfo().vPos - m_tInfo.vPos)) > 260)
		return 1;

	if(!m_bZizizic)
	{
		CObj* pMagicEffect = CObjFactory<CMagicEffect>::CreateObj(D3DXVECTOR3(m_pMonster->GetInfo().vPos.x, m_pMonster->GetInfo().vPos.y, 0.f));
		((CMagicEffect*)pMagicEffect)->SetMonster(m_pMonster);
		m_pEffectList->push_back(pMagicEffect);

		CObj* pZap = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_pMonster->GetInfo().vPos.x, m_pMonster->GetInfo().vPos.y - 30.f, 0.f));
		((CScream*)pZap)->SetScream(SR_ZAP);
		((CScream*)pZap)->SetRGB(128, 0 , 128);
		m_pEffectList->push_back(pZap);

		m_bZizizic = true;
	}

	if(FrameMove(2.5f))
	{
		((CMonster*)m_pMonster)->SetMonsterHP(((CMonster*)m_pMonster)->GetMonsterHP() - m_iDamage);
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
void CMagicRazor::Render(void)		
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
void CMagicRazor::Release(void)		
{
}
