#include "StdAfx.h"
#include "Stage1.h"

#include "ObjFactory.h"
#include "BackGround.h"

#include "RenderMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"


#include "Isaac.h"
#include "UI.h"
#include "MiniMap.h"
#include "Hart.h"

#include "Fly.h"
#include "Gaper.h"
#include "Meat.h"
#include "Mulgun.h"
#include "Hopper.h"
#include "Boss.h"

#include "Block.h"
#include "None.h"
#include "Stone.h"
#include "Fire.h"
#include "Poop.h"
#include "Door.h"

#include "Alpha.h"

#include "Device.h"

CStage1::CStage1(void)
{
}

CStage1::~CStage1(void)
{
	Release();
}

void CStage1::Initialize(void)
{
	m_iRoomNum = 1; //첫번째 방

		// 사운드
	
	
	/*SetPlayer();
	SetUI();*/
	LoadData(m_iRoomNum);
	CChapter::Initialize();
	
	m_BitMap["room1"]		= (new CBitBmp)->LoadBmp(L"../Texture/Buff/room1.bmp");
	

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

	/*CObj* pMonster = CObjFactory<CBoss>::CreateObj(392.f, 290.f);
	m_ObjList[OBJ_MONSTER].push_back(pMonster);	
	((CBoss*)pMonster)->SetBulletlist(&m_ObjList[OBJ_BULLET]);
	((CBoss*)pMonster)->SetPlayer(m_pPlayer);*/

	
	CDevice::GetInstance()->SoundPlay(BGM, 1);
	


	m_pBack = CObjFactory<CBackGround>::CreateObj();

	m_pAlpha = CObjFactory<CAlpha>::CreateObj();

	m_ObjList[OBJ_PLAYER].push_back(m_pPlayer);
	m_ObjList[OBJ_UI].push_back(m_pUI);

	
	CObj::SetBitMap(&m_BitMap);




}

void CStage1::Progress(void)
{
	m_iRoomNum = 1;
	CObjMgr::GetInst()->SetObjList(m_ObjList);

	CChapter::Progress();		// 충돌체크 다 담아둠



	((CMiniMap*)m_pUI)->SetRoom(m_iRoomNum -1);
	((CIsaac*)m_pPlayer)->SetRoomNum(m_iRoomNum);
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
			if(i == OBJ_TILE )			// 타일은 Y소팅 안함
			{
				(*iter)->Progress();
				++iter;
			}
			else if( i == OBJ_BULLET || i == OBJ_MONSTER )	// 총알 리턴1시 삭제
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
	if(m_pAlpha && m_pAlpha->Progress() == 1)
	{
		::Safe_Delete(m_pAlpha);
		m_pAlpha = NULL;
	}
}
void CStage1::Render(HDC hdc)
{
	m_pBack->Render(m_BitMap["BackGround"]->GetMemDC());

	BitBlt(m_BitMap["BackGround"]->GetMemDC(), 0, 100, WINCX, WINCY,
		m_BitMap["room1"]->GetMemDC(), 0, 0, SRCCOPY);
	

	////플레이어 피
	//TCHAR szHP[32] = L"";
	//wsprintf(szHP, L"HP : %d", m_pPlayer->GetHP());
	//TextOut(m_BitMap["BackGround"]->GetMemDC(), 
	//		580,60,
	//		szHP, lstrlen(szHP));
	//TCHAR szOnHit[32] = L"";
	//if(((CIsaac*)m_pPlayer)->GetOnHit())
	//	wsprintf(szOnHit, L"충돌 : True", ((CIsaac*)m_pPlayer)->GetOnHit());
	//else
	//	wsprintf(szOnHit, L"충돌 : False", ((CIsaac*)m_pPlayer)->GetOnHit());

	//TextOut(m_BitMap["BackGround"]->GetMemDC(), 
	//		630,60,
	//		szOnHit, lstrlen(szOnHit));


	CChapter::Render(m_BitMap["BackGround"]->GetMemDC());

	for(list<CObj*>::iterator iter= m_ObjList[OBJ_TILE].begin();
			iter != m_ObjList[OBJ_TILE].end(); ++iter)
	{
		(*iter)->Render(m_BitMap["BackGround"]->GetMemDC());
	}

	
	CRenderMgr::GetInst()->ObjRender(m_BitMap["BackGround"]->GetMemDC());

	

	if(m_pAlpha)
		m_pAlpha->Render(m_BitMap["BackGround"]->GetMemDC());

	BitBlt(hdc, 0, 0, WINCX, WINCY, m_BitMap["BackGround"]->GetMemDC(), 0, 0, SRCCOPY);

	

	CRenderMgr::GetInst()->Release();

}
void CStage1::Release(void)
{
	
	//SavePlayerData();

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

	::Safe_Delete(m_pAlpha);

	//CObjMgr::GetInst()->SetNextRoom(false);
}
