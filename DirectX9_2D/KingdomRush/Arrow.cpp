#include "StdAfx.h"
#include "Arrow.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Blood.h"
#include "Monster.h"

#include "Scream.h"
#include "SDevice.h"

CArrow::CArrow(void)
:m_pMonTarget(NULL)
{
}
CArrow::~CArrow(void)
{
}
HRESULT CArrow::Initialize(void)
{
	m_eProID		= PRO_ARROW;

	m_fSpeed		= 150;
	m_tInfo.vLook	= D3DXVECTOR3(1.f, 0.f, 0.f);
	
	m_fTheta		= 0.f;
	m_fTime			= 0.f;

	m_bHit			= false;
	m_bGuided		= false;
	m_bFormula		= false;

	m_vArchorPos	= D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f );
	m_vMonInitPos	= m_pMonTarget->GetInfo().vPos;
	m_vPreArrPos	= D3DXVECTOR3(0.f, 0.f, 0.f);

	m_bPoisonArr	= false;

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	return S_OK;
}
int CArrow::Progress(void)		
{
	m_fTime += GET_TIME;
	if(m_fTime > 8.f)
		return 1;

	if( m_pMonTarget != NULL && ((CMonster*)m_pMonTarget)->GetTelpo() )
		return 1;


	if( m_pMonTarget != NULL && ((CMonster*)m_pMonTarget)->GetMonsterDie() )
	{
		m_pMonTarget	= NULL;
	}
	if(m_pMonTarget == NULL)
	{
		if( D3DXVec3Length( & (m_vMonInitPos - m_tInfo.vPos)) > 2.f)
		{
			m_tInfo.vDir = m_vMonInitPos - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

			m_fTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook));
			if(m_tInfo.vDir.y < m_tInfo.vLook.y)
				m_fTheta = D3DX_PI * 2 - m_fTheta;

			m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME ;
		}
		return 0;
	}
	
	if(m_bHit)	/*맞았을때 피와 비명, 체력*/
	{
		CObj* pSok = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_pMonTarget->GetInfo().vPos.x, m_pMonTarget->GetInfo().vPos.y - 20.f, 0.f));
		((CScream*)pSok)->SetScream(SR_SOK);
		m_pEffectList->push_back(pSok);

		CObj*	pBlood = CObjFactory<CBlood>::CreateObj(m_pMonTarget->GetInfo().vPos);
		m_pEffectList->push_back(pBlood);

		((CMonster*)m_pMonTarget)->SetMonsterHP(((CMonster*)m_pMonTarget)->GetMonsterHP() - m_iDamage);
		if(m_bPoisonArr)
			((CMonster*)m_pMonTarget)->SetPoisoning(true);

		int iRandSound = rand()%2;
		if(iRandSound == 0)
			CSDevice::GetInstance()->SoundPlay(SD_ARROWHIT1, 0);
		else
			CSDevice::GetInstance()->SoundPlay(SD_ARROWHIT2, 0);
		
		return 1;
	}

	m_vMonInitPos	= m_pMonTarget->GetInfo().vPos;

	float fArrowSpeed = (m_pMonTarget->GetInfo().vPos.x - m_vArchorPos.x) * GET_TIME;

	m_tInfo.vPos.x += fArrowSpeed;

	
	if( abs(m_vMonInitPos.x - m_vArchorPos.x) < 25.f || m_bFormula)
		/*&& !m_bGuided*/	/* 아래 있을때 유도탄 */
	{
		/* 유도 화살 */
		D3DXVECTOR3 vTargeting = m_pMonTarget->GetInfo().vPos 
			+ ((CMonster*)m_pMonTarget)->GetInfo().vDir * ((CMonster*)m_pMonTarget)->GetStats().fSpeed * GET_TIME;

		m_tInfo.vDir = vTargeting - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		m_fTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook));
		if(m_tInfo.vDir.y < m_tInfo.vLook.y)
			m_fTheta = D3DX_PI * 2 - m_fTheta;

		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME ;

		m_bFormula = true;
	}


	if( /*m_bGuided &&*/ !m_bFormula )
	{
		/* 포물선 공식 */
		float fX3 = (m_vMonInitPos.x + m_vArchorPos.x) / 2.f;
		float fY3 = 0.f;
		m_vMonInitPos.y < m_vArchorPos.y ? fY3 = m_vMonInitPos.y - 40.f : fY3 = m_vArchorPos.y - 40.f;

		float a = 0.f;
		float b = 0.f;
		float c = 0.f;

		a = (m_vMonInitPos.y - fY3) / ((m_vMonInitPos.x - fX3) * (fX3 - m_vArchorPos.x)) 
			- ( m_vMonInitPos.y - m_vArchorPos.y) / ((m_vMonInitPos.x - m_vArchorPos.x) * (fX3 - m_vArchorPos.x));
		b = ((m_vMonInitPos.y - m_vArchorPos.y) / (m_vMonInitPos.x - m_vArchorPos.x)) - a * (m_vMonInitPos.x + m_vArchorPos.x);
		c = m_vMonInitPos.y - (a * pow(m_vMonInitPos.x, 2)) - b * m_vMonInitPos.x;
		m_tInfo.vPos.y	= a * pow(m_tInfo.vPos.x, 2) + b * m_tInfo.vPos.x + c;
	}

	m_tInfo.vDir = m_tInfo.vPos - m_vPreArrPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_fTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook));
	if(m_tInfo.vDir.y < m_tInfo.vLook.y)
		m_fTheta = D3DX_PI * 2 - m_fTheta;

	m_vPreArrPos	= m_tInfo.vPos;
 
	return 0;
}
void CArrow::Render(void)
{
	RECT rc = {0, 0, 28, 12};
	if(m_pMonTarget == NULL)
		rc.right  = 18;
	
	int iRedBlue = 255;
	if(m_bPoisonArr)
		iRedBlue = 100;
	
	D3DXMATRIX		matScale, matRotZ, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Projectile", L"Arrow", 0);
	D3DXMatrixScaling(&matScale, 0.9f, 0.9f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, &rc, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, iRedBlue, 255, iRedBlue));
}
void CArrow::Release(void)		


{
}
float	CArrow::ParabolaFormula(D3DXVECTOR3 _vMon, D3DXVECTOR3 _vAcp)
{
	float fResult = 0.f;

	return fResult;
}


