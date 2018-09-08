#include "StdAfx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "ObjFactory.h"
#include "Back.h"
#include "StageBar.h"
#include "ActiveBar.h"
#include "StopButt.h"

#include "Soldier.h"
#include "FootMan.h"

#include "CollisionMgr.h"
#include "RenderMgr.h"
#include "SceneMgr.h"

#include "Effect.h"




IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr(void)
{
}


CObjMgr::~CObjMgr(void)
{
	Release();
}

HRESULT CObjMgr::Initialize(void)
{
	CCollisionMgr::GetInstance()->SetObjList(m_ObjList);
	m_bSetScene	= false;
	return S_OK;
}

int CObjMgr::Progress(void)
{	

	CCollisionMgr::GetInstance()->SoldierToMonster();
	CCollisionMgr::GetInstance()->ArchorTowerToMonster();
	CCollisionMgr::GetInstance()->ArrowToMonster();

	int iResult = 0;

	for(int i = 0; i < OBJ_END; ++i)
	{
		for(list<CObj*>::iterator	iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); /*++iter*/)
		{ 
			iResult = (*iter)->Progress();
			if(i == OBJ_UI || i == OBJ_TOWERUI 
				|| i == OBJ_PROJECTILE || i == OBJ_EFFECT
				|| i == OBJ_LASTWND)
			{
				if(iResult == 1)
				{
					::Safe_Delete((*iter));
					iter = m_ObjList[i].erase(iter);
				}
				else if(iResult == 2)
				{
					CRenderMgr::GetInstance()->Release();
					CSceneMgr::GetInstance()->Initialize(SC_FIELD);
					return 0;
				}
				else
					++iter;
			}
			else if( i == OBJ_SOLDIER
				&& (( ((CSoldier*)(*iter))->GetSoliderID() == SOL_RIFLEMAN
					|| ((CSoldier*)(*iter))->GetSoliderID() == SOL_WIZARD)))
			{
				if(iResult == 1)
				{
					::Safe_Delete((*iter));
					iter = m_ObjList[i].erase(iter);
				}
				else
					++iter;
			}
			else
			{
				if(iResult == 1)
				{
					::Safe_Delete((*iter));
					iter = m_ObjList[i].erase(iter);
				}
				else
				{
					CRenderMgr::GetInstance()->AddObj((*iter));
					++iter;
				}
			}
		}
	}
	return iResult;
}
void CObjMgr::Render(void)
{
	/* 객체 그리기 */
	CRenderMgr::GetInstance()->ObjRender();
	CRenderMgr::GetInstance()->Release();
	
	for(list<CObj*>::iterator 	iter = m_ObjList[OBJ_SOLDIER].begin();
		iter != m_ObjList[OBJ_SOLDIER].end(); ++iter)
	{
		if( ((CSoldier*)(*iter))->GetSoliderID() != SOL_FOOTMAN
			&& ((CSoldier*)(*iter))->GetSoliderID() != SOL_GOLEM )
			(*iter)->Render();
	}
	/* 투사체 그리기 */
	
	for(list<CObj*>::iterator 	iter = m_ObjList[OBJ_EFFECT].begin();
		iter != m_ObjList[OBJ_EFFECT].end(); ++iter)
	{
		(*iter)->Render();
	}
	/* UI 그리기 */
	for(list<CObj*>::iterator 	iter = m_ObjList[OBJ_UI].begin();
		iter != m_ObjList[OBJ_UI].end(); ++iter)
	{
		(*iter)->Render();
	}
	for(list<CObj*>::iterator 	iter = m_ObjList[OBJ_PROJECTILE].begin();
		iter != m_ObjList[OBJ_PROJECTILE].end(); ++iter)
	{
		(*iter)->Render();
	}
	for(list<CObj*>::iterator 	iter = m_ObjList[OBJ_TOWERUI].begin();
		iter != m_ObjList[OBJ_TOWERUI].end(); ++iter)
	{
		(*iter)->Render();
	}
	for(list<CObj*>::iterator 	iter = m_ObjList[OBJ_LASTWND].begin();
		iter != m_ObjList[OBJ_LASTWND].end(); ++iter)
	{
		(*iter)->Render();
	}
}

void CObjMgr::Release(void)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), DeleteOBJ());
		m_ObjList[i].clear();
	}
}

void CObjMgr::AddObject(OBJID _eObjID, CObj* pObj)
{
	m_ObjList[_eObjID].push_back(pObj);
}