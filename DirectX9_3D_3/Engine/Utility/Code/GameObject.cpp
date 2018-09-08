#include "GameObject.h"

#include "Component.h"

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CGameObject::~CGameObject(void)
{
	Release();
}

const Engine::CComponent* Engine::CGameObject::GetComponent(const wstring& wstrComponentKey)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.find(wstrComponentKey);
	if(iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

HRESULT Engine::CGameObject::Initialize(void)
{
	return S_OK;
}

int Engine::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for( ;iter != iter_end ; ++iter)
	{
		if(iter->second!= NULL)
			iter->second->Update();
	}

	return 0;
}

void Engine::CGameObject::Release(void)
{
	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDelete_Map());
	m_mapComponent.clear();
}