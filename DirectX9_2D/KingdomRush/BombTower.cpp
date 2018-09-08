#include "StdAfx.h"
#include "BombTower.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "ObjMgr.h"

#include "Bomb.h"
#include "GMisile.h"
#include "ElecRazor.h"
#include "Monster.h"

#include "Smoke.h"
#include "ObjFactory.h"

#include "Freezing.h"
#include "Click.h"
#include "SDevice.h"

CBombTower::CBombTower(void)
:m_bBombLockOn(false)
,m_pMonster(NULL)
{
}

CBombTower::~CBombTower(void)
{
	Release();
}
HRESULT CBombTower::Initialize(void)
{
	m_eTowerID		= TWR_BOMB1;
	m_bWark			= true;		// 공사중
	m_tWarkFrame	= SHEETFRM(0, 10, 0, 70);
	m_tTowerStat	= TWRSTATS(7, 14, 50, 280.f, 220);
	m_dwWarkTime	= GetTickCount();

	m_wstrObjKey	= L"Bomb1T";
	m_wstrStateKey	= L"Tower";

	m_bTowerClick	= false;	// 클릭이 됬는지

	m_iLevel		= 0;
	m_bLevelup		= false;
	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bDestroyTower = false;

	SetFrame(L"Tower");		//기본 모션

	m_bFireInterval	= false;
	m_bFire			= false;
	m_fFireTime		= 0.f;

	m_pBulletList	= CObjMgr::GetInstance()->GetObjList(OBJ_PROJECTILE);
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	for(int i = 0; i < 2; ++i)
		m_bSkill[i] = false;

	{ //스킬
		m_bGuidedMisile	= false;
		m_fGMisileTime	= 0.f;
		m_iGMisileCnt	= 0;

		m_bChainLight	= false;
	}

	m_bFreezing		= false;
	m_bFreezImg		= false;
	m_pSkillList	= CObjMgr::GetInstance()->GetObjList(OBJ_SKILL);

	m_iFreezAtkCnt	= 0;

	return S_OK;
}
int CBombTower::Progress(void)		
{
	if(m_bDestroyTower)
		return 1;

	if(m_bFreezing)
	{
		m_bBombLockOn	= false;
		m_pMonster		= NULL;
		m_tFrame.fFrame = 0.f;

		if(!m_bFreezImg)
		{
			CObj*	pFreezing	= CObjFactory<CFreezing>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 1.f, 0.f));
			((CFreezing*)pFreezing)->SetTower(this);
			m_pSkillList->push_back(pFreezing);

			CObj*	pClick		= CObjFactory<CClick>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 2.f, 0.f));
			((CClick*)pClick)->SetTower(this);
			m_pSkillList->push_back(pClick);

			m_bFreezImg = true;
		}
		if(m_iFreezAtkCnt == 4)
		{
			m_bFreezImg		= false;
			m_bFreezing		= false;
			m_iFreezAtkCnt	= 0;
		}

		return 0;
	}

	if(m_bLevelup)
	{
		Leveling();

		if(m_iLevel == 1)
		{
			CSDevice::GetInstance()->SoundPlay(SD_BTOWER_TAUNT1, 0);
		}
		else if(m_iLevel == 2)
		{
			CSDevice::GetInstance()->SoundPlay(SD_BTOWER_TAUNT2, 0);
		}
		else if(m_iLevel == 3)
		{
			CSDevice::GetInstance()->SoundPlay(SD_BFG_READY, 0);
		}
		else if(m_iLevel == 4)
		{
			CSDevice::GetInstance()->SoundPlay(SD_TESL_READY, 0);
		}
	}

	if(m_bSkill[0])
	{
		if( m_wstrObjKey == L"Bomb4-1T" )
		{
			m_bChainLight	= true;
		}
		else if( m_wstrObjKey == L"Bomb4-2T")
		{
			m_bGuidedMisile	= true;
			
		}
	}
	if(m_bWark)
	{
		if( m_dwWarkTime + m_tWarkFrame.dwTime < GetTickCount())
		{
			m_dwWarkTime = GetTickCount();
			++m_tWarkFrame.iStart;
			if(m_tWarkFrame.iStart >= m_tWarkFrame.iLast)
			{
				CSDevice::GetInstance()->SoundPlay(SD_BTOWER_READY, 0);
				m_tWarkFrame.iStart = 0;
				m_bWark = false;
			}
		}
	}
	if(m_bBombLockOn)
	{
		if(m_bWark)
			return 0;
		if(!m_bFireInterval)
			FrameMove();	// 프레임속도 조정
		if(m_tFrame.fFrame == 0)
		{
			m_bFire			= false;
			m_bFireInterval	= true;
		}
		if(!m_bFireInterval)
		{
			int iBoomNum = BoomNumbering();
		
			if( int(m_tFrame.fFrame) == iBoomNum && !m_bFire)
			{
				if( m_wstrObjKey != L"Bomb4-1T" )
				{
					CObj* pSmoke = CObjFactory<CSmoke>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x + 5.f, m_tInfo.vPos.y - 25.f, 0.f));
					m_pEffectList->push_back(pSmoke);

					CObj* pBomb = new CBomb;
					pBomb->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 20.f, 0.f));
					pBomb->SetDir(D3DXVECTOR3(0.f, -1.f, 0.f));
					((CBomb*)pBomb)->SetMonTarget(m_pMonster);
					((CBomb*)pBomb)->SetTargetPos(m_pMonster->GetInfo().vPos);
					((CBomb*)pBomb)->SetBombObjKey(m_wstrObjKey);
					((CBomb*)pBomb)->SetBombDamage(m_tTowerStat.iMinDam);
					pBomb->Initialize();
					
					m_pBulletList->push_back(pBomb);
					if(m_bGuidedMisile)
						++m_iGMisileCnt;
					if(m_wstrObjKey == L"Bomb4-2T" && m_bGuidedMisile
						&& m_iGMisileCnt > 1)
					{
						CObj*	pGMisile = CObjFactory<CGMisile>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 20.f, 0.f));
						pGMisile->SetDir(D3DXVECTOR3(0.f, -1.f, 0.f));
						m_pBulletList->push_back(pGMisile);

						m_iGMisileCnt	= 0;
					}
				}
				else if(m_wstrObjKey == L"Bomb4-1T")
				{
					int iChainNum = 2;
					if(m_bChainLight)
					{
						iChainNum = 5;
					}
					CObj* pRazor = new CElecRazor;
					((CElecRazor*)pRazor)->SetMonTarget(m_pMonster);
					((CElecRazor*)pRazor)->SetChainNum(iChainNum);
					((CElecRazor*)pRazor)->SetBombDamage(m_tTowerStat.iMinDam);
					pRazor->Initialize();
					pRazor->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 25.f, 0.f));
					//pBomb->SetDir(D3DXVECTOR3(0.f, -1.f, 0.f));

					m_pBulletList->push_back(pRazor);
				}				
				m_bFire = true;		
			}
		}
		else
		{
			m_fFireTime	+= GET_TIME;
			if( m_fFireTime > 2.f )		// 1.f 변수로 둬서 공속으로 사용
			{
				m_fFireTime		= 0.f;
				m_bFireInterval	= false;
			}
		}
	}
	else
	{
		m_tFrame.fFrame = 0.f;
		m_fFireTime		= 0.f;
		m_bFireInterval	= false;
	}

	/* 몬스터가 죽었을때 */
	if( m_pMonster != NULL )
	{
		if(((CMonster*)m_pMonster)->GetMonsterDie())
		{
			m_bBombLockOn	= false;
			m_pMonster		= NULL;
		}
		else
		{
			/*거리에서 벗어나면*/
			float fDisX	 = m_tInfo.vPos.x - ((CMonster*)m_pMonster)->GetInfo().vPos.x;
			float fDisY	 = m_tInfo.vPos.y - ((CMonster*)m_pMonster)->GetInfo().vPos.y;
			float fRange = m_tTowerStat.iRange / 2.f;

			if( (pow(fDisX, 2) / pow(fRange, 2)) + (pow(fDisY, 2) / pow(fRange/ 1.3f , 2)) >= 1.f )
			{
				m_bBombLockOn	= false;
				m_pMonster		= NULL;
				m_tFrame.fFrame = 0.f;
			}
		}
	}


	return 0;
}
void CBombTower::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"stage1G");
	
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	
	D3DXMatrixTranslation(&matTrans, float((int)m_tInfo.vPos.x), float((int)m_tInfo.vPos.y), 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	/* 건설 */
	if(m_bWark)
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", 19);
		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = pTexture->tImgInfo.Height / 1.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", m_tWarkFrame.iStart);
		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = pTexture->tImgInfo.Height / 1.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(200,255,255,255));
	}
	/* 타워 */	
	else
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);

		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = pTexture->tImgInfo.Height / 1.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

		/* 공격 범위 */
		if(m_bTowerClick)
		{
			pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", 12);

			fX = pTexture->tImgInfo.Width  / 2.f;
			fY = pTexture->tImgInfo.Height / 2.f;
			float fRangeScale = m_tTowerStat.iRange / pTexture->tImgInfo.Width;
			D3DXMatrixScaling(&matRangeScale, fRangeScale, fRangeScale/1.3f, 0.f);

			CDevice::GetInstance()->GetSprite()->SetTransform(&(matRangeScale * matTrans));
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
				&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(200, 255, 255, 255));
		}
	}
}
void CBombTower::Release(void)		
{
}
void CBombTower::Leveling()
{
	if(m_bLvl[LVL_1])
	{
		m_wstrObjKey	= L"Bomb1T";
		m_iLevel		= 0;
		m_tTowerStat	= TWRSTATS(7, 14, 50, 280.f, 220);
	}
	else if(m_bLvl[LVL_2])
	{
		m_wstrObjKey	= L"Bomb2T";
		m_iLevel		= 1;
		m_tTowerStat	= TWRSTATS(15, 30, 50, 290.f, 320);
	}
	else if(m_bLvl[LVL_3])
	{
		m_wstrObjKey	= L"Bomb3T";
		m_iLevel		= 2;
		m_tTowerStat	= TWRSTATS(30, 60, 50, 300.f, 320);
	}
	else if(m_bLvl[LVL_41])
	{
		m_wstrObjKey	= L"Bomb4-2T";
		m_iLevel		= 3;
		m_tTowerStat	= TWRSTATS(60, 110, 50, 300.f, 220);
	}
	else if(m_bLvl[LVL_42])
	{
		m_wstrObjKey	= L"Bomb4-1T";
		m_iLevel		= 4;
		m_tTowerStat	= TWRSTATS(50, 100, 50, 300.f, 220);
	}

	SetFrame(L"Tower");		//기본 모션
	m_tFrame.fFrame = 0.f;

	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bLevelup = false;
}
int CBombTower::BoomNumbering()
{
	int iBoomNum = -1;
	if( m_wstrObjKey == L"Bomb1T")
	{
		iBoomNum	= 9;
	}
	else if( m_wstrObjKey == L"Bomb2T")
	{
		iBoomNum	= 10;
	}
	else if( m_wstrObjKey == L"Bomb3T")
	{
		iBoomNum	= 8;
	}
	else if( m_wstrObjKey == L"Bomb4-1T")
	{
		iBoomNum	= 26;
	}
	else if( m_wstrObjKey == L"Bomb4-2T")
	{
		iBoomNum	= 36;
	}
	return iBoomNum;
}