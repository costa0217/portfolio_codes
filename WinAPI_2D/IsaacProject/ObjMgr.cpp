#include "StdAfx.h"
#include "ObjMgr.h"
#include "Isaac.h"
#include "PlayerBullet.h"
#include "MonBullet.h"
#include "BossSkill.h"
#include "Bomb.h"
#include "Obj.h"
#include "Stage1.h"
#include "SceneMgr.h"

#include "Fly.h"
#include "Gaper.h"
#include "Meat.h"
#include "Mulgun.h"
#include "Hopper.h"
#include "MiniBoss.h"
#include "MiniBoss2.h"
#include "MiniBoss4.h"
#include "Boss.h"

#include "Door.h"
#include "Poop.h"
#include "Fire.h"
#include "Stone.h"

#include "Item.h"
#include "Arrow.h"
#include "Active.h"

#include "PlayerKnife.h"
#include "PlayerSnake.h"
#include "SkillAll.h"
#include "SkillMeteo.h"
#include "SkillLemon.h"

#define Push_Speed 7

CObjMgr*	CObjMgr::m_pInst = NULL;

CObjMgr::CObjMgr(void)
:m_bNextRoom(false)
{
	
}

CObjMgr::~CObjMgr(void)
{
}
void CObjMgr::SetObjList(list<CObj*>*  pObjList)
{
	m_pObjList = pObjList;
}
void CObjMgr::SetTileVector(vector<TILE*>*  pVecTile)
{
	m_pTileVec = pVecTile;
}

void CObjMgr::PlayerTileCollision()
{
	RECT	rc;

	list<CObj*>::iterator iterEndObj	= m_pObjList[OBJ_PLAYER].end();


	for(list<CObj*>::iterator iterTile = m_pObjList[OBJ_TILE].begin();
		iterTile != m_pObjList[OBJ_TILE].end(); ++iterTile)
	{
		for(list<CObj*>::iterator iterObj = m_pObjList[OBJ_PLAYER].begin();
			iterObj != iterEndObj; ++iterObj)
		{
			if((*iterTile)->GetDrawID() ==  TILE_DOOR)	//==========문 충돌==========
			{
				if( ((CDoor*)(*iterTile))->GetOpenDoor() == true)
				{
				if( IntersectRect(&rc, &((CIsaac*)(*iterObj))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
				{
					m_bNextRoom = true;
					if(((CIsaac*)(*iterObj))->GetRoomNum() == 1)		// 1번방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 60)
						{
							(*iterObj)->SetPos(700.f, 350.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage2);
							return;
						}
						else if(((CBlock*)(*iterTile))->GetIndex() == 74)
						{
							(*iterObj)->SetPos(100.f, 350.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage5);
							return;
						}
					}
					else if(((CIsaac*)(*iterObj))->GetRoomNum() == 2)	// 2번방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 7)
						{
							(*iterObj)->SetPos(400.f, 500.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage3);
							return;
						}
						else if(((CBlock*)(*iterTile))->GetIndex() == 74)
						{
							(*iterObj)->SetPos(100.f, 350.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage1);
							return;
						}
						else if(((CBlock*)(*iterTile))->GetIndex() == 127)
						{
							(*iterObj)->SetPos(400.f, 200.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage4);
							return;
						}
					}
					else if(((CIsaac*)(*iterObj))->GetRoomNum() == 3)	// 3번방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 127)
						{
							(*iterObj)->SetPos(400.f, 200.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage2);
							return;
						}
					}

					else if(((CIsaac*)(*iterObj))->GetRoomNum() == 4)	// 4번방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 7)
						{
							(*iterObj)->SetPos(400.f, 500.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage2);
							return;
						}
					}
					else if(((CIsaac*)(*iterObj))->GetRoomNum() == 5)	// 5번방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 7)
						{
							(*iterObj)->SetPos(400.f, 500.f);
							CSceneMgr::GetInst()->SetScene(SC_BossStage);
							return;
						}
						else if(((CBlock*)(*iterTile))->GetIndex() == 60)
						{
							(*iterObj)->SetPos(700.f, 350.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage1);
							return;
						}
						else if(((CBlock*)(*iterTile))->GetIndex() == 74)
						{
							(*iterObj)->SetPos(100.f, 350.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage7);
							return;
						}
						else if(((CBlock*)(*iterTile))->GetIndex() == 127)
						{
							(*iterObj)->SetPos(400.f, 200.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage6);
							return;
						}
					}
					else if(((CIsaac*)(*iterObj))->GetRoomNum() == 6)	// 6번방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 7)
						{
							(*iterObj)->SetPos(400.f, 500.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage5);
							return;
						}
					}
					else if(((CIsaac*)(*iterObj))->GetRoomNum() == 7)	// 7번방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 60)
						{
							(*iterObj)->SetPos(700.f, 350.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage5);
							return;
						}
					}
					else if(((CIsaac*)(*iterObj))->GetRoomNum() == 8)	// 보스방
					{
						if(((CBlock*)(*iterTile))->GetIndex() == 127)
						{
							(*iterObj)->SetPos(400.f, 200.f);
							CSceneMgr::GetInst()->SetScene(SC_Stage5);
							return;
						}
					}
					}
				}
			}
			if( (*iterTile)->GetDrawID() !=  TILE_ROAD && (*iterTile)->GetDrawID() !=  TILE_FIRE )
			{
				if( IntersectRect(&rc, &((CIsaac*)(*iterObj))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
				{
					SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);

					if(!((CIsaac*)(*iterObj))->GetHaning())
					{
						if((*iterTile)->GetDrawID() ==  TILE_DOOR)	//==========문 충돌==========
						{
							if( ((CDoor*)(*iterTile))->GetKeyOpenDoor() == true)
							{
								if(((CIsaac*)(*iterObj))->GetKeyNum() > 0 )
								{
									if( ((CDoor*)(*iterTile))->GetiStageNum() == 2 && !((CDoor*)(*iterTile))->GetKeyInsert() )
									{
										((CIsaac*)(*iterObj))->SetKeyNum(((CIsaac*)(*iterObj))->GetKeyNum() - 1);
										((CDoor*)(*iterTile))->SetKeyInsert(true);
									}
								}
							}
						}
						if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
						{
							if( (*iterTile)->GetInfo().fY > (*iterObj)->GetInfo().fY )		// 위
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX,
									(*iterObj)->GetInfo().fY - rc.bottom);		
							}
							else	// 아래
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX,
									(*iterObj)->GetInfo().fY + rc.bottom);	

							}
						}
						else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
						{				
							if( (*iterTile)->GetInfo().fX < (*iterObj)->GetInfo().fX )		// 오른
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX + rc.right,
									(*iterObj)->GetInfo().fY);							
							}
							else	// 왼
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX - rc.right,
									(*iterObj)->GetInfo().fY);								
							}
						}
					}
					else
					{
						if((*iterTile)->GetDrawID() ==  TILE_BLOCK)
						{
													if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
						{
							if( (*iterTile)->GetInfo().fY > (*iterObj)->GetInfo().fY )		// 위
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX,
									(*iterObj)->GetInfo().fY - rc.bottom);		
							}
							else	// 아래
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX,
									(*iterObj)->GetInfo().fY + rc.bottom);	

							}
						}
						else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
						{				
							if( (*iterTile)->GetInfo().fX < (*iterObj)->GetInfo().fX )		// 오른
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX + rc.right,
									(*iterObj)->GetInfo().fY);							
							}
							else	// 왼
							{
								(*iterObj)->SetPos( (*iterObj)->GetInfo().fX - rc.right,
									(*iterObj)->GetInfo().fY);								
							}
						}

						}
					}
				}
			}
			else if( (*iterTile)->GetDrawID() ==  TILE_FIRE)
			{
				if(!((CIsaac*)(*iterObj))->GetOnHit())
				{
					if( IntersectRect(&rc, &((CIsaac*)(*iterObj))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
					{				
						if(((CIsaac*)(*iterObj))->GetIsaacDir() == DIR_LEFT)
						{
							((CIsaac*)(*iterObj))->SetSpeedX(Push_Speed);
							((CIsaac*)(*iterObj))->SetOnHit(true);
						}
						else if(((CIsaac*)(*iterObj))->GetIsaacDir() == DIR_RIGHT)
						{
							((CIsaac*)(*iterObj))->SetSpeedX(-Push_Speed);
							((CIsaac*)(*iterObj))->SetOnHit(true);
						}
						else if(((CIsaac*)(*iterObj))->GetIsaacDir() == DIR_UP)
						{
							((CIsaac*)(*iterObj))->SetSpeedY(Push_Speed);
							((CIsaac*)(*iterObj))->SetOnHit(true);
						}
						else if(((CIsaac*)(*iterObj))->GetIsaacDir() == DIR_DOWN)
						{
							((CIsaac*)(*iterObj))->SetSpeedY(-Push_Speed);
							((CIsaac*)(*iterObj))->SetOnHit(true);
						}
						((CIsaac*)(*iterObj))->SetOnHit(true);				
					}
				}
			}
		}
	}
}

void CObjMgr::PlayerMonsterCollision()
{
	RECT	rc;

	list<CObj*>::iterator iterEndObj	= m_pObjList[OBJ_PLAYER].end();


	for(list<CObj*>::iterator iterMonster = m_pObjList[OBJ_MONSTER].begin();
		iterMonster != m_pObjList[OBJ_MONSTER].end(); ++iterMonster)
	{
		for(list<CObj*>::iterator iterObj = m_pObjList[OBJ_PLAYER].begin();
			iterObj != iterEndObj; ++iterObj)
		{
			if(!((CIsaac*)(*iterObj))->GetOnHit())
			{
				if( IntersectRect(&rc, &((CIsaac*)(*iterObj))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
				{
					((CIsaac*)(*iterObj))->SetOnHit(true);

					SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);
					if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
					{
						if( (*iterMonster)->GetInfo().fY > (*iterObj)->GetInfo().fY )		// 위
						{
							((CIsaac*)(*iterObj))->SetSpeedY(-Push_Speed);
						}
						else	// 아래
						{
							((CIsaac*)(*iterObj))->SetSpeedY(Push_Speed);
						}
					}
					else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
					{				
						if( (*iterMonster)->GetInfo().fX < (*iterObj)->GetInfo().fX )		// 오른
						{
							((CIsaac*)(*iterObj))->SetSpeedX(Push_Speed);
						}
						else	// 왼
						{
;							((CIsaac*)(*iterObj))->SetSpeedX(-Push_Speed);		
						}
					}
				}
			}
		}
	}
}
void CObjMgr::PlayerBulletCollision()
{
	RECT	rc;

	for(list<CObj*>::iterator iterBullet = m_pObjList[OBJ_BULLET].begin();
		iterBullet != m_pObjList[OBJ_BULLET].end(); ++iterBullet)
	{
		for(list<CObj*>::iterator iterObj = m_pObjList[OBJ_PLAYER].begin();
			iterObj !=  m_pObjList[OBJ_PLAYER].end(); ++iterObj)
		{
			if(!((CIsaac*)(*iterObj))->GetOnHit())
			{
				if(((CPlayerBullet*)(*iterBullet))->GetBulletID() != BUL_PLAYER && ((CMonBullet*)(*iterBullet))->GetBulletID() != BUL_BOSSSKILL 
					&& ((CBullet*)(*iterBullet))->GetBulletID() != BUL_SKILLLEMON)
				{
					if( IntersectRect(&rc, &((CIsaac*)(*iterObj))->GetCOlRect(), &((CMonBullet*)(*iterBullet))->GetCOlRect()))
					{
						((CIsaac*)(*iterObj))->SetOnHit(true);

						SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);
						if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
						{
							if( (*iterBullet)->GetInfo().fY > (*iterObj)->GetInfo().fY )		// 위
							{
								((CIsaac*)(*iterObj))->SetSpeedY(-6);
							}
							else	// 아래
							{
								((CIsaac*)(*iterObj))->SetSpeedY(6);
							}
						}
						else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
						{				
							if( (*iterBullet)->GetInfo().fX < (*iterObj)->GetInfo().fX )		// 오른
							{
								((CIsaac*)(*iterObj))->SetSpeedX(6);
							}
							else	// 왼
							{
								((CIsaac*)(*iterObj))->SetSpeedX(-6);		
							}
						}
					}
				}
				/*보스 스킬시전*/
				else if(((CMonBullet*)(*iterBullet))->GetBulletID() == BUL_BOSSSKILL )
				{
					if( IntersectRect(&rc, &((CIsaac*)(*iterObj))->GetCOlRect(), &((CBossSkill*)(*iterBullet))->GetCOlRect() ))
					{
						((CIsaac*)(*iterObj))->SetOnHit(true);

						SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);
						if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
						{
							if( (*iterBullet)->GetInfo().fY > (*iterObj)->GetInfo().fY )		// 위
							{
								((CIsaac*)(*iterObj))->SetSpeedY(-6);
							}
							else	// 아래
							{
								((CIsaac*)(*iterObj))->SetSpeedY(6);
							}
						}
						else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
						{				
							if( (*iterBullet)->GetInfo().fX < (*iterObj)->GetInfo().fX )		// 오른
							{
								((CIsaac*)(*iterObj))->SetSpeedX(6);
							}
							else	// 왼
							{
								((CIsaac*)(*iterObj))->SetSpeedX(-6);		
							}
						}
					}
				}
			}
		}
	}
}

void CObjMgr::BulletMonsterCollision()
{
	RECT	rc;

	for(list<CObj*>::iterator iterMonster = m_pObjList[OBJ_MONSTER].begin();
		iterMonster != m_pObjList[OBJ_MONSTER].end(); ++iterMonster)
	{
		for(list<CObj*>::iterator iterBullet = m_pObjList[OBJ_BULLET].begin();
			iterBullet != m_pObjList[OBJ_BULLET].end(); ++iterBullet)
		{
			if(((CBullet*)(*iterBullet))->GetBulletID() != BUL_MONSTER && ((CBullet*)(*iterBullet))->GetBulletID() != BUL_BOSSSKILL)	/*몬스터 총알이 아닌경우만*/
			{
				if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYER)
				{
					if( IntersectRect(&rc, &((CPlayerBullet*)(*iterBullet))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
					{
						((CPlayerBullet*)(*iterBullet))->SetPoof(true);

						if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_FLY)
						{
							((CFly*)(*iterMonster))->SetHP(((CFly*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CFly*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_GAPER)
						{
							((CGaper*)(*iterMonster))->SetHP(((CGaper*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CGaper*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MEAT)
						{
							((CMeat*)(*iterMonster))->SetHP(((CMeat*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CMeat*)(*iterMonster))->GetHP() <= 0)
							{
								((CMeat*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MULGUN)
						{
							((CMulgun*)(*iterMonster))->SetHP(((CMulgun*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CMulgun*)(*iterMonster))->GetHP() <= 0)
							{
								((CMulgun*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_HOPPER)
						{
							((CHopper*)(*iterMonster))->SetHP(((CHopper*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CHopper*)(*iterMonster))->GetHP() <= 0)
							{
								((CHopper*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
						{
							((CMiniBoss*)(*iterMonster))->SetHP(((CMiniBoss*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CMiniBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
						{
							((CMiniBoss2*)(*iterMonster))->SetHP(((CMiniBoss2*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CMiniBoss2*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss2*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
						{
							((CMiniBoss4*)(*iterMonster))->SetHP(((CMiniBoss4*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CMiniBoss4*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss4*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_BOSS)
						{
							((CBoss*)(*iterMonster))->SetHP(((CBoss*)(*iterMonster))->GetHP() - ((CPlayerBullet*)(*iterBullet))->GetHP());
							if(((CBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
					}

				}
				else if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYERKNIFE)
				{
					if( IntersectRect(&rc, &((CPlayerKnife*)(*iterBullet))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
					{
					
						/*((CPlayerKnife*)(*iterBullet))->SetPoof(true);*/
						if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_FLY)
						{
							((CFly*)(*iterMonster))->SetHP(((CFly*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CFly*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_GAPER)
						{
							((CGaper*)(*iterMonster))->SetHP(((CGaper*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CGaper*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MEAT)
						{
							((CMeat*)(*iterMonster))->SetHP(((CMeat*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CMeat*)(*iterMonster))->GetHP() <= 0)
							{
								((CMeat*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MULGUN)
						{
							((CMulgun*)(*iterMonster))->SetHP(((CMulgun*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CMulgun*)(*iterMonster))->GetHP() <= 0)
							{
								((CMulgun*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_HOPPER)
						{
							((CHopper*)(*iterMonster))->SetHP(((CHopper*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CHopper*)(*iterMonster))->GetHP() <= 0)
							{
								((CHopper*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
						{
							((CMiniBoss*)(*iterMonster))->SetHP(((CMiniBoss*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CMiniBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
						{
							((CMiniBoss2*)(*iterMonster))->SetHP(((CMiniBoss2*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CMiniBoss2*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss2*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
						{
							((CMiniBoss4*)(*iterMonster))->SetHP(((CMiniBoss4*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CMiniBoss4*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss4*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_BOSS)
						{
							((CBoss*)(*iterMonster))->SetHP(((CBoss*)(*iterMonster))->GetHP() - ((CPlayerKnife*)(*iterBullet))->GetHP());
							if(((CBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
					}

				}
				/*스네이크샷!*/
				else if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYERSNAKE)
				{
					if( IntersectRect(&rc, &((CPlayerSnake*)(*iterBullet))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
					{
						((CPlayerSnake*)(*iterBullet))->SetPoof(true);

						if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_FLY)
						{
							((CFly*)(*iterMonster))->SetHP(((CFly*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CFly*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_GAPER)
						{
							((CGaper*)(*iterMonster))->SetHP(((CGaper*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CGaper*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MEAT)
						{
							((CMeat*)(*iterMonster))->SetHP(((CMeat*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CMeat*)(*iterMonster))->GetHP() <= 0)
							{
								((CMeat*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MULGUN)
						{
							((CMulgun*)(*iterMonster))->SetHP(((CMulgun*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CMulgun*)(*iterMonster))->GetHP() <= 0)
							{
								((CMulgun*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_HOPPER)
						{
							((CHopper*)(*iterMonster))->SetHP(((CHopper*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CHopper*)(*iterMonster))->GetHP() <= 0)
							{
								((CHopper*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
						{
							((CMiniBoss*)(*iterMonster))->SetHP(((CMiniBoss*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CMiniBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
						{
							((CMiniBoss2*)(*iterMonster))->SetHP(((CMiniBoss2*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CMiniBoss2*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss2*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
						{
							((CMiniBoss4*)(*iterMonster))->SetHP(((CMiniBoss4*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CMiniBoss4*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss4*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_BOSS)
						{
							((CBoss*)(*iterMonster))->SetHP(((CBoss*)(*iterMonster))->GetHP() - ((CPlayerSnake*)(*iterBullet))->GetHP());
							if(((CBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
					}
				}
				/*전체공격*/
				else if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_SKILLALL)
				{
					if( IntersectRect(&rc, &((CSkillAll*)(*iterBullet))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
					{
						//((CPlayerSnake*)(*iterBullet))->SetPoof(true);

						if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_FLY)
						{
							((CFly*)(*iterMonster))->SetHP(((CFly*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CFly*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_GAPER)
						{
							((CGaper*)(*iterMonster))->SetHP(((CGaper*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CGaper*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MEAT)
						{
							((CMeat*)(*iterMonster))->SetHP(((CMeat*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CMeat*)(*iterMonster))->GetHP() <= 0)
							{
								((CMeat*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MULGUN)
						{
							((CMulgun*)(*iterMonster))->SetHP(((CMulgun*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CMulgun*)(*iterMonster))->GetHP() <= 0)
							{
								((CMulgun*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_HOPPER)
						{
							((CHopper*)(*iterMonster))->SetHP(((CHopper*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CHopper*)(*iterMonster))->GetHP() <= 0)
							{
								((CHopper*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
						{
							((CMiniBoss*)(*iterMonster))->SetHP(((CMiniBoss*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CMiniBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
						{
							((CMiniBoss2*)(*iterMonster))->SetHP(((CMiniBoss2*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CMiniBoss2*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss2*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
						{
							((CMiniBoss4*)(*iterMonster))->SetHP(((CMiniBoss4*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CMiniBoss4*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss4*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_BOSS)
						{
							((CBoss*)(*iterMonster))->SetHP(((CBoss*)(*iterMonster))->GetHP() - ((CSkillAll*)(*iterBullet))->GetHP());
							if(((CBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
					}
				}
				/*메테오*/
				else if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_SKILLMETEO)
				{
					if( IntersectRect(&rc, &((CSkillMeteo*)(*iterBullet))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
					{
						//((CPlayerSnake*)(*iterBullet))->SetPoof(true);

						(*iterMonster)->SetHP(0);

					}
				}
				else if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_SKILLLEMON)
				{
					if( IntersectRect(&rc, &((CSkillLemon*)(*iterBullet))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
					{
						//((CPlayerSnake*)(*iterBullet))->SetPoof(true);

						(*iterMonster)->SetHP((*iterMonster)->GetHP() - 1);

					}
				}

				/*폭탄인경우*/
				else if( IntersectRect(&rc, &((CBomb*)(*iterBullet))->GetCOlRect(), &((CMonster*)(*iterMonster))->GetColRect()))
				{
					if(((CBomb*)(*iterBullet))->GetExplosion())
					{
						if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_FLY)
						{
							((CFly*)(*iterMonster))->SetHP(((CFly*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CFly*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_GAPER)
						{
							((CGaper*)(*iterMonster))->SetHP(((CGaper*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CGaper*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MEAT)
						{
							((CMeat*)(*iterMonster))->SetHP(((CMeat*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CMeat*)(*iterMonster))->GetHP() <= 0)
							{
								((CMonster*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MULGUN)
						{
							((CMulgun*)(*iterMonster))->SetHP(((CMulgun*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CMulgun*)(*iterMonster))->GetHP() <= 0)
							{
								((CMulgun*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_HOPPER)
						{
							((CHopper*)(*iterMonster))->SetHP(((CHopper*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CHopper*)(*iterMonster))->GetHP() <= 0)
							{
								((CHopper*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
						{
							((CMiniBoss*)(*iterMonster))->SetHP(((CMiniBoss*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CMiniBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
						{
							((CMiniBoss2*)(*iterMonster))->SetHP(((CMiniBoss2*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CMiniBoss2*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss2*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
						{
							((CMiniBoss4*)(*iterMonster))->SetHP(((CMiniBoss4*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CMiniBoss4*)(*iterMonster))->GetHP() <= 0)
							{
								((CMiniBoss4*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
						else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_BOSS)
						{
							((CBoss*)(*iterMonster))->SetHP(((CBoss*)(*iterMonster))->GetHP() - ((CBomb*)(*iterBullet))->GetHP());
							if(((CBoss*)(*iterMonster))->GetHP() <= 0)
							{
								((CBoss*)(*iterMonster))->SetColSize(0, 0); // 충돌박스 없애버림
							}
						}
					}
				}
			}
		}
	}
}
void CObjMgr::TileMonsterCollision()
{
	RECT	rc;

	//list<CObj*>::iterator iterEndObj	= m_pObjList[OBJ_PLAYER].end();

	for(list<CObj*>::iterator iterMonster = m_pObjList[OBJ_MONSTER].begin();
		iterMonster != m_pObjList[OBJ_MONSTER].end(); ++iterMonster)
	{
		for(list<CObj*>::iterator iterTile = m_pObjList[OBJ_TILE].begin();
			iterTile != m_pObjList[OBJ_TILE].end(); ++iterTile)
		{
			if( IntersectRect(&rc, &((CMonster*)(*iterMonster))->GetColRect(), &((CBlock*)(*iterTile))->GetColRect()))
			{
				if(	((CMonster*)(*iterMonster))->GetMonsterID() == MON_FLY ) //=======1.파리^^
				{
					if( (*iterTile)->GetDrawID() == TILE_BLOCK || (*iterTile)->GetDrawID() == TILE_DOOR)
					{
						SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);
						if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
						{
							if( (*iterTile)->GetInfo().fY > (*iterMonster)->GetInfo().fY )		// 위
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX,
									(*iterMonster)->GetInfo().fY - rc.bottom);		
							}
							else	// 아래
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX,
									(*iterMonster)->GetInfo().fY + rc.bottom);	
							}
						}
						else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
						{				
							if( (*iterTile)->GetInfo().fX < (*iterMonster)->GetInfo().fX )		// 오른
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX + rc.right,
									(*iterMonster)->GetInfo().fY);							
							}
							else	// 왼
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX - rc.right,
									(*iterMonster)->GetInfo().fY);								
							}
						}
					}
				}
				else if(	((CMonster*)(*iterMonster))->GetMonsterID() != MON_FLY ) 
				{
					if( (*iterTile)->GetDrawID() != TILE_ROAD)
					{
						SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);
						if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
						{
							if( (*iterTile)->GetInfo().fY > (*iterMonster)->GetInfo().fY )		// 위
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX,
									(*iterMonster)->GetInfo().fY - rc.bottom);

								if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
								{
									if(((CMiniBoss*)(*iterMonster))->GetMiniDIR() == DIR_LD)
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_LU);
									else
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_RU);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
								{
									if(((CMiniBoss2*)(*iterMonster))->GetMiniDIR() == DIR_LD)
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_LU);
									else
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_RU);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
								{
									if(((CMiniBoss4*)(*iterMonster))->GetMiniDIR() == DIR_LD)
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_LU);
									else
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_RU);
								}
							}
							else	// 아래
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX,
									(*iterMonster)->GetInfo().fY + rc.bottom);	

								if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
								{
									if(((CMiniBoss*)(*iterMonster))->GetMiniDIR() == DIR_RU)
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_RD);
									else
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_LD);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
								{
									if(((CMiniBoss2*)(*iterMonster))->GetMiniDIR() == DIR_RU)
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_RD);
									else
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_LD);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
								{
									if(((CMiniBoss4*)(*iterMonster))->GetMiniDIR() == DIR_RU)
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_RD);
									else
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_LD);
								}

							}
						}
						else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
						{				
							if( (*iterTile)->GetInfo().fX < (*iterMonster)->GetInfo().fX )		// 오른
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX + rc.right,
									(*iterMonster)->GetInfo().fY);				

								if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
								{
									if(((CMiniBoss*)(*iterMonster))->GetMiniDIR() == DIR_LU)
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_RU);
									else
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_RD);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
								{
									if(((CMiniBoss2*)(*iterMonster))->GetMiniDIR() == DIR_LU)
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_RU);
									else
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_RD);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
								{
									if(((CMiniBoss4*)(*iterMonster))->GetMiniDIR() == DIR_LU)
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_RU);
									else
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_RD);
								}
							}
							else	// 왼
							{
								(*iterMonster)->SetPos( (*iterMonster)->GetInfo().fX - rc.right,
									(*iterMonster)->GetInfo().fY);
								if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS)
								{
									if(((CMiniBoss*)(*iterMonster))->GetMiniDIR() == DIR_RD)
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_LD);
									else
										((CMiniBoss*)(*iterMonster))->SetMiniDIR(DIR_LU);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS2)
								{
									if(((CMiniBoss2*)(*iterMonster))->GetMiniDIR() == DIR_RD)
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_LD);
									else
										((CMiniBoss2*)(*iterMonster))->SetMiniDIR(DIR_LU);
								}
								else if(((CMonster*)(*iterMonster))->GetMonsterID() == MON_MINIBOSS4)
								{
									if(((CMiniBoss4*)(*iterMonster))->GetMiniDIR() == DIR_RD)
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_LD);
									else
										((CMiniBoss4*)(*iterMonster))->SetMiniDIR(DIR_LU);
								}
							}
						}
					}
				}
			}
		}
	}
}

void CObjMgr::BulletTileCollision()
{
	RECT	rc;

	list<CObj*>::iterator iterEndBullet	= m_pObjList[OBJ_BULLET].end();

	for(list<CObj*>::iterator iterTile = m_pObjList[OBJ_TILE].begin();
		iterTile != m_pObjList[OBJ_TILE].end(); ++iterTile)
	{
		for(list<CObj*>::iterator iterBullet = m_pObjList[OBJ_BULLET].begin();
			iterBullet != iterEndBullet; ++iterBullet)
		{
			if( (*iterTile)->GetDrawID() !=  TILE_ROAD )
			{
				if( ((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYER || ((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYERKNIFE
					|| ((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYERSNAKE)
				{

					if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYER)
					{
						if( IntersectRect(&rc, &((CPlayerBullet*)(*iterBullet))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
						{
							((CPlayerBullet*)(*iterBullet))->SetPoof(true);


							if( (*iterTile)->GetDrawID() ==  TILE_POOP)
							{
								((CPoop*)(*iterTile))->SetPoopCrush(true);
								if(((CPoop*)(*iterTile))->GetPoopFade())
									((CBlock*)(*iterTile))->SetTileSize(0, 0);
							}
							else if( (*iterTile)->GetDrawID() ==  TILE_FIRE)
							{
								((CFire*)(*iterTile))->SetFireFade(true);
								((CBlock*)(*iterTile))->SetTileSize(0, 0);
							}
						}
					}
					else if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYERKNIFE)
					{
						if( IntersectRect(&rc, &((CPlayerKnife*)(*iterBullet))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
						{
							if( (*iterTile)->GetDrawID() ==  TILE_POOP)
							{
								((CPoop*)(*iterTile))->SetPoopCrush(true);
								if(((CPoop*)(*iterTile))->GetPoopFade())
									((CBlock*)(*iterTile))->SetTileSize(0, 0);
							}
							else if( (*iterTile)->GetDrawID() ==  TILE_FIRE)
							{
								((CFire*)(*iterTile))->SetFireFade(true);
								((CBlock*)(*iterTile))->SetTileSize(0, 0);
							}
						}
					}
					else if(((CBullet*)(*iterBullet))->GetBulletID() == BUL_PLAYERSNAKE)
					{
						if( IntersectRect(&rc, &((CPlayerSnake*)(*iterBullet))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
						{
							((CPlayerSnake*)(*iterBullet))->SetPoof(true);
							if( (*iterTile)->GetDrawID() ==  TILE_POOP)
							{
								((CPoop*)(*iterTile))->SetPoopCrush(true);
								if(((CPoop*)(*iterTile))->GetPoopFade())
									((CBlock*)(*iterTile))->SetTileSize(0, 0);
							}
							else if( (*iterTile)->GetDrawID() ==  TILE_FIRE)
							{
								((CFire*)(*iterTile))->SetFireFade(true);
								((CBlock*)(*iterTile))->SetTileSize(0, 0);
							}
						}
					}
				}
				if( ((CBullet*)(*iterBullet))->GetBulletID() == BUL_MONSTER)
				{
					if( IntersectRect(&rc, &((CMonBullet*)(*iterBullet))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
					{
						((CMonBullet*)(*iterBullet))->SetPoof(true);

						if( (*iterTile)->GetDrawID() ==  TILE_POOP)
						{
							((CPoop*)(*iterTile))->SetPoopCrush(true);
							if(((CPoop*)(*iterTile))->GetPoopFade())
								((CBlock*)(*iterTile))->SetTileSize(0, 0);
						}
						else if( (*iterTile)->GetDrawID() ==  TILE_FIRE)
						{
							((CFire*)(*iterTile))->SetFireFade(true);
							((CBlock*)(*iterTile))->SetTileSize(0, 0);
						}
					}
				}

				if( IntersectRect(&rc, &((CBomb*)(*iterBullet))->GetCOlRect(), &((CBlock*)(*iterTile))->GetColRect()))
				{
					SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);

					//폭팔아닐때 이동
					if( !((CBomb*)(*iterBullet))->GetExplosion() && (*iterTile)->GetDrawID() != TILE_FIRE )
					{
						if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
						{
							if( (*iterTile)->GetInfo().fY > (*iterBullet)->GetInfo().fY )		// 위
							{
								(*iterBullet)->SetPos( (*iterBullet)->GetInfo().fX,
									(*iterBullet)->GetInfo().fY - rc.bottom);		
							}
							else	// 아래
							{
								(*iterBullet)->SetPos( (*iterBullet)->GetInfo().fX,
									(*iterBullet)->GetInfo().fY + rc.bottom);	

							}
						}
						else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
						{				
							if( (*iterTile)->GetInfo().fX < (*iterBullet)->GetInfo().fX )		// 오른
							{
								(*iterBullet)->SetPos( (*iterBullet)->GetInfo().fX + rc.right,
									(*iterBullet)->GetInfo().fY);							
							}
							else	// 왼
							{
								(*iterBullet)->SetPos( (*iterBullet)->GetInfo().fX - rc.right,
									(*iterBullet)->GetInfo().fY);								
							}
						}
					}
					//폭탄 폭팔
					if( ((CBomb*)(*iterBullet))->GetExplosion())
					{
						if( (*iterTile)->GetDrawID() ==  TILE_POOP)			/*똥*/
						{
							((CPoop*)(*iterTile))->SetPoopCrush(true);
							if(((CPoop*)(*iterTile))->GetPoopFade())
								((CBlock*)(*iterTile))->SetTileSize(0, 0);
						}
						else if( (*iterTile)->GetDrawID() ==  TILE_STONE)	/*돌*/
						{
							((CStone*)(*iterTile))->SetStoneCrush(true);
							if(((CStone*)(*iterTile))->GetStoneFade())
								((CBlock*)(*iterTile))->SetTileSize(0, 0);
						}
						else if( (*iterTile)->GetDrawID() ==  TILE_FIRE)	/*불*/
						{
							((CFire*)(*iterTile))->SetFireFade(true);
							((CBlock*)(*iterTile))->SetTileSize(0, 0);
						}
					}
				}
			}
		}
	}
}

void CObjMgr::BulletBombCollision()
{
	RECT	rc;
	for(list<CObj*>::iterator iterBullet = m_pObjList[OBJ_BULLET].begin();
		iterBullet != m_pObjList[OBJ_BULLET].end(); ++iterBullet)
	{

		for(list<CObj*>::iterator iterBomb = m_pObjList[OBJ_BULLET].begin();
			iterBomb != m_pObjList[OBJ_BULLET].end(); ++iterBomb)
		{
			if(((CBullet*)(*iterBullet))->GetBulletID() != BUL_MONSTER)
			{
			if( !((CBomb*)(*iterBomb))->GetExplosion() )
			{
				if( IntersectRect(&rc, &((CPlayerBullet*)(*iterBullet))->GetCOlRect(), &((CBomb*)(*iterBomb))->GetCOlRect()))
				{
					SetRect(&rc,0,0, rc.right - rc.left, rc.bottom - rc.top);
					((CPlayerBullet*)(*iterBullet))->SetPoof(true);

					if( rc.right - rc.left >= rc.bottom - rc.top )	// 위 아래
					{
						if( ((CPlayerBullet*)(*iterBullet))->GetInfo().fY > ((CBomb*)(*iterBomb))->GetInfo().fY )		// 위
						{
							((CBomb*)(*iterBomb))->SetSpeedY(-BOMB_SPEED);		
						}
						else	// 아래
						{
							((CBomb*)(*iterBomb))->SetSpeedY(BOMB_SPEED);		
						}
					}
					else if( rc.right - rc.left < rc.bottom - rc.top )	// 오른 왼
					{				
						if( ((CPlayerBullet*)(*iterBullet))->GetInfo().fX < ((CBomb*)(*iterBomb))->GetInfo().fX )		// 오른
						{
							((CBomb*)(*iterBomb))->SetSpeedX(BOMB_SPEED);	
						}
						else	// 왼
						{
							((CBomb*)(*iterBomb))->SetSpeedX(-BOMB_SPEED);	
						}
					}
				}
			}
			}
		}
	}
}

void CObjMgr::PlayerItemCollision()
{
	RECT	rc;

	list<CObj*>::iterator iterEndObj	= m_pObjList[OBJ_PLAYER].end();


	for(list<CObj*>::iterator iterItem = m_pObjList[OBJ_ITEM].begin();
		iterItem != m_pObjList[OBJ_ITEM].end(); ++iterItem )
	{
		for(list<CObj*>::iterator iterObj = m_pObjList[OBJ_PLAYER].begin();
			iterObj != iterEndObj; ++iterObj)
		{
			if( IntersectRect(&rc, &((CIsaac*)(*iterObj))->GetCOlRect(), &(*iterItem)->GetRect() ))
			{
				
				if((*iterItem)->GetItemType() == ITEM_PASSIVE && !((CItem*)(*iterItem))->GetItemFade())
				{
					
					if((*iterItem)->GetStrKey() == "AllUp" )
					{
						((CIsaac*)(*iterObj))->SetHPUp(true);
					}
					else if((*iterItem)->GetStrKey() == "AttackSpeedUp" )
					{
						((CIsaac*)(*iterObj))->SetHPUp(true);
						((CIsaac*)(*iterObj))->SetAttackSpeed(((CIsaac*)(*iterObj))->GetAttackSpeed() - 200);
					}
					else if((*iterItem)->GetStrKey() == "AttackUp" )
					{
						((CIsaac*)(*iterObj))->SetHPUp(true);
						((CIsaac*)(*iterObj))->SetDamage(4);

					}
					else if((*iterItem)->GetStrKey() == "Barrior" )
					{
						((CIsaac*)(*iterObj))->SetHPUp(true);
						((CIsaac*)(*iterObj))->SetFamFly(true);
					}
					else if((*iterItem)->GetStrKey() == "Bomb 10EA" )
					{
						((CIsaac*)(*iterObj))->SetHPUp(true);
						((CIsaac*)(*iterObj))->SetBombNum(((CIsaac*)(*iterObj))->GetBombNum() + 10);

					}
					else if((*iterItem)->GetStrKey() == "Diner" )
						((CIsaac*)(*iterObj))->SetHPUp(true);
					else if((*iterItem)->GetStrKey() == "Familyer" )
					{
						((CIsaac*)(*iterObj))->SetHPUp(true);
						((CIsaac*)(*iterObj))->SetFamBaby(true);

					}
					////else((*iterItem)->GetStrKey() == "Hart" )
					else if((*iterItem)->GetStrKey() == "HealthyDown" )
					{
						((CIsaac*)(*iterObj))->SetOnHit(true);
					}
					else if((*iterItem)->GetStrKey() == "Key" )
					{
						((CIsaac*)(*iterObj))->SetKeyNum(((CIsaac*)(*iterObj))->GetKeyNum() + 1);
					}
					else if((*iterItem)->GetStrKey() == "ReachUp" )
					{
						((CIsaac*)(*iterObj))->SetReach(500);
					}
					else if((*iterItem)->GetStrKey() == "SpeedUp" )
					{
						(*iterObj)->SetMaxSpeed((*iterObj)->GetMaxSpeed() + 2);
					}
					((CItem*)(*iterItem))->SetItemFade(true);

				}
				//ARROW
				else if((*iterItem)->GetItemType() == ITEM_ARROW && !((CItem*)(*iterItem))->GetItemFade())
				{
					if((*iterItem)->GetStrKey() == "BigTear" )
					{
						((CIsaac*)(*iterObj))->SetArrowType(ARROW_BIG);
					}
					else if((*iterItem)->GetStrKey() == "BloodTear" )
					{
						((CIsaac*)(*iterObj))->SetArrowType(ARROW_BLOOD);
					}
					else if((*iterItem)->GetStrKey() == "Knife" )
					{
						((CIsaac*)(*iterObj))->SetArrowType(ARROW_KNIFE);
					}
					else if((*iterItem)->GetStrKey() == "SnakeShot" )
					{
						((CIsaac*)(*iterObj))->SetArrowType(ARROW_SNAKE);
					}
					else if((*iterItem)->GetStrKey() == "Tripleshot" )
					{
						((CIsaac*)(*iterObj))->SetArrowType(ARROW_TRIPLE);
					}

					((CArrow*)(*iterItem))->SetItemFade(true);

				}
				else if((*iterItem)->GetItemType() == ITEM_ACTIVE )
				{
					((CActive*)(*iterItem))->SetItemFade(true);
					if((*iterItem)->GetStrKey() == "AllAttack" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_AllAttack);
					}
					else if((*iterItem)->GetStrKey() == "BlackCandle" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_BlackCandle);
					}
					else if((*iterItem)->GetStrKey() == "BlackHole" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_BlackHole);
					}
					else if((*iterItem)->GetStrKey() == "Boomerang" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_Boomerang);
					}
					else if((*iterItem)->GetStrKey() == "Guided Missile" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_GuidedMissile);
					}
					else if((*iterItem)->GetStrKey() == "Hanging" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_Hanging);
					}
					else if((*iterItem)->GetStrKey() == "Kamikaze" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_kamikaze);
					}
					else if((*iterItem)->GetStrKey() == "Lemon" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_Lemon);
					}
					else if((*iterItem)->GetStrKey() == "Meteo" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_Meteo);
					}
					else if((*iterItem)->GetStrKey() == "Mine" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_Mine);
					}
					else if((*iterItem)->GetStrKey() == "Sheet" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_Sheet);
					}
					else if((*iterItem)->GetStrKey() == "Teleport" )
					{
						((CIsaac*)(*iterObj))->SetActiveType(ACTIVE_Teleport);
					}
					
				}
				

			}

		}
	}
}