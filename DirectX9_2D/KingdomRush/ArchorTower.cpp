#include "StdAfx.h"
#include "ArchorTower.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjFactory.h"
#include "RifleMan.h"
#include "Monster.h"

#include "ObjMgr.h"
#include "Freezing.h"
#include "Click.h"

#include "SDevice.h"

CArchorTower::CArchorTower(void)
:m_bArchorMake(false)
,m_bArchorLockOn(false)
,m_pMonster(NULL)
{
	//ZeroMemory(m_bArchorLockOn, sizeof(bool) * 2);
}

CArchorTower::~CArchorTower(void)
{
	Release();
}
HRESULT CArchorTower::Initialize(void)
{
	m_eTowerID		= TWR_ARCHOR1;
	m_bWark			= true;		// 공사중
	m_tWarkFrame	= SHEETFRM(0, 10, 0, 70);
	m_tTowerStat	= TWRSTATS(4, 6, 1.f, 260.f, 110);
	m_dwWarkTime	= GetTickCount();

	m_wstrObjKey	= L"Archer1T";
	m_wstrStateKey	= L"Tower";

	m_bTowerClick	= false;	// 클릭이 됬는지

	m_iLevel		= 0;
	m_bLevelup		= false;

	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bDestroyTower = false;

	for(int i = 0; i < 2; ++i)
		m_bSkill[i] = false;
	
	m_bSkillInten	= false;
	m_fIntengleTime	= 0.f;

	m_bFreezing		= false;
	m_bFreezImg		= false;
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_SKILL);

	m_iFreezAtkCnt	= 0;

	return S_OK;
}
int CArchorTower::Progress(void)		
{
	if(m_bDestroyTower)
		return 1;

	if(m_bFreezing)	// 얼면 다 안하게
	{
		m_bArchorLockOn = false;
		m_pMonster		= NULL;
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
		if(!m_VecMySoldier.empty())
		{
			for(int i = 0; i < 2; ++i)
			{
				//락온이 되면
				if( !m_bArchorLockOn)
				{
					((CRifleMan*)m_VecMySoldier[i])->SetLockOn(false);
					((CRifleMan*)m_VecMySoldier[i])->SetLockMonster(NULL);
				}
			}
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
			CSDevice::GetInstance()->SoundPlay(SD_ATOWER_TAUNT1, 0);
		}
		else if(m_iLevel == 2)
		{
			CSDevice::GetInstance()->SoundPlay(SD_ATOWER_TAUNT2, 0);
		}
		else if(m_iLevel == 3)
		{
			CSDevice::GetInstance()->SoundPlay(SD_RANG_READY, 0);
		}
		else if(m_iLevel == 4)
		{
			CSDevice::GetInstance()->SoundPlay(SD_MUSK_READY, 0);
		}

		unsigned int VecSize = m_VecMySoldier.size();
		for(unsigned int i = 0; i < VecSize; ++i)
		{
			((CRifleMan*)m_VecMySoldier[i])->SetLevelup(m_iLevel);
			((CRifleMan*)m_VecMySoldier[i])->SetDamage(m_tTowerStat.iMaxDam);
			((CRifleMan*)m_VecMySoldier[i])->SetAttackSpd(m_tTowerStat.fFireSpd);
		}	
	}

	if(m_bSkill[0])
	{
		for(unsigned int i = 0; i < 2; ++i)
		{
			((CRifleMan*)m_VecMySoldier[i])->SetSkillOnOff(0, true);
		}
	}
	if(m_bSkill[1])
	{
		for(unsigned int i = 0; i < 2; ++i)
		{
			((CRifleMan*)m_VecMySoldier[i])->SetSkillOnOff(1, true);
		}
		if(m_wstrObjKey == L"Archer4-1T")
		{
			m_bSkillInten	= true;
			m_fIntengleTime	+= GET_TIME;
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
				CSDevice::GetInstance()->SoundPlay(SD_ATOWER_READY, 0);

				if(!m_bArchorMake)
				{
					for(int i = 0; i < 2; ++i)
					{
						CObj* pRifleMan = new CRifleMan;
						((CRifleMan*)pRifleMan)->SetIndex(i + 1);

						pRifleMan->SetPos(D3DXVECTOR3(m_tInfo.vPos.x - 6.f + (15.f * i), m_tInfo.vPos.y - 26.f, 0.f));
						pRifleMan->Initialize();
						((CRifleMan*)pRifleMan)->SetDamage(m_tTowerStat.iMaxDam);
						((CRifleMan*)pRifleMan)->SetAttackSpd(m_tTowerStat.fFireSpd);	
						((CRifleMan*)pRifleMan)->SetTower(this);

						m_VecMySoldier.push_back(pRifleMan);
						m_pSoldierList->push_back(m_VecMySoldier[i]);
					}
					m_bArchorMake = true;
				}
			}
		}
	}
	if( m_pMonster != NULL )
	{
		if(((CMonster*)m_pMonster)->GetMonsterDie())
		{
			m_bArchorLockOn = false;
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
				m_bArchorLockOn = false;
				m_pMonster		= NULL;
			}
		}
	}

	if(!m_VecMySoldier.empty())
	{
		for(int i = 0; i < 2; ++i)
		{
			//락온이 되면
			if(!((CRifleMan*)m_VecMySoldier[i])->GetLockOn() )
			{
				((CRifleMan*)m_VecMySoldier[i])->SetLockMonster(m_pMonster);
				((CRifleMan*)m_VecMySoldier[i])->SetLockOn(true);
			}
			if( !m_bArchorLockOn)
			{
				((CRifleMan*)m_VecMySoldier[i])->SetLockOn(false);
				((CRifleMan*)m_VecMySoldier[i])->SetLockMonster(NULL);
			}
		}
	}
	return 0;
}
void CArchorTower::Render(void)		
{

	D3DXMATRIX	matScale, matRangeScale, matTrans;

	/* 타워밑바닥 */
	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"stage1G");
						
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
	
	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	/* 건설 */
	if(m_bWark)
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", 17);
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
		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, L"Tower", 0);

		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = pTexture->tImgInfo.Height / 1.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
		if(m_bTowerClick)
		{
			/* 사정거리 */
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
void CArchorTower::Release(void)		
{
	unsigned int VecSize = m_VecMySoldier.size();
	for(unsigned int i = 0; i < VecSize; ++i)
	{
		((CRifleMan*)m_VecMySoldier[i])->SetDie(true);
	}
}
void CArchorTower::Leveling()
{
	if(m_bLvl[LVL_1])
	{
		m_wstrObjKey	= L"Archer1T";
		m_iLevel		= 0;
		m_tTowerStat	= TWRSTATS(4, 6, 1.f, 260.f, 110);
	}
	else if(m_bLvl[LVL_2])
	{
		m_wstrObjKey	= L"Archer2T";
		m_iLevel		= 1;
		m_tTowerStat	= TWRSTATS(7, 11, 0.85f, 280.f, 160);
	}
	else if(m_bLvl[LVL_3])
	{
		m_wstrObjKey	= L"Archer3T";
		m_iLevel		= 2;
		m_tTowerStat	= TWRSTATS(10, 16, 0.7f, 280.f, 230);
		
	}
	else if(m_bLvl[LVL_41])
	{
		m_wstrObjKey	= L"Archer4-1T";
		m_iLevel		= 3;
		m_tTowerStat	= TWRSTATS(13, 19, 0.6f, 300.f, 230);
	}
	else if(m_bLvl[LVL_42])
	{
		m_wstrObjKey	= L"Archer4-2T";
		m_iLevel		= 4;
		m_tTowerStat	= TWRSTATS(35, 65, 3.f, 400.f, 230);
	}

	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bLevelup = false;
}