#include "InfoSubject.h"

IMPLEMENT_SINGLETON(Engine::CInfoSubject)

Engine::CInfoSubject::CInfoSubject(void)
{

}

Engine::CInfoSubject::~CInfoSubject(void)
{
	Release();
}

Engine::DATALIST* Engine::CInfoSubject::GetDatalist(int message)
{
	MAPDATALIST::iterator iter = m_mapDataList.find(message);
	if(iter == m_mapDataList.end())
		return NULL;

	return &iter->second;
}

void Engine::CInfoSubject::AddData(int message, void* pData)
{
	if(pData)
	{
		MAPDATALIST::iterator	iter = m_mapDataList.find(message);
		if(iter == m_mapDataList.end())
		{
			m_mapDataList[message] = DATALIST();
		}
		m_mapDataList[message].push_back(pData);
		Notify(message);
	}
}

void Engine::CInfoSubject::RemoveData(int message, void* pData)
{
	MAPDATALIST::iterator	mapiter = m_mapDataList.find(message);
	if(mapiter != m_mapDataList.end())
	{
		DATALIST::iterator		iterlist = mapiter->second.begin();
		DATALIST::iterator		iterlist_end = mapiter->second.end();

		for( ; iterlist != iterlist_end ; ++iterlist)
		{
			if( (*iterlist) == pData ) 
			{
				mapiter->second.erase(iterlist);
				return;
			}
		}
	}
}

void Engine::CInfoSubject::Release(void)
{
	for(MAPDATALIST::iterator	iter = m_mapDataList.begin();
		iter != m_mapDataList.end(); ++iter)
	{
		iter->second.clear();
	}
	m_mapDataList.clear();
}