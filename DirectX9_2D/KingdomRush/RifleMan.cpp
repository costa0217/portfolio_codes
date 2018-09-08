#include "StdAfx.h"
#include "RifleMan.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "Arrow.h"
#include "Bullet.h"

#include "Tower.h"
#include "SDevice.h"

CRifleMan::CRifleMan(void)
:m_pMonster(NULL)
,m_pTower(NULL)
{
}

CRifleMan::~CRifleMan(void)
{
	Release();
}
HRESULT CRifleMan::Initialize(void)	
{
	m_wstrObjKey	= L"Archer1T";
	m_wstrStateKey	= L"D";

	m_eSoldierID	= SOL_RIFLEMAN;
	m_bDie			= false;
	m_bLockOn		= false;

	m_tInfo.vDir	= D3DXVECTOR3(1.f, 0.f, 0.f);

	m_iLevel		= 0;
	m_bLevelup		= false;
	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	SetFrame(m_wstrStateKey);		//기본 모션
	
	m_bStateUp		= false;
	m_bStateDown	= false;

	m_bFirstAtk		= false;
	m_bFire			= false;
	m_bFireInterval	= false;

	//스킬
	{
		m_bPoison	= false;
		m_bCritical	= false;
		m_bSlow		= false;
	}

	m_fFireTime		= 0.f;

	for(int i = 0; i < 2; ++i)
		m_bSkill[i] = false;

	m_pBulletList	= CObjMgr::GetInstance()->GetObjList(OBJ_PROJECTILE);

	return S_OK;
}
int		CRifleMan::Progress(void)		
{
	if(m_bDie)
		return 1;

	if(m_bLevelup)
		Leveling();

	if(m_bLockOn)	//락온 된 대상이있으면
	{
		if(!m_bFireInterval)
			FrameMove(1.5f);
		if(m_bLockOn && m_tFrame.fFrame == 0)
		{
			m_bFire			= false;
			m_bFireInterval	= true;
		}

		if(!m_bFireInterval)
		{
			D3DXVECTOR3 vMonster = m_pMonster->GetInfo().vPos;
			m_tInfo.vDir = vMonster - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

			if(!m_bFirstAtk)
			{
				if(m_iIndex == 1)
					m_tFrame.fFrame = 2;
				m_bFirstAtk = true;
			}
			SetDirImage(vMonster);

			if( int(m_tFrame.fFrame) == 4 && !m_bFire)
			{
				if( m_wstrObjKey != L"Archer4-2T" )
				{
					D3DXVECTOR3 vXdir = D3DXVECTOR3(0.f, -1.f, 0.f);

					CObj* pArrow = new CArrow;
					pArrow->SetPos(m_tInfo.vPos);
					pArrow->SetDir(vXdir);
					((CArrow*)pArrow)->SetMonTarget(m_pMonster);
					((CArrow*)pArrow)->SetDamage(m_iDamage);
				
					pArrow->Initialize();
					if(m_bPoison)
						((CArrow*)pArrow)->SetPoisonArrow(true);

					m_pBulletList->push_back(pArrow);
					
					//if(!CSDevice::GetInstance()->SoundPlaying(SD_ARROWFIRE))
						CSDevice::GetInstance()->SoundPlay(SD_ARROWFIRE, 0);
					
				}
				else
				{
					CObj* pBullet = new CBullet;
					pBullet->SetPos(m_tInfo.vPos);
					((CBullet*)pBullet)->SetMonTarget(m_pMonster);
					((CBullet*)pBullet)->SetDamage(m_iDamage);
					if(m_bCritical)
						((CBullet*)pBullet)->SetDamage(m_iDamage + 30);
					
					pBullet->Initialize();
					if(m_bSlow)
						((CBullet*)pBullet)->SetSlow(true);

					m_pBulletList->push_back(pBullet);

					CSDevice::GetInstance()->SoundPlay(SD_SNIPER, 0);

				}
				m_bFire = true;
			}
		}
		else
		{
			m_fFireTime	+= GET_TIME;
			if( m_fFireTime > m_fAttackSpd)		// 1.f 변수로 둬서 공속으로 사용
			{
				m_fFireTime		= 0.f;
				m_bFireInterval	= false;
			}
		}
	}
	else
	{
		m_tFrame.fFrame = 0.f;
		m_bFirstAtk		= false;
		m_bStateUp		= false;
		m_bStateDown	= false;
		m_fFireTime		= 0.f;
		m_bFireInterval	= false;
	}

	/* 1번스킬 활성화 */
	if(m_bSkill[0])
	{
		if(m_wstrObjKey == L"Archer4-1T")
		{
			m_bPoison = true;	// 독
		}
		else if(m_wstrObjKey == L"Archer4-2T")
		{
			// 크리티컬
			m_bCritical	= true;
		}
	}
	/* 2번스킬 활성화 */
	if(m_bSkill[1])
	{
		if(m_wstrObjKey == L"Archer4-1T")
		{
			// 인탱글
		}
		else if(m_wstrObjKey == L"Archer4-2T")
		{
			m_bSlow = true;
		}
	}

	return 0;
}
void	CRifleMan::Render(void)		
{
	
	D3DXMATRIX		matScale, matTrans;

	if( !((CTower*)m_pTower)->GetFreezing() )
	{
		float		fScale = 0.9f;
		m_tInfo.vDir.x < 0 ? fScale *= -1 : fScale *= 1;

		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
		D3DXMatrixScaling(&matScale, fScale, 0.9f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = pTexture->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	}

}
void	CRifleMan::Release(void)		
{
}
void	CRifleMan::Leveling()
{
	if(m_bLvl[LVL_1])
	{
		m_wstrObjKey	= L"Archer1T";
		m_iLevel		= 0;
	}
	else if(m_bLvl[LVL_2])
	{
		m_wstrObjKey	= L"Archer2T";
		m_tInfo.vPos.y	-= 2.f;
		m_iLevel		= 1;
	}
	else if(m_bLvl[LVL_3])
	{
		m_wstrObjKey	= L"Archer3T";
		m_tInfo.vPos.y	-= 3.f;
		m_iLevel		= 2;
	}
	else if(m_bLvl[LVL_41])
	{
		m_wstrObjKey	= L"Archer4-1T";
		m_tInfo.vPos.y	-= 3.f;
		m_iLevel		= 3;
	}
	else if(m_bLvl[LVL_42])
	{
		m_wstrObjKey	= L"Archer4-2T";
		m_tInfo.vPos.y	+= 5.f;
		m_iLevel		= 4;
	}

	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bLevelup = false;
}
void CRifleMan::SetDirImage(D3DXVECTOR3 _vMonster)
{
	if(_vMonster.y < m_tInfo.vPos.y)
	{
		if(!m_bStateUp)
		{
			m_wstrStateKey	= L"U";
			SetFrame(m_wstrStateKey);
			m_bStateUp		= true;
			m_bStateDown	= false;
			if(m_iIndex == 1)
				m_tFrame.fFrame = 2;
		}
	}
	else
	{
		if(!m_bStateDown)
		{
			m_wstrStateKey	= L"D";
			SetFrame(m_wstrStateKey);
			m_bStateUp		= false;
			m_bStateDown	= true;
			if(m_iIndex == 1)
				m_tFrame.fFrame = 2;
		}
	}
}