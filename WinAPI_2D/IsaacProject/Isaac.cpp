#include "StdAfx.h"
#include "Isaac.h"
#include "Obj.h"
#include "KeyMgr.h"
#include "ObjFactory.h"

#include "PlayerBig.h"
#include "PlayerKnife.h"
#include "PlayerBullet.h"
#include "PlayerSnake.h"
#include "Bomb.h"

#include "SkillAll.h"
#include "SkillMeteo.h"
#include "SkillLemon.h"

#include "FamFly.h"
#include "FamBaby.h"

#include "Hart.h"

#include "ObjMgr.h"
#include "Device.h"


#define ISAAC_SIZE 32
CIsaac::CIsaac(void)
:m_dwKey(0)
,m_dwTime(GetTickCount()), m_dwShot(GetTickCount())
,m_iRoomNum(0)
{
}

CIsaac::~CIsaac(void)
{
	Release();
}

void CIsaac::Initialize(void)
{
	m_BlendDC = GetDC(g_hWnd);
	

	
	m_tInfo.fCX  = 50.f;
	m_tInfo.fCY  = 50.f;
	m_strKey	 = "isaac";
	m_iHP		 = 6;

	m_fSpeedX		= 0;
	m_fSpeedY		= 0;
	m_fMaxSpeedX	= 7.f;
	m_fMaxSpeedY	= 7.f;
	
	m_tFrame		= FRAME(0, 7, 1, 80);
	m_dwState		= ST_STAND;
	m_iIsaacHead	= IH_FRONT;
	m_eDir			= DIR_DOWN;
	m_bOnHit		= false;
	m_iHitTime		= 0;
	m_bHPdown		= false;

	m_bHPUP			= false;

	m_bFamFlyEvolve = false;
	m_bFamBabyEvolve= false;

	m_iCoinNum		= 0;		
	m_iBombNum		= 1;		
	m_iKeyNum		= 0;	

	m_iAttackSpeed	= 500;		//공격속도

	m_iReach		= 300;		//사거리
	m_iDamage		= 2;		//데미지

	m_eArrowType	= ARROW_DEFAULT;	// 총알 종류
	m_eActiveType	= ACTIVE_Default;	// 아이템 종류

	/*액티브 변수*/
	m_bACT_hanging	= false;
	m_bACT_AllAttack= false;
	m_bACT_Meteo	= false;
	m_bACT_Teleport	= false;
	m_bACT_Lemon	= false;
}
int CIsaac::Progress(void)
{
	if(m_bACT_Lemon)
	{
		CObj* pPlayerBullet = CObjFactory<CSkillLemon>::CreateObj( m_tInfo.fX, m_tInfo.fY);
		m_pBulletlist->push_back(pPlayerBullet);
		m_bACT_Lemon = false;
	}
	if(m_bACT_Teleport)
	{
		m_tInfo.fX = float(rand()%600 + 100);
		m_tInfo.fY = float(rand()%350 + 200);
		m_bACT_Teleport = false;
	}
	if(m_bACT_Meteo)
	{
		for(int i = 0; i < 15; ++i)
		{
		CObj* pPlayerBullet = CObjFactory<CSkillMeteo>::CreateObj(float(rand()%640 + 80), float(rand()%370) + 190);
		m_pBulletlist->push_back(pPlayerBullet);
		}

		m_bACT_Meteo = false;
	}
	if(m_bACT_AllAttack)
	{
		CObj* pBullet = CObjFactory<CSkillAll>::CreateObj(WINCX /2.f, 100.f);
		m_pBulletlist->push_back(pBullet);
		pBullet = CObjFactory<CSkillAll>::CreateObj(WINCX /4.f, 80.f);
		m_pBulletlist->push_back(pBullet);
		pBullet = CObjFactory<CSkillAll>::CreateObj(WINCX * 0.81f, 90.f);
		m_pBulletlist->push_back(pBullet);

		m_bACT_AllAttack = false;
	}

	if(m_bFamFlyEvolve)
	{
		CObj* pFamilyer = CObjFactory<CFamFly>::CreateObj(m_tInfo.fX, m_tInfo.fY);
		((CFamFly*)pFamilyer)->SetPlayer(this);
		m_pFamilyerlist->push_back(pFamilyer);

		m_bFamFlyEvolve = false;
	}
	if(m_bFamBabyEvolve)
	{
		CObj* pFamilyer = CObjFactory<CFamBaby>::CreateObj(m_tInfo.fX, m_tInfo.fY);
		((CFamBaby*)pFamilyer)->SetPlayer(this);
		m_pFamilyerlist->push_back(pFamilyer);

		m_bFamBabyEvolve = false;
	}
	if(m_bHPUP)
	{	/*체력 하트 작업*/
		for(vector<CObj*>::reverse_iterator iter = m_pVecHart->rbegin();
				iter != m_pVecHart->rend(); ++iter)
		{
			((CHart*)(*iter))->SetHP(2);
		}
		m_iHP	= m_pVecHart->size() * 2;

		if(m_pVecHart->size() < 6)
			m_pVecHart->push_back(CObjFactory<CHart>::CreateObj(590.f + (33 * m_pVecHart->size()), 40.f) );
		else
			m_pVecHart->push_back(CObjFactory<CHart>::CreateObj(590.f + (33 * (m_pVecHart->size() - 6)), 70.f) );

		m_iHP	+= 2;
		m_bHPUP = false;
	}
	if(m_bOnHit)
	{
		
		
		if(!m_bHPdown)
		{
			CDevice::GetInstance()->SoundPlay(HURT, 0);
			for(vector<CObj*>::reverse_iterator iter = m_pVecHart->rbegin();
				iter != m_pVecHart->rend(); )
			{
				if( ((CHart*)(*iter))->GetHP() > 0 )
				{
					--m_iHP;
					((CHart*)(*iter))->SetHP(((CHart*)(*iter))->GetHP() - 1);
					break;
				}
				else/* if(((CHart*)(*iter))->GetHP() == 0 )*/
				{
					++iter;
				}
			}
		}
		m_bHPdown = true;
		++m_iHitTime;
		if(m_iHitTime > 80)
		{
			m_iHitTime	= 0;
			m_bOnHit	= false;
			m_bHPdown	= false;
		}
	}
	KeyInput();
	m_tInfo.fX += m_fSpeedX;	
	m_tInfo.fY += m_fSpeedY;	

	SetMotion(ST_STAND, 7, 1, 80);
	SetMotion(ST_UP, 7, 2, 80);
	SetMotion(ST_DOWN, 7, 2, 80);
	SetMotion(ST_LEFT, 7, 4, 80);
	SetMotion(ST_RIGHT, 7, 3, 80);
	SetMotion(ST_ITEM, 7, 5, 100);
	
	return 0;
}
void CIsaac::Render(HDC hdc)
{
	TransparentBlt(hdc,						//그림자
		int(m_tInfo.fX - m_tInfo.fCX / 3.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f) + 30,
		int(m_tInfo.fCX / 1.5f), int(m_tInfo.fCY / 4.f),
		(*m_pBitMap)["shadow"]->GetMemDC(),
		0, 0,
		120, 49,
		RGB(61, 16, 123));

	if(!m_bOnHit)
	{
		if( m_dwState != ST_ITEM)
		{
			if(!m_bACT_hanging)
			{
			TransparentBlt(hdc,						//몸통
				int(m_tInfo.fX - m_tInfo.fCX / 2.f),
				int(m_tInfo.fY - m_tInfo.fCY / 2.f),
				int(m_tInfo.fCX), int(m_tInfo.fCY),
				(*m_pBitMap)[m_strKey]->GetMemDC(),
				int(ISAAC_SIZE * m_tFrame.iStart),	
				int(ISAAC_SIZE * m_tFrame.iScene),
				ISAAC_SIZE, ISAAC_SIZE,
				RGB(61, 16, 123));
			}

			TransparentBlt(hdc,		// 머리
				int(m_tInfo.fX - m_tInfo.fCX / 2.f) ,
				int(m_tInfo.fY - m_tInfo.fCY / 2.f) - 18,
				int(m_tInfo.fCX), int(m_tInfo.fCY),
				(*m_pBitMap)[m_strKey]->GetMemDC(),
				int(ISAAC_SIZE * m_iIsaacHead),	0,
				ISAAC_SIZE, ISAAC_SIZE,
				RGB(61, 16, 123));
		}
		else if( m_dwState == ST_ITEM )
		{
			TransparentBlt(hdc,		// 득템
				int(m_tInfo.fX - m_tInfo.fCX / 2.f) ,
				int(m_tInfo.fY - m_tInfo.fCY / 2.f) - 15,
				int(m_tInfo.fCX), int(m_tInfo.fCY),
				(*m_pBitMap)[m_strKey]->GetMemDC(),
				int(ISAAC_SIZE * m_tFrame.iStart),
				int(ISAAC_SIZE * m_tFrame.iScene),
				ISAAC_SIZE, ISAAC_SIZE,
				RGB(61, 16, 123));
		}
	}
	else
	{
		if( m_dwState != ST_ITEM)
		{
			if(!m_bACT_hanging)
			{
			TransparentBlt(m_BlendDC,						//몸통
				int(m_tInfo.fX - m_tInfo.fCX / 2.f),
				int(m_tInfo.fY - m_tInfo.fCY / 2.f),
				int(m_tInfo.fCX), int(m_tInfo.fCY),
				(*m_pBitMap)[m_strKey]->GetMemDC(),
				int(ISAAC_SIZE * m_tFrame.iStart),	
				int(ISAAC_SIZE * m_tFrame.iScene),
				ISAAC_SIZE, ISAAC_SIZE,
				RGB(61, 16, 123));
			}
			TransparentBlt(m_BlendDC,						// 머리
				int(m_tInfo.fX - m_tInfo.fCX / 2.f) ,
				int(m_tInfo.fY - m_tInfo.fCY / 2.f) - 18,
				int(m_tInfo.fCX), int(m_tInfo.fCY),
				(*m_pBitMap)[m_strKey]->GetMemDC(),
				int(ISAAC_SIZE * m_iIsaacHead),	0,
				ISAAC_SIZE, ISAAC_SIZE,
				RGB(61, 16, 123));

		}
		else if( m_dwState == ST_ITEM )
		{
			TransparentBlt(m_BlendDC,		// 득템
				int(m_tInfo.fX - m_tInfo.fCX / 2.f) ,
				int(m_tInfo.fY - m_tInfo.fCY / 2.f) - 15,
				int(m_tInfo.fCX), int(m_tInfo.fCY),
				(*m_pBitMap)[m_strKey]->GetMemDC(),
				int(ISAAC_SIZE * m_tFrame.iStart),
				int(ISAAC_SIZE * m_tFrame.iScene),
				ISAAC_SIZE, ISAAC_SIZE,
				RGB(61, 16, 123));

		}
	}
}
void CIsaac::Release(void)
{
	for(vector<CObj*>::iterator iter = m_pVecHart->begin();
		iter != m_pVecHart->end(); ++iter)
	{
		::Safe_Delete(*iter);
	}
	m_pVecHart->clear();
	vector<CObj*>().swap((*m_pVecHart));
}
void CIsaac::KeyInput(void)
{
	m_dwKey = CKeyMgr::GetInst()->GetKey();

	// 머리 키입력
	if(GetAsyncKeyState(VK_UP))
	{
		m_iIsaacHead = IH_REAR;
		if(m_dwShot + m_iAttackSpeed < GetTickCount())
		{
			m_dwShot = GetTickCount();
			++m_iIsaacHead;
			if(m_eArrowType == ARROW_DEFAULT)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 0.f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_TRIPLE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 0.f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 6.f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 0.28f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_BLOOD)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 0.f, m_iReach, m_iDamage + 1);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_KNIFE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerKnife>::CreateObj(m_tInfo.fX, m_tInfo.fY , 0.f, m_iReach, m_iDamage);
				((CPlayerKnife*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_SNAKE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerSnake>::CreateObj(m_tInfo.fX, m_tInfo.fY , 0.f, m_iReach, m_iDamage);
				m_pBulletlist->push_back(pPlayerBullet);
			}	
			else if(m_eArrowType == ARROW_BIG)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBig>::CreateObj(m_tInfo.fX, m_tInfo.fY - (rand()%5 + 16), 0.f, m_iReach- 100, m_iDamage);
				((CPlayerBig*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
		}
	}
	else if(GetAsyncKeyState(VK_DOWN))
	{
		m_iIsaacHead = IH_FRONT;
		if(m_dwShot + m_iAttackSpeed < GetTickCount())
		{
			m_dwShot = GetTickCount();
 			++m_iIsaacHead;
			if(m_eArrowType == ARROW_DEFAULT)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 3.14f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_TRIPLE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 2.86f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 3.14f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 3.42f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_BLOOD)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 3.14f, m_iReach, m_iDamage + 1);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_KNIFE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerKnife>::CreateObj(m_tInfo.fX, m_tInfo.fY , 3.14f, m_iReach, m_iDamage);
				((CPlayerKnife*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_SNAKE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerSnake>::CreateObj(m_tInfo.fX, m_tInfo.fY , 3.14f, m_iReach, m_iDamage);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_BIG)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBig>::CreateObj(m_tInfo.fX, m_tInfo.fY - (rand()%5 + 16), 3.14f, m_iReach- 100, m_iDamage);
				((CPlayerBig*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
		}
	}
	else if(GetAsyncKeyState(VK_LEFT))
	{
		m_iIsaacHead = IH_LEFT;
		if(m_dwShot + m_iAttackSpeed < GetTickCount())
		{
			m_dwShot = GetTickCount();
			++m_iIsaacHead;
			if(m_eArrowType == ARROW_DEFAULT)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY - (rand()%5 + 16), 4.71f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_TRIPLE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 4.43f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 4.71f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 4.99f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_BLOOD)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 4.71f, m_iReach, m_iDamage + 1);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_KNIFE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerKnife>::CreateObj(m_tInfo.fX, m_tInfo.fY , 4.71f, m_iReach, m_iDamage);
				((CPlayerKnife*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_SNAKE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerSnake>::CreateObj(m_tInfo.fX, m_tInfo.fY , 4.71f, m_iReach, m_iDamage);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_BIG)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBig>::CreateObj(m_tInfo.fX, m_tInfo.fY - (rand()%5 + 16), 4.71f, m_iReach- 100, m_iDamage);
				((CPlayerBig*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
		}
	}
	else if(GetAsyncKeyState(VK_RIGHT))
	{
		m_iIsaacHead = IH_RIGHT;
		if(m_dwShot + m_iAttackSpeed < GetTickCount())
		{
			m_dwShot = GetTickCount();
			++m_iIsaacHead;
			if(m_eArrowType == ARROW_DEFAULT)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY - (rand()%5 + 16), 1.57f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_TRIPLE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 1.29f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 1.57f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
				pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 1.85f, m_iReach, m_iDamage);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_BLOOD)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBullet>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 1.57f, m_iReach, m_iDamage + 1);
				((CPlayerBullet*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_KNIFE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerKnife>::CreateObj(m_tInfo.fX - 10 + rand()%21, m_tInfo.fY , 1.57f, m_iReach, m_iDamage);
				((CPlayerKnife*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_SNAKE)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerSnake>::CreateObj(m_tInfo.fX, m_tInfo.fY , 1.57f, m_iReach, m_iDamage);
				m_pBulletlist->push_back(pPlayerBullet);
			}
			else if(m_eArrowType == ARROW_BIG)
			{
				CObj* pPlayerBullet = CObjFactory<CPlayerBig>::CreateObj(m_tInfo.fX, m_tInfo.fY - (rand()%5 + 16), 1.57f, m_iReach - 100, m_iDamage);
				((CPlayerBig*)pPlayerBullet)->SetPlayer(this);
				m_pBulletlist->push_back(pPlayerBullet);
			}
		}
	}
	// =====스-페-이-스-바======
	if(GetAsyncKeyState(VK_SPACE) &  0x0001)
	{
		m_dwState = ST_ITEM;
		BoolReset();

		CDevice::GetInstance()->SoundPlay(HOLY, 0);

		if(m_eActiveType == ACTIVE_Hanging)
			m_bACT_hanging = true;
		else if(m_eActiveType == ACTIVE_AllAttack)
			m_bACT_AllAttack = true;
		else if(m_eActiveType == ACTIVE_Meteo)
			m_bACT_Meteo = true;
		else if(m_eActiveType == ACTIVE_Teleport)
			m_bACT_Teleport = true;
		else if(m_eActiveType == ACTIVE_Lemon)
			m_bACT_Lemon = true;

		m_eActiveType = ACTIVE_Default;
	}	
	// =========================

	//폭탄생성
	if( (m_iBombNum > 0) && ((GetAsyncKeyState(VK_SHIFT) &  0x0001) || (GetAsyncKeyState('E') &  0x0001)))
	{
		CObj* pPlayerBullet = CObjFactory<CBomb>::CreateObj(m_tInfo.fX - 17, m_tInfo.fY - 17);
			m_pBulletlist->push_back(pPlayerBullet);
		--m_iBombNum;
	}
	

	// 몸통 키입력
	/* 대각선 */
	if((m_dwKey & KEY_LEFT) && (m_dwKey & KEY_UP))
	{
		m_eDir		= DIR_LEFT;
		m_dwState	= ST_LEFT;
		if( -m_fMaxSpeedX < m_fSpeedX)
			m_fSpeedX -= 0.5f;
		if( -m_fMaxSpeedY < m_fSpeedY)
			m_fSpeedY -= 0.5f;
		return;
	}
	if((m_dwKey & KEY_LEFT) && (m_dwKey & KEY_DOWN))
	{
		m_eDir		= DIR_LEFT;
		m_dwState	= ST_LEFT;
		if( -m_fMaxSpeedX < m_fSpeedX)
			m_fSpeedX -= 0.5f;
		if( m_fMaxSpeedY > m_fSpeedY)
			m_fSpeedY += 0.5f;
		return;
	}
	if((m_dwKey & KEY_RIGHT) && (m_dwKey & KEY_DOWN))
	{
		m_eDir		= DIR_RIGHT;
		m_dwState	= ST_RIGHT;
		if( m_fMaxSpeedX > m_fSpeedX)
			m_fSpeedX += 0.5f;
		if( m_fMaxSpeedY > m_fSpeedY)
			m_fSpeedY += 0.5f;
		return;
	}
	if((m_dwKey & KEY_RIGHT) && (m_dwKey & KEY_UP))
	{
		m_eDir		= DIR_RIGHT;
		m_dwState	= ST_RIGHT;
		if( m_fMaxSpeedX > m_fSpeedX)
			m_fSpeedX += 0.5f;
		if( -m_fMaxSpeedY < m_fSpeedY)
			m_fSpeedY -= 0.5f;
		return;
	}

	/* 일직선 */
	if(GetAsyncKeyState('A'))
	{
		m_eDir		= DIR_LEFT;
		m_dwState	= ST_LEFT;
		OUTAGE_Y;

		if( -m_fMaxSpeedX < m_fSpeedX)
			--m_fSpeedX;
	}
	else if(GetAsyncKeyState('D'))
	{
		m_eDir		= DIR_RIGHT;
		m_dwState	= ST_RIGHT;
		OUTAGE_Y;

		if( m_fMaxSpeedX > m_fSpeedX)
			++m_fSpeedX;
	}

	else if(GetAsyncKeyState('W'))
	{
		m_eDir		= DIR_UP;
		m_dwState	= ST_UP;
		OUTAGE_X;

		if( -m_fMaxSpeedY < m_fSpeedY)
			--m_fSpeedY;
	}
	else if(GetAsyncKeyState('S'))
	{
		m_eDir		= DIR_DOWN;
		m_dwState	= ST_DOWN;
		OUTAGE_X;

		if( m_fMaxSpeedY > m_fSpeedY)
			++m_fSpeedY;
	}
	else // 관성
	{
		if(m_fSpeedX > 0 )
			m_fSpeedX -= 0.5f;
		else if(m_fSpeedX < 0 )
			m_fSpeedX += 0.5f;

		if(m_fSpeedY > 0 )
			m_fSpeedY -= 0.5f;
		else if(m_fSpeedY < 0 )
			m_fSpeedY += 0.5f;
	}
}

void CIsaac::SetMotion(DWORD _dwState, int _iLast, int _iScene, DWORD _dwTime)
{
	if(m_dwTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwTime = GetTickCount();
		++m_tFrame.iStart;
	}

	if(m_tFrame.iStart >= m_tFrame.iLast)
	{
		if(m_dwState != ST_STAND)
			m_dwState = ST_STAND;

		m_tFrame.iStart = 0;
	}

	if(m_dwState & _dwState)
	{
		if(m_tFrame.iScene != _iScene)
			m_tFrame.iStart = 0;

		m_tFrame.iLast = _iLast;
		m_tFrame.iScene = _iScene;
		m_tFrame.dwTime = _dwTime;
	}
}
void CIsaac::SetFamilyerlist(list<CObj*>* _pFamilyerlist)
{
	m_pFamilyerlist = _pFamilyerlist;
}
void CIsaac::SetBulletlist(list<CObj*>* _pBulletlist)
{
	m_pBulletlist = _pBulletlist;
}
void CIsaac::SetVect(vector<CObj*>* _pVecHart)
{
	m_pVecHart = _pVecHart;
}
RECT CIsaac::GetCOlRect()
{
	RECT rc = {
		int(m_tInfo.fX - m_tInfo.fCX / 4.f),
		int(m_tInfo.fY - m_tInfo.fCY / 4.f),
		int(m_tInfo.fX + m_tInfo.fCX / 4.f),
		int(m_tInfo.fY + m_tInfo.fCY / 4.f)};

	return rc;
}

void CIsaac::BoolReset()
{
	m_bACT_hanging		= false;
	m_bACT_AllAttack	= false;
	m_bACT_Meteo		= false;
	m_bACT_Teleport		= false;

}
