#include "StdAfx.h"
#include "Bomb.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"
#include "Scream.h"

#include "SDevice.h"

CBomb::CBomb(void)
:m_pMonster(NULL)
{
}

CBomb::~CBomb(void)
{
	Release();
}
HRESULT CBomb::Initialize(void)
{
	m_eProID		= PRO_BOMB;
	m_fSpeed		= 200;
	m_fTime			= 0.f;
	m_bBoom			= false;
	m_bBoomImg		= false;
	m_bFirstBoom	= false;

	m_wstrStateKey	= L"Bomb";

	if(m_wstrObjKey == L"Bomb1T")
		m_fBoomRange	= 25.f;
	else if(m_wstrObjKey == L"Bomb2T")
		m_fBoomRange	= 27.f;
	else if(m_wstrObjKey == L"Bomb3T")
		m_fBoomRange	= 30.f;
	else if(m_wstrObjKey == L"Bomb4-2T")
		m_fBoomRange	= 32.f;
	
	m_fBoomRange *= 1.5f;

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);


	//m_tInfo.vDir.x = (m_vTargetPos.x - m_tInfo.vPos.x) / 100.f;

	return S_OK;
}
int CBomb::Progress(void)		
{

	D3DXVECTOR3 vTargeting;
	if( m_pMonster == NULL || ((CMonster*)m_pMonster)->GetMonsterDie() || ((CMonster*)m_pMonster)->GetTelpo() )
	{
		m_pMonster	= NULL;
		vTargeting	= m_vPreTargeting;
	}
	else
	{
		vTargeting = m_pMonster->GetInfo().vPos 
			+ ((CMonster*)m_pMonster)->GetInfo().vDir * ((CMonster*)m_pMonster)->GetStats().fSpeed * GET_TIME;
		m_vPreTargeting		= vTargeting;
	}
	

	float fPeak = -1.6f + (6.f  * m_fTime * m_fTime) / 2.f;
	if( fPeak > 0 && m_vTargetPos.y < m_tInfo.vPos.y)
		m_bBoom = true;

	if(m_bBoom)	/* ÆøÅºÅÍÁú¶§ */
	{
		if(!m_bBoomImg)
		{
			m_wstrStateKey	= L"Bomb";
			SetFrame(m_wstrStateKey);
			m_bBoomImg	= true;
			m_fTheta	= 0.f;

			CObj* pkboom = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 20.f, 0.f));
			((CScream*)pkboom)->SetScream(SR_KBOOM);
			m_pEffectList->push_back(pkboom);

			CSDevice::GetInstance()->SoundPlay(SD_BOMB, 0);
		}
		if(FrameMove())
			return 1;

		else
			return 0;
	}
	else
	{
		m_tFrame.fFrame	= 0.f;
		m_fTheta += D3DXToRadian(1.f);
	}

	m_tInfo.vDir = vTargeting - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);


	m_fTime += GET_TIME;

	m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed * GET_TIME;
	m_tInfo.vPos.y += (-1.6f + (6.f * m_fTime * m_fTime) / 2.f) * m_fSpeed * GET_TIME;
	return 0;
}
void CBomb::Render(void)		
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	if(m_bBoom)
		fY = pTexture->tImgInfo.Height / 1.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CBomb::Release(void)		
{
}
