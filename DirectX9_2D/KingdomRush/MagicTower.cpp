#include "StdAfx.h"
#include "MagicTower.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Wizard.h"

#include "Monster.h"

#include "MagicBall.h"
#include "MagicBolt.h"
#include "MagicRazor.h"
#include "DeathPing.h"
#include "Back.h"

#include "Teleport.h"
#include "Golem.h"

#include "Freezing.h"
#include "Click.h"
#include "Wizard.h"

#include "SDevice.h"

CMagicTower::CMagicTower(void)
:m_bMagicMake(false)
,m_pMonster(NULL)
{
}

CMagicTower::~CMagicTower(void)
{
	Release();
}
HRESULT CMagicTower::Initialize(void)
{
	m_eTowerID		= TWR_MAGIC1;
	m_bWark			= true;		// 공사중
	m_tWarkFrame	= SHEETFRM(0, 10, 0, 70);
	m_tTowerStat	= TWRSTATS(9, 17, 60, 250.f, 160);
	m_dwWarkTime	= GetTickCount();

	m_wstrObjKey	= L"Magic1T";
	m_wstrStateKey	= L"Tower";
	SetFrame(L"Tower");		//기본 모션


	m_bTowerClick	= false;	// 클릭이 됬는지

	m_iLevel		= 0;
	m_bLevelup		= false;
	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bDestroyTower = false;
	m_bMagicLockOn	= false;

	m_bFireInterval	= false;
	m_bFire			= false;
	m_fFireTime		= 0.f;

	m_pBulletList	= CObjMgr::GetInstance()->GetObjList(OBJ_PROJECTILE);
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	for(int i = 0; i < 2; ++i)
		m_bSkill[i] = false;

	{//스킬
		m_bDeathPing	= false;
		m_fDeathTime	= 0.f;

		m_bTeleport		= false;
		m_fTelpoTime	= 0.f;
		m_pVecTile		= ((CBack*)CObjMgr::GetInstance()->GetObjList(OBJ_BACK)->front())->GetVecTile();

		m_bGolemSummon	= false;
		m_bGolemDeath	= true;
		m_fGolemRespawn	= 0.f;
	}

	m_vRellyPoint	= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y + 50.f, 0.f);

	m_bFreezing		= false;
	m_bFreezImg		= false;
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_SKILL);

	m_iFreezAtkCnt	= 0;

	return S_OK;
}
int CMagicTower::Progress(void)		
{
	if(m_bDestroyTower)
		return 1;

	for( vector<CObj*>::iterator iter = m_VecMySoldier.begin();
		iter != m_VecMySoldier.end(); )
	{
		if( ((CSoldier*)(*iter))->GetSoliderID() != SOL_GOLEM )
		{
			++iter;
			continue;
		}

		if( ((CGolem*)(*iter))->GetDie() )
		{
			iter = m_VecMySoldier.erase(iter);
		}
		else
			++iter;
	}

	if(m_bFreezing)
	{
		m_bMagicLockOn  = false;
		m_pMonster		= NULL;
		m_tFrame.fFrame = 0.f;

		if(!m_bFreezImg)
		{
			CObj*	pFreezing	= CObjFactory<CFreezing>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 1.f, 0.f));
			((CFreezing*)pFreezing)->SetTower(this);
			m_pEffectList->push_back(pFreezing);

			CObj*	pClick		= CObjFactory<CClick>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 2.f, 0.f));
			((CClick*)pClick)->SetTower(this);
			m_pEffectList->push_back(pClick);

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
			CSDevice::GetInstance()->SoundPlay(SD_MTOWER_TAUNT1, 0);
		}
		else if(m_iLevel == 2)
		{
			CSDevice::GetInstance()->SoundPlay(SD_MTOWER_TAUNT2, 0);
		}
		else if(m_iLevel == 3)
		{
			CSDevice::GetInstance()->SoundPlay(SD_ARCA_READY, 0);
		}
		else if(m_iLevel == 4)
		{
			CSDevice::GetInstance()->SoundPlay(SD_SORC_READY, 0);
		}

		unsigned int VecSize = m_VecMySoldier.size();
		for(unsigned int i = 0; i < VecSize; ++i)
		{
			((CWizard*)m_VecMySoldier[i])->SetLevelup(m_iLevel);
		}
	}
	if(m_bSkill[0])
	{
		if( m_wstrObjKey == L"Magic4-2T" )
		{
			m_bDeathPing	= true;
		}
		else if( m_wstrObjKey == L"Magic4-1T")
		{
			if(m_bGolemDeath || !m_bGolemSummon)
			{
				CObj* pGolem = new CGolem;
				((CGolem*)pGolem)->SetRellyPoint(m_vRellyPoint);
				pGolem->Initialize();

				m_VecMySoldier.push_back(pGolem);
				m_pSoldierList->push_back(m_VecMySoldier[1]);

				m_bGolemDeath	= false;
			}
			m_bGolemSummon	= true;

			
			if(m_bGolemSummon && !m_bGolemDeath)
			{
				if( m_VecMySoldier.size() == 2 )
					((CGolem*)m_VecMySoldier[1])->SetRellyPoint(m_vRellyPoint);
			}
			if( m_VecMySoldier.size() == 1 )
			{
				m_fGolemRespawn		+= GET_TIME;
				if( m_fGolemRespawn	> 3.f )
				{
					m_bGolemDeath		= true;
				}
			}
		}
	}
	if(m_bSkill[1])
	{
		if( m_wstrObjKey == L"Magic4-2T" )
		{
			m_bTeleport		= true;
		}
		else if( m_wstrObjKey == L"Magic4-1T")
		{
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
				m_tWarkFrame.iStart = 0;
				m_bWark = false;

				CSDevice::GetInstance()->SoundPlay(SD_MTOWER_READY, 0);
				if(!m_bMagicMake)
				{
					CObj* pWizard = new CWizard;
					pWizard->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 30.f, 0.f));
					pWizard->Initialize();
					((CWizard*)pWizard)->SetTower(this);

					m_VecMySoldier.push_back(pWizard);
					m_pSoldierList->push_back(m_VecMySoldier[0]);

					m_bMagicMake = true;
				}
			}
		}
	}

	if( m_pMonster != NULL )
	{
		if(((CMonster*)m_pMonster)->GetMonsterDie())
		{
			m_bMagicLockOn	= false;
			m_pMonster		= NULL;
			m_tFrame.fFrame = 0.f;
		}
		else
		{
			/*거리에서 벗어나면*/
			float fDisX	 = m_tInfo.vPos.x - ((CMonster*)m_pMonster)->GetInfo().vPos.x;
			float fDisY	 = m_tInfo.vPos.y - ((CMonster*)m_pMonster)->GetInfo().vPos.y;
			float fRange = m_tTowerStat.iRange / 2.f;

			if( (pow(fDisX, 2) / pow(fRange, 2)) + (pow(fDisY, 2) / pow(fRange/ 1.3f , 2)) >= 1.f )
			{
				m_bMagicLockOn  = false;
				m_pMonster		= NULL;
				m_tFrame.fFrame = 0.f;
			}
		}
	}
	if(m_bMagicLockOn)
	{	
		if(m_bWark)
			return 0;

		if(m_bDeathPing)
			m_fDeathTime	+= GET_TIME;
		if(m_bTeleport)
			m_fTelpoTime	+= GET_TIME;

		if(m_fTelpoTime > 4.f)
		{
			/*int			iPreNode = 0;
			iPreNode  = ((CMonster*)m_pMonster)->GetOriginRoute()->size() - ((CMonster*)m_pMonster)->GetMyRoute()->size();*/

			if( ((CMonster*)m_pMonster)->GetMonsterID() != MON_SNOWMAN 
				&& ((CMonster*)m_pMonster)->GetMonsterID() != MON_SPIDER
				&& ((CMonster*)m_pMonster)->GetMonsterID() != MON_BABYSPIDER)
			{

				vector<int> vecTelpoRoute;
				
				vecTelpoRoute = (*((CMonster*)m_pMonster)->GetOriginRoute());

				CObj* pTelpo = CObjFactory<CTeleport>::CreateObj(m_pMonster->GetInfo().vPos);
				((CTeleport*)pTelpo)->SetMonster(m_pMonster);
				m_pEffectList->push_back(pTelpo);

				pTelpo = CObjFactory<CTeleport>::CreateObj(m_pMonster->GetInfo().vPos);
				((CTeleport*)pTelpo)->SetMonster(NULL);
				m_pEffectList->push_back(pTelpo);
				
				int iRand = rand()%3;
				for(int i = 0; i < iRand; ++i)
				{
					vecTelpoRoute.pop_back();
				}
				((CMonster*)m_pMonster)->SetMyRoute(vecTelpoRoute);
				((CMonster*)m_pMonster)->SetTelpo(true);
				m_pMonster->SetPos( (*m_pVecTile)[vecTelpoRoute.back()]->vPos );
			}

			m_fTelpoTime = 0.f;			
		}

		if(!m_bFireInterval)
			FrameMove();	// 프레임속도 조정
		if(m_tFrame.fFrame == 0)
		{
			m_bFire			= false;
			m_bFireInterval	= true;
		}
		if(!m_bFireInterval)
		{
			int iMagicNum = MagicNumbering();
			if( int(m_tFrame.fFrame) == iMagicNum && !m_bFire)
			{
				if(m_wstrObjKey != L"Magic4-2T" && m_wstrObjKey != L"Magic4-1T")
				{
					CObj* pMagicBall = new CMagicBall;
					pMagicBall->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 50.f, 0.f));
					((CMagicBall*)pMagicBall)->SetMonTarget(m_pMonster);
					((CMagicBall*)pMagicBall)->SetDamage(m_tTowerStat.iMaxDam);
					pMagicBall->Initialize();

					m_pBulletList->push_back(pMagicBall);

					CSDevice::GetInstance()->SoundPlay(SD_MAGICSHOT, 0);
				}
				else if(m_wstrObjKey == L"Magic4-1T")
				{
					CObj* pMagicBolt = new CMagicBolt;
					pMagicBolt->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 50.f, 0.f));
					((CMagicBolt*)pMagicBolt)->SetMonTarget(m_pMonster);
					((CMagicBolt*)pMagicBolt)->SetDamage(m_tTowerStat.iMaxDam);
					pMagicBolt->Initialize();

					m_pBulletList->push_back(pMagicBolt);

					CSDevice::GetInstance()->SoundPlay(SD_MAGICBOLT, 0);
				}
				else if(m_wstrObjKey == L"Magic4-2T")
				{
					if(m_fDeathTime	> 2.f)	//데스핑
					{
						// 레이져 쏘고 데스타임 0
						CObj* pDeath = new CDeathPing;
						((CDeathPing*)pDeath)->SetMonTarget(m_pMonster);
						pDeath->Initialize();
						pDeath->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 47.f, 0.f));
						m_pBulletList->push_back(pDeath);

						m_fDeathTime	= 0.f;

						CSDevice::GetInstance()->SoundPlay(SD_DEATHPING, 0);
						//m_fFireTime		= 0.f;
					}
					else
					{
						CObj* pRazor = new CMagicRazor;
						((CMagicRazor*)pRazor)->SetMonTarget(m_pMonster);
						((CMagicRazor*)pRazor)->SetDamage(m_tTowerStat.iMaxDam);
						pRazor->Initialize();
						pRazor->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 47.f, 0.f));
						m_pBulletList->push_back(pRazor);

						CSDevice::GetInstance()->SoundPlay(SD_MAGICRAZOR, 0);
					}					
				}
				m_bFire = true;
			}
		}
		else
		{
			m_fFireTime	+= GET_TIME;
			if( m_fFireTime > m_tTowerStat.fFireSpd )		// 1.f 변수로 둬서 공속으로 사용
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
		m_fDeathTime	= 0.f;
	}

	if(!m_VecMySoldier.empty())
	{		
	}
	return 0;
}
void CMagicTower::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"stage1G");

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	/* 건설 */
	if(m_bWark)
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", 18);
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
		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, L"Tower", (int)m_tFrame.fFrame);
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
void CMagicTower::Release(void)		
{
	//unsigned int VecSize = m_VecMySoldier.size();
	
	((CWizard*)m_VecMySoldier[0])->SetDie(true);
	if(m_bGolemSummon)
	{
		if( m_VecMySoldier.size() == 2 )
			((CGolem*)m_VecMySoldier[1])->SetDie(true);
	}
}
void CMagicTower::Leveling()
{
	if(m_bLvl[LVL_1])
	{
		m_wstrObjKey	= L"Magic1T";
		m_iLevel		= 0;
		m_tTowerStat	= TWRSTATS(9, 17, 2.f, 250.f, 160);
	}
	else if(m_bLvl[LVL_2])
	{
		m_wstrObjKey	= L"Magic2T";
		m_iLevel		= 1;
		m_tTowerStat	= TWRSTATS(23, 43, 2.f, 250.f, 240);
	}
	else if(m_bLvl[LVL_3])
	{
		m_wstrObjKey	= L"Magic3T";
		m_iLevel		= 2;
		m_tTowerStat	= TWRSTATS(40, 74, 2.f, 260.f, 240);
	}
	else if(m_bLvl[LVL_41])
	{
		m_wstrObjKey	= L"Magic4-2T";
		m_iLevel		= 3;
		m_tTowerStat	= TWRSTATS(42, 90, 2.f, 250.f, 240);///////////////////////////////////////////////////////////// 데미지 바꿔놓기
	}
	else if(m_bLvl[LVL_42])
	{
		m_wstrObjKey	= L"Magic4-1T";
		m_iLevel		= 4;
		m_tTowerStat	= TWRSTATS(76, 78, 4.f, 270.f, 240);
	}

	SetFrame(L"Tower");		//기본 모션
	m_tFrame.fFrame = 0.f;


	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bLevelup = false;


}
int CMagicTower::MagicNumbering()
{
	int iMagicNum = -1;
	if( m_wstrObjKey == L"Magic1T")
	{
		iMagicNum	= 4;
	}
	else if( m_wstrObjKey == L"Magic2T")
	{
		iMagicNum	= 4;
	}
	else if( m_wstrObjKey == L"Magic3T")
	{
		iMagicNum	= 4;
	}
	else if( m_wstrObjKey == L"Magic4-2T")
	{
		iMagicNum	= 13;
	}
	else if( m_wstrObjKey == L"Magic4-1T")
	{
		iMagicNum	= 7;
	}
	return iMagicNum;
}