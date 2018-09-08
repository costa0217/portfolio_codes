#include "Layer.h"

#include "GameObject.h"
#include "Component.h"

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CLayer::~CLayer(void)
{
	Release();
}

const Engine::CComponent* Engine::CLayer::GetComponent(const wstring& wstrObjKey , const wstring& wstrComponentKey)
{
	MAPOBJLIST::iterator	mapiter = m_mapObjlist.find(wstrObjKey);
	if(mapiter == m_mapObjlist.end())
		return NULL;

	OBJECTLIST::iterator	iterlist = mapiter->second.begin();
	OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

	for( ;iterlist != iterlist_end ; ++iterlist)
	{
		const CComponent* pComponent = (*iterlist)->GetComponent(wstrComponentKey);
		if(pComponent != NULL)
			return pComponent;
	}

	return NULL;
}

HRESULT Engine::CLayer::AddObject(const wstring& wstrObjKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		MAPOBJLIST::iterator	iter = m_mapObjlist.find(wstrObjKey);
		if(iter == m_mapObjlist.end())
		{
			m_mapObjlist[wstrObjKey] = OBJECTLIST();
		}
		m_mapObjlist[wstrObjKey].push_back(pGameObject);
	}
	return S_OK;
}

void Engine::CLayer::Update(void)
{
	int iResult = 0;
	MAPOBJLIST::iterator		mapiter = m_mapObjlist.begin();
	MAPOBJLIST::iterator		mapiter_end = m_mapObjlist.end();

	for( ; mapiter != mapiter_end; ++mapiter )
	{
		OBJECTLIST::iterator	iterlist = mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

		for( ; iterlist != iterlist_end ;)
		{
			iResult = (*iterlist)->Update();

			if(iResult == 1)
			{
				Safe_Delete(*iterlist);
				iterlist = mapiter->second.erase(iterlist);
			}

			else
				++iterlist;
		}
	}
}

void Engine::CLayer::Render(void)
{
	MAPOBJLIST::iterator		mapiter = m_mapObjlist.begin();
	MAPOBJLIST::iterator		mapiter_end = m_mapObjlist.end();

	for( ; mapiter != mapiter_end; ++mapiter )
	{
		OBJECTLIST::iterator	iterlist = mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

		for( ; iterlist != iterlist_end ; ++iterlist)
		{
			(*iterlist)->Render();
		}
	}
}

Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLayer*		pLayer = new CLayer(pDevice);

	return pLayer;
}

void Engine::CLayer::Release(void)
{
	MAPOBJLIST::iterator		mapiter = m_mapObjlist.begin();
	MAPOBJLIST::iterator		mapiter_end = m_mapObjlist.end();

	for( ; mapiter != mapiter_end; ++mapiter )
	{
		OBJECTLIST::iterator	iterlist = mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

		for( ; iterlist != iterlist_end ; ++iterlist)
		{
			Engine::Safe_Delete(*iterlist);
		}
		mapiter->second.clear();
	}
	m_mapObjlist.clear();
}

const Engine::VTXTEX* Engine::CLayer::GetTerrainVertex(const wstring& wstrObjKey)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.find(wstrObjKey);
	if(iter == m_mapObjlist.end())
		return NULL;

	return iter->second.front()->GetTerrainVtx();
}
