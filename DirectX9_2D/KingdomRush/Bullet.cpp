#include "StdAfx.h"
#include "Bullet.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"

#include "Blood.h"
#include "Scream.h"
#include "Slow.h"

CBullet::CBullet(void)
:m_pMonTarget(NULL)
{
}

CBullet::~CBullet(void)
{
	Release();
}

HRESULT CBullet::Initialize(void)
{
	m_eProID		= PRO_BULLET;

	m_fSpeed		= 500.f;
	m_fTime			= 0.f;
	m_bHit			= false;
	m_bSlow			= false;

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);
	return S_OK;
}
int CBullet::Progress(void)		
{
	m_fTime += GET_TIME;
	if(m_fTime > 8.f)
		return 1;
	
	if( m_pMonTarget != NULL && (((CMonster*)m_pMonTarget)->GetMonsterDie() || ((CMonster*)m_pMonTarget)->GetTelpo()))
	{
		m_pMonTarget	= NULL;
	}
	if(m_pMonTarget == NULL)
	{
		return 1;
	}
	if(m_bHit)	/*맞았을때 피와 비명, 체력*/
	{
		CObj*	pBlood = CObjFactory<CBlood>::CreateObj(m_pMonTarget->GetInfo().vPos);
		m_pEffectList->push_back(pBlood);

		if(m_iDamage == 95)
		{
			//크리
			CObj* pCrit = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_pMonTarget->GetInfo().vPos.x, m_pMonTarget->GetInfo().vPos.y - 20.f, 0.f));
			((CScream*)pCrit)->SetScream(SR_CRIT);
			m_pEffectList->push_back(pCrit);
		}
		else
		{
			CObj* pSok = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_pMonTarget->GetInfo().vPos.x, m_pMonTarget->GetInfo().vPos.y - 20.f, 0.f));
			((CScream*)pSok)->SetScream(SR_SOK);
			((CScream*)pSok)->SetRGB(255, 50, 50);
			m_pEffectList->push_back(pSok);
		}

		if(m_bSlow && !((CMonster*)m_pMonTarget)->GetSlow())
		{
			((CMonster*)m_pMonTarget)->SetSpeed(((CMonster*)m_pMonTarget)->GetSpeed() / 2.f);
			((CMonster*)m_pMonTarget)->SetSlow(true);

			CObj* pSlow = CObjFactory<CSlow>::CreateObj(m_pMonTarget->GetInfo().vPos);
			((CSlow*)pSlow)->SetMonster(m_pMonTarget);
			m_pEffectList->push_back(pSlow);
		}

		((CMonster*)m_pMonTarget)->SetMonsterHP(((CMonster*)m_pMonTarget)->GetMonsterHP() - m_iDamage);
		return 1;
	}

	D3DXVECTOR3 vTargeting = m_pMonTarget->GetInfo().vPos 
		+ ((CMonster*)m_pMonTarget)->GetInfo().vDir * ((CMonster*)m_pMonTarget)->GetStats().fSpeed * GET_TIME;

	m_tInfo.vDir = vTargeting - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME ;

	return 0;
}
void CBullet::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Projectile", L"Arrow", 3);
	D3DXMatrixScaling(&matScale, 0.9f, 0.9f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CBullet::Release(void)		
{
}
