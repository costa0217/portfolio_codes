#include "Scene.h"
#include "Layer.h"
#include "Engine_Include.h"
#include "KeyMgr.h"

bool         Engine::CScene::m_bPause = false;

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_bRain(false)
, m_bStart(false)
, m_siba(true)
{

}

Engine::CScene::~CScene(void)
{
	Release();
}

HRESULT Engine::CScene::InitScene(void)
{
	return S_OK;
}

void Engine::CScene::Update(void)
{
	MAPLAYER::iterator		iter = m_mapLayer.begin();
	MAPLAYER::iterator		iter_end = m_mapLayer.end();

	MAPLAYER::iterator		iter2 = m_mapLayer.find(LAYER_PAUSE);
	MAPLAYER::iterator		iter3 = m_mapLayer.find(LAYER_RAIN);
	MAPLAYER::iterator		iter4 = m_mapLayer.find(LAYER_CHOICE);

	for( ; iter != iter_end; ++iter)
	{
		if(m_bPause)
		{
			if(iter == iter2)
			{
				iter->second->Update();
			}
		}
		else if(iter != iter2 && iter != iter3)
		{
			if(iter != iter4)
			{
				iter->second->Update();
			}

			else if(iter == iter4)
			{
				if(m_siba)
				{
					iter->second->Update();
				}
			}
		}
		else if(iter == iter3 )
		{
			if(m_bRain)
			{
				iter->second->Update();
			}
		}
	}

	if(GetAsyncKeyState('P') & 0x0001 )
	{
		m_bPause ^= true;
	}
}

void Engine::CScene::Render(void)
{
	MAPLAYER::iterator		iter = m_mapLayer.begin();
	MAPLAYER::iterator		iter_end = m_mapLayer.end();

	MAPLAYER::iterator		iter2 = m_mapLayer.find(LAYER_PAUSE);
	MAPLAYER::iterator		iter3 = m_mapLayer.find(LAYER_RAIN);
	MAPLAYER::iterator		iter4 = m_mapLayer.find(LAYER_CHOICE);

	if(m_bStart)
	{
		m_siba = false;
	}

	for( ; iter != iter_end; ++iter)
	{
		if(iter != iter2 && iter != iter3)
		{
			if(iter != iter4)
			{
				iter->second->Render();
			}

			else if(iter == iter4)
			{
				if(m_siba)
				{
					iter->second->Render();
				}
			}
		}

		else if(iter == iter2)
		{
			if(m_bPause)
			{
				iter->second->Render();
			}
		}

		else if(iter == iter3 )
		{
			if(m_bRain)
			{
				iter->second->Render();
			}
		}
	}


	if(GetAsyncKeyState('R') & 0x0001 )
	{
		m_bRain ^= true;
	}
}

void Engine::CScene::Release(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDelete_Map());
	m_mapLayer.clear();
}

const Engine::VTXTEX* Engine::CScene::GetTerrainVtx(const WORD& LayerID, const wstring& wstrObjKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);
	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetTerrainVertex(wstrObjKey);
}

void Engine::CScene::SetBool(bool byunsoo)
{
	m_bPause = byunsoo;
}

