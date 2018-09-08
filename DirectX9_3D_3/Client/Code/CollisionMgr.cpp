#include "stdafx.h"
#include "CollisionMgr.h"

#include "Export_Function.h"
#include "Collision.h"
#include "TerrainCol.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr(void)
{

}

CCollisionMgr::~CCollisionMgr(void)
{
	Release();
}

Engine::CCollision* CCollisionMgr::CloneColObject(COLLSIONID eCollisionID)
{
	MAPCOLLISION::iterator		iter = m_mapCollision.find(eCollisionID);
	if(iter == m_mapCollision.end())
		return NULL;

	return iter->second->Clone();
}

HRESULT CCollisionMgr::AddColObject(COLLSIONID eCollisionID)
{
	MAPCOLLISION::iterator	iter = m_mapCollision.find(eCollisionID);
	if(iter != m_mapCollision.end())
	{
		MSG_BOX(L"중복된 개체가 정의되어 있음");
		return E_FAIL;
	}

	Engine::CCollision*			pCollision = NULL;

	switch(eCollisionID)
	{
	case COL_TERRAIN:
		pCollision = CTerrainCol::Create();
		break;

	case COL_MOUSE:
		break;
	}

	m_mapCollision.insert(MAPCOLLISION::value_type(eCollisionID, pCollision));
	return S_OK;
}

void CCollisionMgr::Release(void)
{
	for_each(m_mapCollision.begin(), m_mapCollision.end(), Engine::CDelete_Map());
	m_mapCollision.clear();
}
