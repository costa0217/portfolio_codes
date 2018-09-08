#include "StdAfx.h"
#include "BossStage.h"

#include "ObjFactory.h"
#include "BackGround.h"

#include "RenderMgr.h"
#include "ObjMgr.h"

#include "Isaac.h"
#include "UI.h"
#include "MiniMap.h"
#include "Boss.h"

#include "Block.h"
#include "None.h"
#include "Stone.h"
#include "Fire.h"
#include "Poop.h"
#include "Door.h"

#include "Device.h"

CBossStage::CBossStage(void)
{
}

CBossStage::~CBossStage(void)
{
	Release();
}

void CBossStage::Initialize(void)
{
	m_iRoomNum = 8; //  보스 방
	CDevice::GetInstance()->SoundStop(BGM);

	SetPlayer();
	SetUI();
	LoadData(m_iRoomNum);
	CChapter::Initialize();

	CDevice::GetInstance()->SoundPlay(BOSSBGM, 1);

	bOpenDoorSND = false;

	m_BitMap["bossroom"]		= (new CBitBmp)->LoadBmp(L"../Texture/Buff/bossroom.bmp");
	m_BitMap["end"]				= (new CBitBmp)->LoadBmp(L"../Texture/end.bmp");

	m_tEndingFrame = FRAME(0, 1, 0, 80);
	m_dwEndingTime = GetTickCount();

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

	CObj* pMonster = CObjFactory<CBoss>::CreateObj(392.f, 290.f);
	m_ObjList[OBJ_MONSTER].push_back(pMonster);	
	((CBoss*)pMonster)->SetBulletlist(&m_ObjList[OBJ_BULLET]);
	((CBoss*)pMonster)->SetPlayer(m_pPlayer);

	m_pBack = CObjFactory<CBackGround>::CreateObj();

	m_ObjList[OBJ_PLAYER].push_back(m_pPlayer);
	m_ObjList[OBJ_UI].push_back(m_pUI);
	
	
	CDevice::GetInstance()->SoundPlay(CLOSE, 0);

	CObj::SetBitMap(&m_BitMap);
}
void CBossStage::Progress(void)
{
	m_iRoomNum = 8;
	CObjMgr::GetInst()->SetObjList(m_ObjList);

	CChapter::Progress();		// 충돌체크 다 담아둠

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
					CDevice::GetInstance()->SoundPlay(BOSSDOOR, 0);
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
			if(i == OBJ_TILE )			// 타일은 Y소팅 안함
			{
				(*iter)->Progress();
				++iter;
			}
			else if( i == OBJ_BULLET || i == OBJ_MONSTER  )	// 총알/몬스터 리턴1시 삭제
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

	if(bOpenDoorSND)
	{
		if(m_dwEndingTime + m_tEndingFrame.dwTime < GetTickCount())
		{
			m_dwEndingTime = GetTickCount();
			
			if(m_tEndingFrame.iStart >= m_tEndingFrame.iLast)
			{
				m_tEndingFrame.iStart = 0;
			}
			++m_tEndingFrame.iStart;
		}
	}
}
void CBossStage::Render(HDC hdc)
{
	m_pBack->Render(m_BitMap["BackGround"]->GetMemDC());

	BitBlt(m_BitMap["BackGround"]->GetMemDC(), 0, 100, WINCX, WINCY,
		m_BitMap["bossroom"]->GetMemDC(), 0, 0, SRCCOPY);


	for(list<CObj*>::iterator iter= m_ObjList[OBJ_TILE].begin();
			iter != m_ObjList[OBJ_TILE].end(); ++iter)
	{
		(*iter)->Render(m_BitMap["BackGround"]->GetMemDC());
	}

	CChapter::Render(m_BitMap["BackGround"]->GetMemDC());
	CRenderMgr::GetInst()->ObjRender(m_BitMap["BackGround"]->GetMemDC());


	if(bOpenDoorSND)
	{
		TransparentBlt(m_BitMap["BackGround"]->GetMemDC(),								
			int(WINCX / 2.f - 128.f),
			int(WINCY / 2.f - 100.f),
			256, 288,
			m_BitMap["end"]->GetMemDC(),
			int(0 * m_tEndingFrame.iStart),	0,
			128, 144,
			RGB(255, 0, 255));
	}
	BitBlt(hdc, 0, 0, WINCX, WINCY, m_BitMap["BackGround"]->GetMemDC(), 0, 0, SRCCOPY);
	CRenderMgr::GetInst()->Release();
	
}
void CBossStage::Release(void)
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
