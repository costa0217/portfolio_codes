#include "StdAfx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Mouse.h"
#include "Tower.h"
#include "UI.h"
#include "TowerWnd1.h"
#include "TowerWnd2.h"
#include "TowerWnd3.h"
#include "TowerWnd4.h"
#include "TowerWnd5.h"

#include "Soldier.h"
#include "Monster.h"
#include "FootMan.h"
#include "Golem.h"

#include "ArchorTower.h"
#include "BombTower.h"
#include "MagicTower.h"

#include "ObjFactory.h"
#include "UIFactory.h"
#include "ObjMgr.h"

#include "Projectile.h"
#include "Arrow.h"
#include "Bomb.h"
#include "Bullet.h"
#include "MagicBall.h"
#include "MagicBolt.h"
#include "GMisile.h"
#include "Meteo.h"
#include "Crator.h"

#include "LastWnd.h"
#include "StopWnd.h"

#include "Nunu.h"
#include "Pang.h"
#include "Jetman.h"
#include "Spider.h"
#include "Snowman.h"

#include "SDevice.h"

#define dfTowerUIRange 60
#define	dfTowerCheck 25
IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr(void)
{
}

CCollisionMgr::~CCollisionMgr(void)
{
}
bool CCollisionMgr::MouseToTowerCollision()
{
	list<CObj*>::iterator Toweriter = m_pObjList[OBJ_TOWER].begin();
	list<CObj*>::iterator TowerEnditer = m_pObjList[OBJ_TOWER].end();
	for(; Toweriter != TowerEnditer; ++Toweriter)
	{
		if(m_pObjList[OBJ_TOWERUI].empty())
		{
			if(D3DXVec3Length( &( (*Toweriter)->GetInfo().vPos - CMouse::GetPos()) ) < dfTowerCheck 
				&& ((CTower*)(*Toweriter))->GetTowerID() != TWR_EMPTY
				&& ((CTower*)(*Toweriter))->GetFreezing() )
			{
				((CTower*)(*Toweriter))->SetFreezAtkCnt(((CTower*)(*Toweriter))->GetFreezAtkCnt() + 1);
				CSDevice::GetInstance()->SoundPlay(SD_HITICE, 0);
				return false;
			}

			// 처음 짓는
			if(D3DXVec3Length( &( (*Toweriter)->GetInfo().vPos - CMouse::GetPos()) ) < dfTowerCheck 
				&& ((CTower*)(*Toweriter))->GetTowerID() == TWR_EMPTY)
			{
				CObj* pTowerWnd = CObjFactory<CTowerWnd1>::CreateObj(D3DXVECTOR3((*Toweriter)->GetInfo().vPos.x, (*Toweriter)->GetInfo().vPos.y, 0.f));
				((CTowerWnd1*)pTowerWnd)->SetTowerList(&m_pObjList[OBJ_TOWER]);
				((CTowerWnd1*)pTowerWnd)->SetSoldierList(&m_pObjList[OBJ_SOLDIER]);
				((CTowerWnd1*)pTowerWnd)->SetTower((*Toweriter));
				CObjMgr::GetInstance()->AddObject(OBJ_TOWERUI, pTowerWnd);

				CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);

			}
			// 병사 업글
			else if(D3DXVec3Length( &( (*Toweriter)->GetInfo().vPos - CMouse::GetPos()) ) < dfTowerCheck 
				&& ((CTower*)(*Toweriter))->GetTowerID() == TWR_KNIGHT1 )
			{
				if( ((CTower*)(*Toweriter))->GetLevel() < 2)
				{
					CObj* pTowerWnd2 = CObjFactory<CTowerWnd2>::CreateObj(D3DXVECTOR3((*Toweriter)->GetInfo().vPos.x, (*Toweriter)->GetInfo().vPos.y, 0.f));
					((CTowerWnd2*)pTowerWnd2)->SetTowerList(&m_pObjList[OBJ_TOWER]);
					((CTowerWnd2*)pTowerWnd2)->SetTower((*Toweriter));
					((CTowerWnd2*)pTowerWnd2)->SetVecTile(m_pVecTile);
					CObjMgr::GetInstance()->AddObject(OBJ_TOWERUI, pTowerWnd2);	

					CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);

				}
				/* 3티어 */
				else if( ((CTower*)(*Toweriter))->GetLevel() < 3 )
				{ 
					CObj* pTowerWnd4 = new CTowerWnd4;
					pTowerWnd4->SetPos(D3DXVECTOR3((*Toweriter)->GetInfo().vPos.x, (*Toweriter)->GetInfo().vPos.y, 0.f));
					((CTowerWnd4*)pTowerWnd4)->SetTowerList(&m_pObjList[OBJ_TOWER]);
					((CTowerWnd4*)pTowerWnd4)->SetTower((*Toweriter));
					((CTowerWnd4*)pTowerWnd4)->SetVecTile(m_pVecTile);
					pTowerWnd4->Initialize();
					CObjMgr::GetInstance()->AddObject(OBJ_TOWERUI, pTowerWnd4);	

					CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);
				}
				else
				{
					CObj* pTowerWnd5 = new CTowerWnd5;
					pTowerWnd5->SetPos(D3DXVECTOR3((*Toweriter)->GetInfo().vPos.x, (*Toweriter)->GetInfo().vPos.y, 0.f));
					((CTowerWnd5*)pTowerWnd5)->SetTowerList(&m_pObjList[OBJ_TOWER]);
					((CTowerWnd5*)pTowerWnd5)->SetTower((*Toweriter));
					((CTowerWnd5*)pTowerWnd5)->SetVecTile(m_pVecTile);
					pTowerWnd5->Initialize();
					CObjMgr::GetInstance()->AddObject(OBJ_TOWERUI, pTowerWnd5);	
					
					CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);
				}
			}
			// 다른거 업글 
			else if(D3DXVec3Length( &( (*Toweriter)->GetInfo().vPos - CMouse::GetPos()) ) < dfTowerCheck 
				&& ( ((CTower*)(*Toweriter))->GetTowerID() == TWR_ARCHOR1
					 || ((CTower*)(*Toweriter))->GetTowerID() == TWR_MAGIC1
					 || ((CTower*)(*Toweriter))->GetTowerID() == TWR_BOMB1) )
			{
				if( ((CTower*)(*Toweriter))->GetLevel() < 2 )
				{
					CObj* pTowerWnd3 = CObjFactory<CTowerWnd3>::CreateObj(D3DXVECTOR3((*Toweriter)->GetInfo().vPos.x, (*Toweriter)->GetInfo().vPos.y, 0.f));
					((CTowerWnd3*)pTowerWnd3)->SetTowerList(&m_pObjList[OBJ_TOWER]);
					((CTowerWnd3*)pTowerWnd3)->SetTower((*Toweriter));
					CObjMgr::GetInstance()->AddObject(OBJ_TOWERUI, pTowerWnd3);

					CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);
				}
				/* 3티어 */
				else if( ((CTower*)(*Toweriter))->GetLevel() < 3 )
				{
					CObj* pTowerWnd4 = new CTowerWnd4;
					pTowerWnd4->SetPos(D3DXVECTOR3((*Toweriter)->GetInfo().vPos.x, (*Toweriter)->GetInfo().vPos.y, 0.f));
					((CTowerWnd4*)pTowerWnd4)->SetTowerList(&m_pObjList[OBJ_TOWER]);
					((CTowerWnd4*)pTowerWnd4)->SetTower((*Toweriter));
					((CTowerWnd4*)pTowerWnd4)->SetVecTile(m_pVecTile);
					pTowerWnd4->Initialize();
					CObjMgr::GetInstance()->AddObject(OBJ_TOWERUI, pTowerWnd4);

					CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);
				}
				else
				{
					CObj* pTowerWnd5 = new CTowerWnd5;
					pTowerWnd5->SetPos(D3DXVECTOR3((*Toweriter)->GetInfo().vPos.x, (*Toweriter)->GetInfo().vPos.y, 0.f));
					((CTowerWnd5*)pTowerWnd5)->SetTowerList(&m_pObjList[OBJ_TOWER]);
					((CTowerWnd5*)pTowerWnd5)->SetTower((*Toweriter));
					((CTowerWnd5*)pTowerWnd5)->SetVecTile(m_pVecTile);
					pTowerWnd5->Initialize();
					CObjMgr::GetInstance()->AddObject(OBJ_TOWERUI, pTowerWnd5);	

					CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);
				}
				((CTower*)(*Toweriter))->SetTowerClick(true);
			}
		}
	}
	return false;
}
void CCollisionMgr::TowerWndClear()
{
	list<CObj*>::iterator TowerUIiter = m_pObjList[OBJ_TOWERUI].begin();
	list<CObj*>::iterator TowerUIEnditer = m_pObjList[OBJ_TOWERUI].end();

	

	if(!m_pObjList[OBJ_TOWERUI].empty())
	{
		// relly 찍을때 유아이 남아있게끔
		if( ((CTowerWnd2*)(*TowerUIiter))->GetRelly() == true 
			||  ((CTowerWnd4*)(*TowerUIiter))->GetRelly() == true
			||  ((CTowerWnd5*)(*TowerUIiter))->GetRelly() == true)
			return;

		if(D3DXVec3Length( &( (*TowerUIiter)->GetInfo().vPos - CMouse::GetPos()) ) < dfTowerUIRange )
			return;
	}
	list<CObj*>::iterator Toweriter = m_pObjList[OBJ_TOWER].begin();
	list<CObj*>::iterator TowerEnditer = m_pObjList[OBJ_TOWER].end();

	for(; Toweriter != TowerEnditer; ++Toweriter)
	{
		((CTower*)(*Toweriter))->SetTowerClick(false);
	}

	for(; TowerUIiter != TowerUIEnditer; ++TowerUIiter)
	{
		::Safe_Delete((*TowerUIiter));
	}
	m_pObjList[OBJ_TOWERUI].clear();
}


void CCollisionMgr::SoldierToMonster()
{
	list<CObj*>::iterator Soliter = m_pObjList[OBJ_SOLDIER].begin();
	list<CObj*>::iterator SolEnditer = m_pObjList[OBJ_SOLDIER].end();
	for(; Soliter != SolEnditer; ++Soliter)
	{
		for(list<CObj*>::iterator Moniter = m_pObjList[OBJ_MONSTER].begin();
			Moniter != m_pObjList[OBJ_MONSTER].end(); ++Moniter)
		{
			if( ((CSoldier*)(*Soliter))->GetSoliderID() == SOL_FOOTMAN 
				&& ((CMonster*)(*Moniter))->GetMonType() != MTYPE_FLY)
			{
				D3DXVECTOR3 vDistance = (*Moniter)->GetInfo().vPos - (*Soliter)->GetInfo().vPos;
				if( ((CFootMan*)(*Soliter))->GetStateKey() == L"Stand"
					&& !((CMonster*)(*Moniter))->GetBattle() /*== false*/
					&& !((CFootMan*)(*Soliter))->GetBattleMode() /*== false*/
					&& !((CMonster*)(*Moniter))->GetMonsterDie()
					&& !((CFootMan*)(*Soliter))->GetDie()
					&& D3DXVec3Length(&vDistance) < 40 )
				{
					((CMonster*)(*Moniter))->SetBattle(true);
					((CFootMan*)(*Soliter))->SetBattleMode(true);
					((CMonster*)(*Moniter))->SetBattleSoldier(*Soliter);
					((CFootMan*)(*Soliter))->SetBattleMonster(*Moniter); 
				}
			}
			else if( ((CSoldier*)(*Soliter))->GetSoliderID() == SOL_GOLEM 
				&& ((CMonster*)(*Moniter))->GetMonType() != MTYPE_FLY)
			{
				D3DXVECTOR3 vDistance = (*Moniter)->GetInfo().vPos - (*Soliter)->GetInfo().vPos;
				if( ((CGolem*)(*Soliter))->GetStateKey() == L"S_Stand"
					&& !((CMonster*)(*Moniter))->GetBattle() /*== false*/
					&& !((CGolem*)(*Soliter))->GetBattleMode() /*== false*/
					&& !((CMonster*)(*Moniter))->GetMonsterDie()
					&& !((CGolem*)(*Soliter))->GetDie()
					&& D3DXVec3Length(&vDistance) < 50 
					&& ((CGolem*)(*Soliter))->GetBattleMonster() == NULL)
				{
					
					((CMonster*)(*Moniter))->SetBattle(true);
					((CGolem*)(*Soliter))->SetBattleMode(true);
					((CMonster*)(*Moniter))->SetBattleSoldier(*Soliter);
					((CGolem*)(*Soliter))->SetBattleMonster(*Moniter); 
				}
			}
		}
	}
}
void CCollisionMgr::ArchorTowerToMonster()
{
	list<CObj*>::iterator TWRiter = m_pObjList[OBJ_TOWER].begin();
	list<CObj*>::iterator TWREnditer = m_pObjList[OBJ_TOWER].end();

	for(; TWRiter != TWREnditer; ++TWRiter)
	{
		for(list<CObj*>::iterator Moniter = m_pObjList[OBJ_MONSTER].begin();
			Moniter != m_pObjList[OBJ_MONSTER].end(); ++Moniter)
		{
			if( ((CTower*)(*TWRiter))->GetTowerID() == TWR_ARCHOR1)
			{
				float fDisX	 = (*TWRiter)->GetInfo().vPos.x - (*Moniter)->GetInfo().vPos.x;
				float fDisY	 = (*TWRiter)->GetInfo().vPos.y - (*Moniter)->GetInfo().vPos.y;
				float fRange = ((CTower*)(*TWRiter))->GetTowerStats().iRange / 2.f;

				if( (pow(fDisX, 2) / pow(fRange, 2)) + (pow(fDisY, 2) / pow(fRange/ 1.3f , 2)) < 1.f )
				{
					//아처타워의 사정거리에 닿았을때
					if( !((CMonster*)(*Moniter))->GetMonsterDie()
						&& !((CArchorTower*)(*TWRiter))->GetArchorLockOn() 
						&& ((CArchorTower*)(*TWRiter))->GetLockOnMonster() == NULL )
					{
						((CArchorTower*)(*TWRiter))->SetArchorLockOn(true);
						((CArchorTower*)(*TWRiter))->SetLockOnMonster((*Moniter));
					}
					// 인탱글 범위
					if( !((CMonster*)(*Moniter))->GetMonsterDie() 
						&& ((CMonster*)(*Moniter))->GetMonType() != MTYPE_FLY
						&& ((CArchorTower*)(*TWRiter))->GetIntengle())
					{
						//인탱글 주기
						if(	((CArchorTower*)(*TWRiter))->GetIntengleTime() > 3.f )
						{
							((CMonster*)(*Moniter))->SetIntengling(true);
						}
					}
				}
				else if( ((CArchorTower*)(*TWRiter))->GetArchorLockOn()
					&& ((CArchorTower*)(*TWRiter))->GetLockOnMonster() == (*Moniter) )
				{
					((CArchorTower*)(*TWRiter))->SetArchorLockOn(false);
					((CArchorTower*)(*TWRiter))->SetLockOnMonster(NULL);
				}
			}
			else if( ((CTower*)(*TWRiter))->GetTowerID() == TWR_BOMB1
				&& ((CMonster*)(*Moniter))->GetMonType() != MTYPE_FLY)
			{
				float fDisX	 = (*TWRiter)->GetInfo().vPos.x - (*Moniter)->GetInfo().vPos.x;
				float fDisY	 = (*TWRiter)->GetInfo().vPos.y - (*Moniter)->GetInfo().vPos.y;
				float fRange = ((CTower*)(*TWRiter))->GetTowerStats().iRange / 2.f;

				if( (pow(fDisX, 2) / pow(fRange, 2)) + (pow(fDisY, 2) / pow(fRange/ 1.3f , 2)) < 1.f )
				{
					//폭탄타워의 사정거리에 닿았을때
					if( !((CMonster*)(*Moniter))->GetMonsterDie()
						&& !((CBombTower*)(*TWRiter))->GetBombLockOn() 
						&& ((CBombTower*)(*TWRiter))->GetLockOnMonster() == NULL )
					{
						((CBombTower*)(*TWRiter))->SetBombLockOn(true);
						((CBombTower*)(*TWRiter))->SetLockOnMonster((*Moniter));
					}
				}
				else if( ((CBombTower*)(*TWRiter))->GetBombLockOn()
					&& ((CBombTower*)(*TWRiter))->GetLockOnMonster() == (*Moniter) )
				{
					((CBombTower*)(*TWRiter))->SetBombLockOn(false);
					((CBombTower*)(*TWRiter))->SetLockOnMonster(NULL);
				}
			}
			else if( ((CTower*)(*TWRiter))->GetTowerID() == TWR_MAGIC1)
			{
				float fDisX	 = (*TWRiter)->GetInfo().vPos.x - (*Moniter)->GetInfo().vPos.x;
				float fDisY	 = (*TWRiter)->GetInfo().vPos.y - (*Moniter)->GetInfo().vPos.y;
				float fRange = ((CTower*)(*TWRiter))->GetTowerStats().iRange / 2.f;

				if( (pow(fDisX, 2) / pow(fRange, 2)) + (pow(fDisY, 2) / pow(fRange/ 1.3f , 2)) < 1.f )
				{
					//마법타워의 사정거리에 닿았을때
					if( !((CMonster*)(*Moniter))->GetMonsterDie()
						&& !((CMagicTower*)(*TWRiter))->GetMagicLockOn() 
						&& ((CMagicTower*)(*TWRiter))->GetLockOnMonster() == NULL )
					{
						((CMagicTower*)(*TWRiter))->SetMagicLockOn(true);
						((CMagicTower*)(*TWRiter))->SetLockOnMonster((*Moniter));
					}
				}
				else if( ((CMagicTower*)(*TWRiter))->GetMagicLockOn()
					&& ((CMagicTower*)(*TWRiter))->GetLockOnMonster() == (*Moniter) )
				{
					((CMagicTower*)(*TWRiter))->SetMagicLockOn(false);
					((CMagicTower*)(*TWRiter))->SetLockOnMonster(NULL);
				}
			}
		}
		if( ((CTower*)(*TWRiter))->GetTowerID() == TWR_ARCHOR1
			&& ((CArchorTower*)(*TWRiter))->GetIntengle() 
			&& ((CArchorTower*)(*TWRiter))->GetIntengleTime() > 3.f )	//인탱글 주기
		{
			((CArchorTower*)(*TWRiter))->SetIntengleTime(0.f);
		}
	}
}
void CCollisionMgr::ArrowToMonster()
{
	list<CObj*>::iterator Proiter = m_pObjList[OBJ_PROJECTILE].begin();
	list<CObj*>::iterator ProEnditer = m_pObjList[OBJ_PROJECTILE].end();

	for( ; Proiter != ProEnditer; ++Proiter)
	{
		for(list<CObj*>::iterator Moniter = m_pObjList[OBJ_MONSTER].begin();
				Moniter != m_pObjList[OBJ_MONSTER].end(); ++Moniter)
		{
			if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_ARROW
				&& ((CArrow*)(*Proiter))->GetMonTarget() == (*Moniter)	)
			{
				if( D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < 15 )
				{
					((CArrow*)(*Proiter))->SetHit(true);
				}
			}
			else if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_BULLET
				&& ((CBullet*)(*Proiter))->GetMonTarget() == (*Moniter)	)
			{
				if( D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < 15 )
				{
					((CBullet*)(*Proiter))->SetHit(true);
				}
			}
			else if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_MBALL
				&& ((CMagicBall*)(*Proiter))->GetMonTarget() == (*Moniter)	)
			{
				if( D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < 15 )
				{
					((CMagicBall*)(*Proiter))->SetHit(true);
				}
			}
			else if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_MBOLT
				&& ((CMagicBolt*)(*Proiter))->GetMonTarget() == (*Moniter)	)
			{
				if( D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < 15 )
				{
					((CMagicBolt*)(*Proiter))->SetHit(true);
				}
			}

			if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_BOMB )
			{
				if( ((CBomb*)(*Proiter))->GetBoom() 
					&& D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < ((CBomb*)(*Proiter))->GetBoomRange() )
				{
					if( !((CBomb*)(*Proiter))->GetFirstBooom() )
					{
						int iDamage = ((CBomb*)(*Proiter))->GetBombDamage();
						((CMonster*)(*Moniter))->SetMonsterHP(((CMonster*)(*Moniter))->GetMonsterHP() - iDamage );
					}					
				}
			}
			else if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_GMISILE )
			{
				if( ((CGMisile*)(*Proiter))->GetBoom() 
					&& D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < ((CGMisile*)(*Proiter))->GetBoomRange() )
				{
					if( !((CGMisile*)(*Proiter))->GetFirstBooom() )
					{
						int iDamage = ((CGMisile*)(*Proiter))->GetBombDamage();
						((CMonster*)(*Moniter))->SetMonsterHP(((CMonster*)(*Moniter))->GetMonsterHP() - iDamage );
					}
				}
			}
			else if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_METEO )
			{
				if( ((CMeteo*)(*Proiter))->GetBoom() 
					&& D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < ((CMeteo*)(*Proiter))->GetBoomRange() )
				{
					if(  !((CMeteo*)(*Proiter))->GetFirstBooom() )
					{
						int iDamage = ((CMeteo*)(*Proiter))->GetBoomDamage();
						((CMonster*)(*Moniter))->SetMonsterHP(((CMonster*)(*Moniter))->GetMonsterHP() - iDamage );
					}
				}
			}
			else if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_CRATOR )
			{
				if( D3DXVec3Length( & ((*Moniter)->GetInfo().vPos - (*Proiter)->GetInfo().vPos)) < 20)
				{
					if( ((CCrator*)(*Proiter))->GetOneCircle() )
					{
						int iDamage = ((CCrator*)(*Proiter))->GetDamage();
						((CMonster*)(*Moniter))->SetMonsterHP(((CMonster*)(*Moniter))->GetMonsterHP() - iDamage );
					}
				}
			}
		}
		if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_BOMB )
		{
			if(((CBomb*)(*Proiter))->GetBoom())
			{
				((CBomb*)(*Proiter))->SetFirstBooom(true);
			}
		}
		if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_GMISILE )
		{
			if(((CGMisile*)(*Proiter))->GetBoom())
			{
				((CGMisile*)(*Proiter))->SetFirstBooom(true);
			}
		}
		if( ((CProjectile*)(*Proiter))->GetProjectileID() == PRO_METEO )
		{
			if(((CMeteo*)(*Proiter))->GetBoom())
			{
				((CMeteo*)(*Proiter))->SetFirstBooom(true);
			}
		}
	}
}
void CCollisionMgr::MouseToUI()
{
	list<CObj*>::iterator Uiter		= m_pObjList[OBJ_UI].begin();
	list<CObj*>::iterator EndUiter	= m_pObjList[OBJ_UI].end();

	for( ; Uiter != EndUiter; ++Uiter )
	{
		D3DXVECTOR3  vUIpos = (*Uiter)->GetInfo().vPos;
		if( ((CUI*)(*Uiter))->GetUIType() == UI_SKILLBUTT)
		{
			if( CMouse::GetPos().y < vUIpos.y + 15.f
				&& CMouse::GetPos().y > vUIpos.y - 22.f
				&& CMouse::GetPos().x > vUIpos.x - 15.f
				&& CMouse::GetPos().x < vUIpos.x + 15.f)
			{
				((CUI*)(*Uiter))->SetClick(true);
			}
			else
			{
				((CUI*)(*Uiter))->SetClick(false);
			}
		}
		else if( ((CUI*)(*Uiter))->GetUIType() == UI_STOPBUTT)
		{
			if( !((CUI*)(*Uiter))->GetClick()
				&& CMouse::GetPos().y < vUIpos.y + 18.f
				&& CMouse::GetPos().y > vUIpos.y - 18.f
				&& CMouse::GetPos().x > vUIpos.x - 19.f
				&& CMouse::GetPos().x < vUIpos.x + 19.f)
			{
				((CUI*)(*Uiter))->SetMouseState(2);
				((CUI*)(*Uiter))->SetClick(true);
			}
		}
		else if( ((CUI*)(*Uiter))->GetUIType() == UI_OPTIONBUTT)
		{
			if( !((CUI*)(*Uiter))->GetClick()
				&& CMouse::GetPos().y < vUIpos.y + 18.f
				&& CMouse::GetPos().y > vUIpos.y - 18.f
				&& CMouse::GetPos().x > vUIpos.x - 19.f
				&& CMouse::GetPos().x < vUIpos.x + 19.f)
			{
				((CUI*)(*Uiter))->SetClick(true);
			}
		}
		else if( ((CUI*)(*Uiter))->GetUIType() == UI_STARTBUTT)
		{
			if( !((CUI*)(*Uiter))->GetClick()
				&& CMouse::GetPos().y < vUIpos.y + 18.f
				&& CMouse::GetPos().y > vUIpos.y - 18.f
				&& CMouse::GetPos().x > vUIpos.x - 16.f
				&& CMouse::GetPos().x < vUIpos.x + 16.f)
			{
				((CUI*)(*Uiter))->SetClick(true);
			}
		}
	}
}
void CCollisionMgr::MouseToWnd()
{
	list<CObj*>::iterator Liter		= m_pObjList[OBJ_LASTWND].begin();
	list<CObj*>::iterator EndLiter	= m_pObjList[OBJ_LASTWND].end();

	for( ; Liter != EndLiter; ++Liter )
	{
		D3DXVECTOR3  vWNDpos = (*Liter)->GetInfo().vPos;
		if( ((CLastWnd*)(*Liter))->GetWndType() == WND_STOP)
		{
			if( ((CLastWnd*)(*Liter))->GetClick()
				&& CMouse::GetPos().y < vWNDpos.y + 212.f
				&& CMouse::GetPos().y > vWNDpos.y - 212.f
				&& CMouse::GetPos().x > vWNDpos.x - 175.f
				&& CMouse::GetPos().x < vWNDpos.x + 175.f)
			{
				((CLastWnd*)(*Liter))->SetClick(true);
			}
			else
			{
				((CLastWnd*)(*Liter))->SetClick(false);
			}
			
		}
		else if( ((CLastWnd*)(*Liter))->GetWndType() == WND_OPTION)
		{
			if( ((CLastWnd*)(*Liter))->GetClick()
				&& CMouse::GetPos().y < vWNDpos.y + 212.f
				&& CMouse::GetPos().y > vWNDpos.y - 212.f
				&& CMouse::GetPos().x > vWNDpos.x - 175.f
				&& CMouse::GetPos().x < vWNDpos.x + 175.f)
			{
				((CLastWnd*)(*Liter))->SetClick(true);
			}
			else
			{
				((CLastWnd*)(*Liter))->SetClick(false);
			}
			
		}
	}
}
void CCollisionMgr::BossTowerFreezing()
{
	list<CObj*>::iterator Miter		= m_pObjList[OBJ_MONSTER].begin();
	list<CObj*>::iterator EndMiter	= m_pObjList[OBJ_MONSTER].end();

	for( ; Miter != EndMiter; ++Miter)
	{
		for(list<CObj*>::iterator Titer = m_pObjList[OBJ_TOWER].begin();
			Titer != m_pObjList[OBJ_TOWER].end(); ++Titer)
		{
			if( ((CMonster*)(*Miter))->GetMonsterID() == MON_SNOWMAN 
				&& ((CSnowman*)(*Miter))->GetFreezingSkill()
				&& D3DXVec3Length( & ( (*Miter)->GetInfo().vPos - (*Titer)->GetInfo().vPos)) < 150.f )
			{
				((CTower*)(*Titer))->SetFreezing(true);
			}
		}
	}
}