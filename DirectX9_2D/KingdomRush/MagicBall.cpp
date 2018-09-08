#include "StdAfx.h"
#include "MagicBall.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"

#include "Scream.h"

CMagicBall::CMagicBall(void)
:m_pMonTarget(NULL)
{
}

CMagicBall::~CMagicBall(void)
{
	Release();
}
HRESULT CMagicBall::Initialize(void)
{
	m_eProID		= PRO_MBALL;

	m_fSpeed		= 200.f;
	m_fTime			= 0.f;
	m_fTheta		= 0.f;
	m_bHit			= false;

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);
	return S_OK;
}
int CMagicBall::Progress(void)		
{
	m_fTime += GET_TIME;
	if(m_fTime > 8.f)
		return 1;

	if( m_pMonTarget != NULL && ((CMonster*)m_pMonTarget)->GetMonsterDie() )
	{
		m_pMonTarget	= NULL;
	}
	if(m_pMonTarget == NULL)
	{
		return 1;
	}
	if(m_bHit)	/*맞았을때 피와 비명, 체력*/
	{
		CObj* pZap = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 20.f, 0.f));
		((CScream*)pZap)->SetScream(SR_ZAP);
		m_pEffectList->push_back(pZap);

		((CMonster*)m_pMonTarget)->SetMonsterHP(((CMonster*)m_pMonTarget)->GetMonsterHP() - m_iDamage);
		return 1;
	}

	D3DXVECTOR3 vTargeting = m_pMonTarget->GetInfo().vPos 
		+ ((CMonster*)m_pMonTarget)->GetInfo().vDir * ((CMonster*)m_pMonTarget)->GetStats().fSpeed * GET_TIME;

	m_tInfo.vDir = vTargeting - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_fTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook));
	if(m_tInfo.vDir.y < m_tInfo.vLook.y)
		m_fTheta = D3DX_PI * 2 - m_fTheta;

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME ;

	return 0;
}
void CMagicBall::Render(void)		
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Projectile", L"Magic1", 0);
	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CMagicBall::Release(void)		
{
}
