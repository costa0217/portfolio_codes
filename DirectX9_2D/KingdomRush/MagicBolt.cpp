#include "StdAfx.h"
#include "MagicBolt.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"
#include "Scream.h"

CMagicBolt::CMagicBolt(void)
:m_pMonTarget(NULL)
{
}

CMagicBolt::~CMagicBolt(void)
{
	Release();
}
HRESULT CMagicBolt::Initialize(void)
{
	m_eProID		= PRO_MBOLT;

	m_fSpeed		= 150.f;
	m_fTime			= 0.f;
	m_fTheta		= 0.f;
	m_fBoltScale	= 0.5f;
	m_bHit			= false;

	m_wstrObjKey	= L"Magic4-1T";
	m_wstrStateKey	= L"Magic";

	SetFrame(m_wstrStateKey);

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);
	return S_OK;
}
int CMagicBolt::Progress(void)		
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
		CObj* pZap = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_pMonTarget->GetInfo().vPos.x, m_pMonTarget->GetInfo().vPos.y - 30.f, 0.f));
		((CScream*)pZap)->SetScream(SR_ZAP);
		((CScream*)pZap)->SetRGB(200, 95 , 42);
		m_pEffectList->push_back(pZap);

		((CMonster*)m_pMonTarget)->SetMonsterHP(((CMonster*)m_pMonTarget)->GetMonsterHP() - m_iDamage);
		return 1;
	}

	FrameMove(1.5f);

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
void CMagicBolt::Render(void)		
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	
	m_fBoltScale += GET_TIME;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, m_fBoltScale, m_fBoltScale, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CMagicBolt::Release(void)		
{
}

