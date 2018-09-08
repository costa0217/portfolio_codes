#include "StdAfx.h"
#include "KnightTower.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjFactory.h"
#include "FootMan.h"

#include "ObjMgr.h"
#include "Freezing.h"
#include "Click.h"

#include "SDevice.h"

CKnightTower::CKnightTower(void)
{
}

CKnightTower::~CKnightTower(void)
{
	Release();
}
HRESULT CKnightTower::Initialize(void)
{
	m_eTowerID		= TWR_KNIGHT1;
	m_bWark			= true;		// 공사중
	m_tWarkFrame	= SHEETFRM(0, 10, 0, 70);
	m_tTowerStat	= TWRSTATS(1, 3, 10, 250.f, 110);
	m_dwWarkTime	= GetTickCount();

	m_fRespawnTime	= 0;

	m_wstrObjKey	= L"Knight1T";
	m_wstrStateKey	= L"Tower";

	m_bTowerClick	= false;	// 클릭이 됬는지
	
	m_iLevel		= 0;
	m_bLevelup		= false;
	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bDestroyTower = false;

	for(int i = 0; i < 3; ++i)
	{
		CObj* pFootMan = new CFootMan;
		((CFootMan*)pFootMan)->SetRellyPoint(D3DXVECTOR3(m_vRellyPoint.x, m_vRellyPoint.y, 0.f));
		((CFootMan*)pFootMan)->SetIndex(i + 1);

		pFootMan->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
		pFootMan->Initialize();

		m_VecMySoldier.push_back(pFootMan);
		m_pSoldierList->push_back(m_VecMySoldier[i]);
	}

	for(int i = 0; i < 2; ++i)
		m_bSkill[i] = false;

	m_bFreezing		= false;
	m_bFreezImg		= false;
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_SKILL);

	m_iFreezAtkCnt	= 0;

	return S_OK;
}
int CKnightTower::Progress(void)		
{

	if(m_bDestroyTower)
		return 1;

	

	/* 풋맨다이 */
	for( vector<CObj*>::iterator iter = m_VecMySoldier.begin();
		iter != m_VecMySoldier.end(); )
	{
		if( ((CFootMan*)(*iter))->GetDie() )
		{
			iter = m_VecMySoldier.erase(iter);
		}
		else
			++iter;
	}
	/* 리스폰 */
	unsigned int VecSize = m_VecMySoldier.size();
	if(VecSize < 3)
	{
		m_fRespawnTime += GET_TIME;

		bool	bFootAlive[3];
		ZeroMemory(bFootAlive, sizeof(bool) * 3);
		for(unsigned int i = 0; i < VecSize; ++i)
		{
			bFootAlive[((CFootMan*)m_VecMySoldier[i])->GetIndex() - 1] = true;
		}
		for( int i = 0; i < 3; ++i )
		{
			if( m_fRespawnTime > 3.f)
			{
				if(!bFootAlive[i])
				{
					CObj* pFootMan = new CFootMan;
					((CFootMan*)pFootMan)->SetRellyPoint(D3DXVECTOR3(m_vRellyPoint.x, m_vRellyPoint.y, 0.f));
					((CFootMan*)pFootMan)->SetIndex(i + 1);
					
					pFootMan->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
					pFootMan->Initialize();
					((CFootMan*)pFootMan)->SetLevelup(m_iLevel);

					m_VecMySoldier.push_back(pFootMan);
					m_pSoldierList->push_back(pFootMan);

					m_fRespawnTime = 0.f;
				}
			}
		}
	}
	else
	{
		m_fRespawnTime = 0.f;
	}
	

	if(m_bFreezing)
	{
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

	/* 랠리포인트 */
	for(unsigned int i = 0; i < VecSize; ++i)
	{
		((CFootMan*)m_VecMySoldier[i])->SetRellyPoint(m_vRellyPoint);
	}
	if(m_vPreRelly != m_vRellyPoint)
	{
		//챠지!!
		int iRand = rand()%3;
		if(m_iLevel != 3 && m_iLevel != 4)
		{
			if(iRand == 0)
				CSDevice::GetInstance()->SoundPlay(SD_KTOWER_MOVE, 0);
		}
		else if(m_iLevel == 3)
		{
			if(iRand == 0)
				CSDevice::GetInstance()->SoundPlay(SD_PALA_MOVE, 0);
		}
		else if(m_iLevel == 4)
		{
			if(iRand == 0)
				CSDevice::GetInstance()->SoundPlay(SD_BABA_MOVE, 0);
		}
	}	

	/* 레벨업 */
	if(m_bLevelup)
	{
		Leveling();
		if(m_iLevel == 1)
		{
			CSDevice::GetInstance()->SoundPlay(SD_KTOWER_TAUNT1, 0);
		}
		else if(m_iLevel == 2)
		{
			CSDevice::GetInstance()->SoundPlay(SD_KTOWER_TAUNT2, 0);
		}
		else if(m_iLevel == 3)
		{
			CSDevice::GetInstance()->SoundPlay(SD_PALA_READY, 0);
		}
		else if(m_iLevel == 4)
		{
			CSDevice::GetInstance()->SoundPlay(SD_BABA_READY, 0);
		}
		for(unsigned int i = 0; i < VecSize; ++i)
		{
			((CFootMan*)m_VecMySoldier[i])->SetLevelup(m_iLevel);
		}	

	}
	if(m_bSkill[0])
	{
		for(unsigned int i = 0; i < VecSize; ++i)
		{
			((CFootMan*)m_VecMySoldier[i])->SetSkillOnOff(0, true);
		}
	}
	if(m_bSkill[1])
	{
		for(unsigned int i = 0; i < VecSize; ++i)
		{
			((CFootMan*)m_VecMySoldier[i])->SetSkillOnOff(1, true);
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
				CSDevice::GetInstance()->SoundPlay(SD_KTOWER_READY, 0);
				m_tWarkFrame.iStart = 0;
				m_bWark = false;
			}
		}
	}
	m_vPreRelly	= m_vRellyPoint;
	return 0;
}
void CKnightTower::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans;

	/* 바닥 */
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
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", 16);
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
	}
}
void CKnightTower::Release(void)		
{
	unsigned int VecSize = m_VecMySoldier.size();
	for(unsigned int i = 0; i < VecSize; ++i)
	{
		((CFootMan*)m_VecMySoldier[i])->SetDie(true);
	}
	//m_VecMySoldier.clear();	
}
void CKnightTower::Leveling()
{
	if(m_bLvl[LVL_1])
	{
		m_wstrObjKey	= L"Knight1T";
		m_eTowerID		= TWR_KNIGHT1;
		m_tTowerStat	= TWRSTATS(1, 3, 10, 250.f, 110);
		m_iLevel		= 0;
	}
	else if(m_bLvl[LVL_2])
	{
		m_wstrObjKey	= L"Knight2T";
		m_tTowerStat	= TWRSTATS(1, 3, 10, 250.f, 110);
		m_iLevel		= 1;
	}
	else if(m_bLvl[LVL_3])
	{
		m_wstrObjKey	= L"Knight3T";
		m_iLevel		= 2;
	}
	else if(m_bLvl[LVL_41])
	{
		m_wstrObjKey	= L"Knight4-1T";
		m_iLevel		= 3;
	}
	else if(m_bLvl[LVL_42])
	{
		m_wstrObjKey	= L"Knight4-2T";
		m_iLevel		= 4;
	}

	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bLevelup = false;
}