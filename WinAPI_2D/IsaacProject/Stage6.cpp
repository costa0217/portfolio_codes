#include "StdAfx.h"
#include "Stage6.h"

#include "ObjFactory.h"
#include "BackGround.h"

#include "RenderMgr.h"
#include "ObjMgr.h"

#include "Isaac.h"
#include "UI.h"
#include "MiniMap.h"

#include "Item.h"
#include "Arrow.h"
#include "Active.h"

#include "Fly.h"
#include "Gaper.h"
#include "Meat.h"
#include "Mulgun.h"
#include "Hopper.h"
#include "MiniBoss.h"
#include "Boss.h"

#include "Block.h"
#include "None.h"
#include "Stone.h"
#include "Fire.h"
#include "Poop.h"
#include "Door.h"

#include "Device.h"

CStage6::CStage6(void)
{
}

CStage6::~CStage6(void)
{
	Release();
}

void CStage6::Initialize(void)
{
	m_iRoomNum = 6; //  다섯번째 방

	SetPlayer();
	SetUI();
	LoadData(m_iRoomNum);
	CChapter::Initialize();

	bOpenDoorSND = false;

	m_BitMap["room6"]		= (new CBitBmp)->LoadBmp(L"../Texture/Buff/room6.bmp");

	for(int i = 0; i < TILEY; ++i)
	{
		for(int j = 0; j < TILEX; ++j)
		{
			int INDEX = i * TILEX + j;

			if(INDEX < 0 || INDEX > TILEX * TILEY)
				break;

			if(m_VecTile[INDEX]->iDrawID == TILE_BLOCK)
			{
				CObj* objTile = CObjFactory<CNone>::CreateObj(m_VecTile[INDEX]->fX, m_VecTile[INDEX]->fY, m_VecTile[INDEX]->iDrawID);
				m_ObjList[OBJ_TILE].push_back(objTile);
			}
			else if(m_VecTile[INDEX]->iDrawID == TILE_STONE)
			{
				CObj* objTile = CObjFactory<CStone>::CreateObj(m_VecTile[INDEX]->fX, m_VecTile[INDEX]->fY, m_VecTile[INDEX]->iDrawID);
				m_ObjList[OBJ_TILE].push_back(objTile);
			}
			else if(m_VecTile[INDEX]->iDrawID == TILE_FIRE)
			{
				CObj* objTile = CObjFactory<CFire>::CreateObj(m_VecTile[INDEX]->fX, m_VecTile[INDEX]->fY, m_VecTile[INDEX]->iDrawID);
				m_ObjList[OBJ_TILE].push_back(objTile);
			}
			else if(m_VecTile[INDEX]->iDrawID == TILE_POOP)
			{
				CObj* objTile = CObjFactory<CPoop>::CreateObj(m_VecTile[INDEX]->fX, m_VecTile[INDEX]->fY, m_VecTile[INDEX]->iDrawID);
				m_ObjList[OBJ_TILE].push_back(objTile);
			}
			else if(m_VecTile[INDEX]->iDrawID == TILE_DOOR)
			{
				CObj* objTile = CObjFactory<CDoor>::CreateObj(m_VecTile[INDEX]->fX, m_VecTile[INDEX]->fY, m_VecTile[INDEX]->iDrawID, INDEX);
				m_ObjList[OBJ_TILE].push_back(objTile);
			}
		}
	}

	/*================ Passive 아이템 ================*/
	CObj* pItem = CObjFactory<CItem>::CreateObj(110.f, 275.f, "AllUp");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(110.f, 325.f, "AttackSpeedUp");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(110.f, 375.f, "AttackUp");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(110.f, 425.f, "Barrior");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(110.f, 475.f, "Bomb 10EA");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	
	pItem = CObjFactory<CItem>::CreateObj(170.f, 275.f, "Diner");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(170.f, 325.f, "Familyer");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(170.f, 375.f, "HealthyDown");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(170.f, 425.f, "Key");
	m_ObjList[OBJ_ITEM].push_back(pItem);
	pItem = CObjFactory<CItem>::CreateObj(170.f, 475.f, "ReachUp");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	pItem = CObjFactory<CItem>::CreateObj(170.f, 225.f, "SpeedUp");
	m_ObjList[OBJ_ITEM].push_back(pItem);	
	/*================================================*/

	/*================  Arrow  아이템 ================*/
	CObj* pArrow = CObjFactory<CArrow>::CreateObj(300.f, 425.f, "BigTear");
	m_ObjList[OBJ_ITEM].push_back(pArrow);
	pArrow = CObjFactory<CArrow>::CreateObj(380.f, 425.f, "BloodTear");
	m_ObjList[OBJ_ITEM].push_back(pArrow);
	pArrow = CObjFactory<CArrow>::CreateObj(460.f, 425.f, "Knife");
	m_ObjList[OBJ_ITEM].push_back(pArrow);
	pArrow = CObjFactory<CArrow>::CreateObj(340.f, 480.f, "SnakeShot");
	m_ObjList[OBJ_ITEM].push_back(pArrow);
	pArrow = CObjFactory<CArrow>::CreateObj(280.f, 480.f, "Tripleshot");
	m_ObjList[OBJ_ITEM].push_back(pArrow);
	
	/*================================================*/

	/*================  Active 아이템 ================*/
	CObj* pActive = CObjFactory<CActive>::CreateObj(685.f, 275.f, "AllAttack");
	m_ObjList[OBJ_ITEM].push_back(pActive);
	/*pActive = CObjFactory<CActive>::CreateObj(685.f, 325.f, "BlackCandle");
	m_ObjList[OBJ_ITEM].push_back(pActive);
	pActive = CObjFactory<CActive>::CreateObj(685.f, 375.f, "BlackHole");
	m_ObjList[OBJ_ITEM].push_back(pActive);
	pActive = CObjFactory<CActive>::CreateObj(685.f, 425.f, "Boomerang");
	m_ObjList[OBJ_ITEM].push_back(pActive);*/

	/*pActive = CObjFactory<CActive>::CreateObj(625.f, 275.f, "Guided Missile");
	m_ObjList[OBJ_ITEM].push_back(pActive);*/
	pActive = CObjFactory<CActive>::CreateObj(625.f, 325.f, "Hanging");
	m_ObjList[OBJ_ITEM].push_back(pActive);
	/*pActive = CObjFactory<CActive>::CreateObj(625.f, 375.f, "Kamikaze");
	m_ObjList[OBJ_ITEM].push_back(pActive);*/
	pActive = CObjFactory<CActive>::CreateObj(625.f, 425.f, "Lemon");
	m_ObjList[OBJ_ITEM].push_back(pActive);

	pActive = CObjFactory<CActive>::CreateObj(560.f, 275.f, "Meteo");
	m_ObjList[OBJ_ITEM].push_back(pActive);
	/*pActive = CObjFactory<CActive>::CreateObj(560.f, 325.f, "Mine");
	m_ObjList[OBJ_ITEM].push_back(pActive);
	pActive = CObjFactory<CActive>::CreateObj(560.f, 375.f, "Sheet");
	m_ObjList[OBJ_ITEM].push_back(pActive);*/
	pActive = CObjFactory<CActive>::CreateObj(560.f, 425.f, "Teleport");
	m_ObjList[OBJ_ITEM].push_back(pActive);

	/*================================================*/

	m_pBack = CObjFactory<CBackGround>::CreateObj();

	m_ObjList[OBJ_PLAYER].push_back(m_pPlayer);
	((CIsaac*)m_pPlayer)->SetFamilyerlist(&m_ObjList[OBJ_FAMLIYER]);
	m_ObjList[OBJ_UI].push_back(m_pUI);

	
	CDevice::GetInstance()->SoundPlay(CLOSE, 0);
	CDevice::GetInstance()->SoundPlay(ITEMROOM, 0);
	

	CObj::SetBitMap(&m_BitMap);
}
void CStage6::Progress(void)
{
	m_iRoomNum = 6;
	CObjMgr::GetInst()->SetObjList(m_ObjList);

	CChapter::Progress();		// 충돌체크 다 담아둠
	KeyInput();					// 키입력

	((CMiniMap*)m_pUI)->SetRoom(m_iRoomNum - 1);
	((CIsaac*)m_pPlayer)->SetRoomNum(m_iRoomNum);	//지금 있는 방 번호
	((CIsaac*)m_pPlayer)->SetBulletlist(&m_ObjList[OBJ_BULLET]);
	
	/*몬스터 없을때만 문열림*/
	if(m_ObjList[OBJ_MONSTER].size() == 0)
	{
		for(list<CObj*>::iterator iterTile = m_ObjList[OBJ_TILE].begin();
			iterTile != m_ObjList[OBJ_TILE].end(); ++iterTile)
		{
			if((*iterTile)->GetDrawID() == TILE_DOOR)
			{
				((CDoor*)(*iterTile))->SetOpenDoor(true);
				if(!bOpenDoorSND)
				{
					CDevice::GetInstance()->SoundPlay(OPEN, 0);
					bOpenDoorSND = true;
				}
			}
		}
	}
	else
	{
		for(list<CObj*>::iterator iterTile = m_ObjList[OBJ_TILE].begin();
			iterTile != m_ObjList[OBJ_TILE].end(); ++iterTile)
		{
			if((*iterTile)->GetDrawID() == TILE_DOOR)
			{
				((CDoor*)(*iterTile))->SetOpenDoor(false);
			}
		}
	}

	for(int i = 0; i < OBJ_END; ++i)
	{
		for(list<CObj*>::iterator iter= m_ObjList[i].begin();
			iter != m_ObjList[i].end();)
		{
			if(i == OBJ_TILE || i == OBJ_ITEM || i == OBJ_BLOOD )			// 타일은 Y소팅 안함
			{	
				if((*iter)->Progress() == 1)
				{
					::Safe_Delete(*iter);
					iter = m_ObjList[i].erase(iter);
				}
				else
				{
					++iter;
				}	
				/*(*iter)->Progress();
				++iter;*/
			}
			else if( i == OBJ_BULLET || i == OBJ_MONSTER )	// 총알/몬스터 리턴1시 삭제
			{
				if((*iter)->Progress() == 1)
				{
					::Safe_Delete(*iter);
					iter = m_ObjList[i].erase(iter);
				}
				else
				{
					CRenderMgr::GetInst()->AddObj((*iter));
					++iter;
				}	
			}
			else
			{
				(*iter)->Progress();
				CRenderMgr::GetInst()->AddObj((*iter));
				++iter;
			}
		}
	}
}
void CStage6::Render(HDC hdc)
{
	m_pBack->Render(m_BitMap["BackGround"]->GetMemDC());

	BitBlt(m_BitMap["BackGround"]->GetMemDC(), 0, 100, WINCX, WINCY,
		m_BitMap["room6"]->GetMemDC(), 0, 0, SRCCOPY);

	for(list<CObj*>::iterator iter= m_ObjList[OBJ_BLOOD].begin();
			iter != m_ObjList[OBJ_BLOOD].end(); ++iter)
	{
		(*iter)->Render(m_BitMap["BackGround"]->GetMemDC());
	}


	for(list<CObj*>::iterator iter= m_ObjList[OBJ_TILE].begin();
			iter != m_ObjList[OBJ_TILE].end(); ++iter)
	{
		(*iter)->Render(m_BitMap["BackGround"]->GetMemDC());
	}

	for(list<CObj*>::iterator iter= m_ObjList[OBJ_ITEM].begin();
			iter != m_ObjList[OBJ_ITEM].end(); ++iter)
	{
		(*iter)->Render(m_BitMap["BackGround"]->GetMemDC());
	}



	CChapter::Render(m_BitMap["BackGround"]->GetMemDC());
	CRenderMgr::GetInst()->ObjRender(m_BitMap["BackGround"]->GetMemDC());


	BitBlt(hdc, 0, 0, WINCX, WINCY, m_BitMap["BackGround"]->GetMemDC(), 0, 0, SRCCOPY);
	CRenderMgr::GetInst()->Release();
	
}
void CStage6::Release(void)
{
	::Safe_Delete(m_pBack);
	for(vector<TILE*>::iterator iter = m_VecTile.begin();
		iter != m_VecTile.end(); ++iter)
	{
		::Safe_Delete(*iter);
	}
	m_VecTile.clear();
	vector<TILE*>().swap(m_VecTile);
	
	for(int i = 1; i < OBJ_END; ++i)
	{
		if( i != OBJ_UI )
		{
			for(list<CObj*>::iterator iter= m_ObjList[i].begin();
				iter != m_ObjList[i].end(); ++iter)
			{
				::Safe_Delete(*iter);
			}
		}
		m_ObjList[i].clear();
	}
	m_ObjList[OBJ_PLAYER].clear();
	//CObjMgr::GetInst()->SetNextRoom(false);
}

void CStage6::KeyInput(void)
{
	if(GetAsyncKeyState('1'))
		m_eMonType = MON_FLY;
	if(GetAsyncKeyState('2'))
		m_eMonType = MON_GAPER;
	if(GetAsyncKeyState('3'))
		m_eMonType = MON_MEAT;
	if(GetAsyncKeyState('4'))
		m_eMonType = MON_MINIBOSS;

	if((GetAsyncKeyState(VK_LBUTTON) & 0x0001))
	{
		Picking(m_eMonType);
	}
}

void CStage6::Picking(MONSTERID m_eMonType)
{
	float fX = float(GetMouse().x);
	float fY = float(GetMouse().y);

	if(m_eMonType == MON_FLY)
	{
		CObj* pMonster = CObjFactory<CFly>::CreateObj(fX, fY);
		((CFly*)pMonster)->SetPlayer(m_pPlayer);
		m_ObjList[OBJ_MONSTER].push_back(pMonster);	
	}
	else if(m_eMonType == MON_GAPER)
	{
		CObj* pMonster = CObjFactory<CGaper>::CreateObj(fX, fY);
		((CGaper*)pMonster)->SetPlayer(m_pPlayer);
		((CGaper*)pMonster)->SetBulletlist(&m_ObjList[OBJ_BULLET]);
		((CGaper*)pMonster)->SetBloodlist(&m_ObjList[OBJ_BLOOD]);
		m_ObjList[OBJ_MONSTER].push_back(pMonster);	
	}
	else if(m_eMonType == MON_MEAT)
	{
		CObj* pMonster = CObjFactory<CMeat>::CreateObj(fX, fY);
		((CMeat*)pMonster)->SetBulletlist(&m_ObjList[OBJ_BULLET]);
		((CMeat*)pMonster)->SetBloodlist(&m_ObjList[OBJ_BLOOD]);
		m_ObjList[OBJ_MONSTER].push_back(pMonster);	

	}
	else if(m_eMonType == MON_MINIBOSS)
	{
		CObj* pMonster = CObjFactory<CMiniBoss>::CreateObj(fX, fY);
		m_ObjList[OBJ_MONSTER].push_back(pMonster);	
		((CMiniBoss*)pMonster)->SetMonsterlist(&m_ObjList[OBJ_MONSTER]);
		((CMiniBoss*)pMonster)->SetBloodlist(&m_ObjList[OBJ_BLOOD]);
	}
}