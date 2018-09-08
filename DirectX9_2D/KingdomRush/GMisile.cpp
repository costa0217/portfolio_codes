#include "StdAfx.h"
#include "GMisile.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"
#include "SDevice.h"

CGMisile::CGMisile(void)
:m_pMonster(NULL)
{
}

CGMisile::~CGMisile(void)
{
}
HRESULT	CGMisile::Initialize(void)
{
	m_eProID		= PRO_GMISILE;

	m_wstrObjKey	= L"Bomb3T";
	m_wstrStateKey	= L"Bomb";

	m_fSpeed		= 200.f;

	m_tFrame.fFrame	= 0.f;
	m_fTheta		= 0.f;
	m_fRotTime		= 0.f;
	m_fImgTheta		= 0.f;
	m_iCnt			= 0;		//얼마나 돌았는지
	m_iMonNum		= 0;		//몇번째 몬스터

	m_vOriPos		= m_tInfo.vPos;
	m_bRotate		= false;
	m_bFrist		= false;
	m_bChooseTarget	= false;

	m_bBoom			= false;
	m_bBoomImg		= false;
	m_bFirstBoom	= false;


	m_fBoomRange	= 50.f;
	m_fBombDamage	= 60;

	return S_OK;
}
int		CGMisile::Progress(void)		
{
	if(!m_bFrist && D3DXVec3Length( & (m_tInfo.vPos - m_vOriPos)) > 100 )
	{
		m_bRotate		= true;
		m_bFrist		= true;
		m_tInfo.vDir	= D3DXVECTOR3(-1.f, 0.f, 0.f);
	}
	if(m_bRotate)
	{
		m_fRotTime += GET_TIME;

		if(m_fRotTime > 0.04f)
		{
			m_fTheta = D3DXToRadian(10);
			D3DXMATRIX	matRotZ;
			m_iCnt	+= 10;
			D3DXMatrixRotationZ(&matRotZ, m_fTheta);
			D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vDir, &matRotZ);
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

			m_fImgTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &D3DXVECTOR3(0.f, -1.f, 0.f)));
			if(m_tInfo.vDir.x < 0)
				m_fImgTheta = D3DX_PI * 2 - m_fImgTheta;

			if(m_iCnt == 540)
			{
				CSDevice::GetInstance()->SoundPlay(SD_GMISILE, 0);
				m_bRotate = false;
			}

			m_fRotTime = 0.f;
		}
	}
	if(m_bFrist && !m_bRotate && !m_bBoom)
	{
		if(!m_bChooseTarget)
		{
			m_pMonsterList = CObjMgr::GetInstance()->GetObjList(OBJ_MONSTER);
			int iMonListSize = m_pMonsterList->size();

			if( iMonListSize == 0)
				return 1;
			m_iMonNum = rand()%iMonListSize;

			list<CObj*>::iterator iter = m_pMonsterList->begin();

			for(int i = 0; i < m_iMonNum; ++i)
				++iter;
			m_pMonster		= (*iter);

			m_bChooseTarget = true;
		}
		if(m_pMonster != NULL)
			m_tInfo.vDir = m_pMonster->GetInfo().vPos - m_tInfo.vPos;
		else
			m_tInfo.vDir = m_vPreTargeting - m_tInfo.vPos;

		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		m_fImgTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &D3DXVECTOR3(0.f, -1.f, 0.f)));
		if(m_tInfo.vDir.x < 0)
			m_fImgTheta = D3DX_PI * 2 - m_fImgTheta;
	}
	if( m_pMonster != NULL && ((CMonster*)m_pMonster)->GetMonsterDie() )
	{
		m_vPreTargeting = m_pMonster->GetInfo().vPos;
		m_pMonster = NULL;		
	}
	else
	{
	}
	if(m_pMonster != NULL && D3DXVec3Length( & (m_pMonster->GetInfo().vPos - m_tInfo.vPos)) < 10.f)
	{
		m_bBoom = true;
	}
	if(m_pMonster == NULL && D3DXVec3Length( & (m_vPreTargeting - m_tInfo.vPos)) < 10.f)
	{
		m_bBoom = true;
	}
	if(m_bBoom)	/* 폭탄터질때 */
	{
		if(!m_bBoomImg)
		{
			m_wstrStateKey	= L"Bomb";
			SetFrame(m_wstrStateKey);
			m_bBoomImg	= true;
			m_fImgTheta	= 0.f;

			CSDevice::GetInstance()->SoundPlay(SD_BOMB, 0);
		}
		if(FrameMove())
			return 1;
		else
			return 0;
	}
	else
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME;

	return 0;
}
void	CGMisile::Render(void)		
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fImgTheta);
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
void	CGMisile::Release(void)		
{
	
}
