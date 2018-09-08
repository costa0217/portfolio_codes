#include "StdAfx.h"
#include "Stage7.h"

#include "ObjFactory.h"
#include "BackGround.h"

#include "RenderMgr.h"
#include "ObjMgr.h"

#include "Isaac.h"
#include "UI.h"
#include "MiniMap.h"

#include "Hopper.h"

#include "Block.h"
#include "None.h"
#include "Stone.h"
#include "Fire.h"
#include "Poop.h"
#include "Door.h"

#include "Device.h"

CStage7::CStage7(void)
{
}

CStage7::~CStage7(void)
{
	Release();
}

void CStage7::Initialize(void)
{
	m_iRoomNum = 7; //  �ϰ���° ��

	SetPlayer();
	SetUI();
	LoadData(m_iRoomNum);
	CChapter::Initialize();

	bOpenDoorSND = false;

	m_BitMap["room7"]		= (new CBitBmp)->LoadBmp(L"../Texture/Buff/room7.bmp");

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

	for (int i = 0; i < 2; ++i)
	{
		CObj* pMonster = CObjFactory<CHopper>::CreateObj(float(rand()%640 + 80), float(rand()%350 + 170));
		m_ObjList[OBJ_MONSTER].push_back(pMonster);	
		((CHopper*)pMonster)->SetBloodlist(&m_ObjList[OBJ_BLOOD]);
	}

	m_pBack = CObjFactory<CBackGround>::CreateObj();

	m_ObjList[OBJ_PLAYER].push_back(m_pPlayer);
	m_ObjList[OBJ_UI].push_back(m_pUI);

	
	CDevice::GetInstance()->SoundPlay(CLOSE, 0);
	

	CObj::SetBitMap(&m_BitMap);
}
void CStage7::Progress(void)
{
	m_iRoomNum = 7;
	CObjMgr::GetInst()->SetObjList(m_ObjList);

	CChapter::Progress();		// �浹üũ �� ��Ƶ�

	((CMiniMap*)m_pUI)->SetRoom(m_iRoomNum - 1);
	((CIsaac*)m_pPlayer)->SetRoomNum(m_iRoomNum);	//���� �ִ� �� ��ȣ
	((CIsaac*)m_pPlayer)->SetBulletlist(&m_ObjList[OBJ_BULLET]);

	/*���� �������� ������*/
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
			if(i == OBJ_TILE || i == OBJ_BLOOD )			// Ÿ���� Y���� ����
			{
				(*iter)->Progress();
				++iter;
			}
			else if( i == OBJ_BULLET || i == OBJ_MONSTER  )	// �Ѿ�/���� ����1�� ����
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
void CStage7::Render(HDC hdc)
{
	m_pBack->Render(m_BitMap["BackGround"]->GetMemDC());

	BitBlt(m_BitMap["BackGround"]->GetMemDC(), 0, 100, WINCX, WINCY,
		m_BitMap["room7"]->GetMemDC(), 0, 0, SRCCOPY);

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

	CChapter::Render(m_BitMap["BackGround"]->GetMemDC());
	CRenderMgr::GetInst()->ObjRender(m_BitMap["BackGround"]->GetMemDC());


	BitBlt(hdc, 0, 0, WINCX, WINCY, m_BitMap["BackGround"]->GetMemDC(), 0, 0, SRCCOPY);
	CRenderMgr::GetInst()->Release();
	
}
void CStage7::Release(void)
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