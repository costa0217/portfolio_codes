#include "ResourcesMgr.h"
#include "TriColor.h"
#include "RcColor.h"
#include "Texture.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "CubePlayer.h"
#include "WireTex.h"
#include "HeliCol.h"

IMPLEMENT_SINGLETON(Engine::CResourcesMgr)

Engine::CResourcesMgr::CResourcesMgr(void)
: iLoadCnt(0)
{

}

Engine::CResourcesMgr::~CResourcesMgr(void)
{
	Release();
}

Engine::CResources* Engine::CResourcesMgr::CloneResource(RESOURCETYPE eResourceType, const wstring& wstrResourceKey)
{
	MAPRESOURCE::iterator	iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if(iter == m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"복사할 대상을 찾을수 없음");
		return NULL;
	}

	return iter->second->CloneResource();
}

void Engine::CResourcesMgr::GetVertexInfo(RESOURCETYPE eResourceType, const wstring& wstrResourceKey , void* pVertex)
{
	MAPRESOURCE::iterator		iter = m_mapResource[eResourceType].find(wstrResourceKey);
	if(iter == m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"찾으려는 버퍼 없음");
		return ;
	}
	((CVIBuffer*)iter->second)->GetVtxInfo(pVertex);
}

void Engine::CResourcesMgr::SetVertexInfo(RESOURCETYPE eResourceType, const wstring& wstrResourceKey , void* pVertex)
{
	MAPRESOURCE::iterator		iter = m_mapResource[eResourceType].find(wstrResourceKey);
	if(iter == m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"찾으려는 버퍼 없음");
		return ;
	}
	((CVIBuffer*)iter->second)->SetVtxInfo(pVertex);
}

HRESULT Engine::CResourcesMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice 
										 , RESOURCETYPE eResourceType 
										 , CVIBuffer::BUFFERTYPE eBufferType 
										 , const wstring& wstrResourceKey
										 , const WORD& wCntX, const WORD& wCntZ, const WORD& wItv)
{
	MAPRESOURCE::iterator		iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if(iter != m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"이미 추가 되어 있는 버퍼");
		return E_FAIL;
	}

	CResources*		pResource = NULL;


	++iLoadCnt;

	switch(eBufferType)
	{
	case CVIBuffer::BUFFER_TRICOL:
		pResource = CTriColor::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCCOL:
		pResource = CRcColor::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCTEX:
		pResource = CRcTex::Create(pDevice);
		break;

	//case CVIBuffer::BUFFER_TERRAINTEX:
	//	pResource = CTerrainTex::Create(pDevice, wCntX, wCntZ, wItv);
	//	break;

	case CVIBuffer::BUFFER_CUBETEX:
		pResource = CCubeTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_CUBEPLAYER:
		pResource = CCubePlayer::Create(pDevice);
		break;
	case CVIBuffer::BUFFER_WIRETEX:
		pResource = CWireTex::Create(pDevice);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_mapResource[eResourceType].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));

	return S_OK;
}

HRESULT Engine::CResourcesMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice 
										 , RESOURCETYPE eResourceType 
										 , CVIBuffer::BUFFERTYPE eBufferType 
										 , const wstring& wstrResourceKey, const wstring& wstrPath /*= L""*/
										 /*, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv*/)
{
	MAPRESOURCE::iterator		iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if(iter != m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"이미 추가 되어 있는 버퍼");
		return E_FAIL;
	}

	++iLoadCnt;

	CResources*		pResource = NULL;

	switch(eBufferType)
	{
	case CVIBuffer::BUFFER_TRICOL:
		pResource = CTriColor::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCCOL:
		pResource = CRcColor::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCTEX:
		pResource = CRcTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_TERRAINTEX:
		pResource = CTerrainTex::Create(pDevice, wstrPath/*, wCntX, wCntZ, wItv*/);
		break;

	case CVIBuffer::BUFFER_CUBETEX:
		pResource = CCubeTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_CUBEPLAYER:
		pResource = CCubePlayer::Create(pDevice);
		break;
	case CVIBuffer::BUFFER_WIRETEX:
		pResource = CWireTex::Create(pDevice);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_mapResource[eResourceType].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));

	return S_OK;
}

HRESULT Engine::CResourcesMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice , RESOURCETYPE eResourceType , TEXTURETYPE eTextureType , const wstring& wstrResourceKey , const wstring& wstrFilePath , const WORD& wCnt)
{
	MAPRESOURCE::iterator	iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if(iter != m_mapResource[eResourceType].end())
		return E_FAIL;

	++iLoadCnt;

	CResources*		pResource = CTexture::Create(pDevice, eTextureType, wstrFilePath, wCnt);
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_mapResource[eResourceType].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));
	return S_OK;
}

HRESULT Engine::CResourcesMgr::AddDataBuffer(LPDIRECT3DDEVICE9 pDevice 
											 , RESOURCETYPE eResourceType 
											 , CVIBuffer::BUFFERTYPE eBufferType 
											 , const wstring& wstrResourceKey
											 , vector<VTXCOL>& rVecVTX)
{
	MAPRESOURCE::iterator		iter = m_mapResource[eResourceType].find(wstrResourceKey);

	if(iter != m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"이미 추가 되어 있는 버퍼");
		return E_FAIL;
	}

	CResources*		pResource = NULL;
	int vtxCnt = rVecVTX.size();

	++iLoadCnt;

	switch(eBufferType)
	{
	//HELI
	case CVIBuffer::BUFFER_HELICOL:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_CHINOOK:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;


	case CVIBuffer::BUFFER_PROPELL:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_MINIPROPELL:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	//WIRE
	case CVIBuffer::BUFFER_HOOK:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;


	/* Monster */

	//BUNKER
	case CVIBuffer::BUFFER_BUNKER:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;


	//RETONA
	case CVIBuffer::BUFFER_RETONA:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	//DODGE
	case CVIBuffer::BUFFER_DODGE:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	//AIRGUN
	case CVIBuffer::BUFFER_AIRGUNBODY:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;
	case CVIBuffer::BUFFER_AIRGUNHEAD:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;
	case CVIBuffer::BUFFER_AIRGUNPAW:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	//TANK
	case CVIBuffer::BUFFER_TANK:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_TANKPAW:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_TANKPAWSIN:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	/* WEAPON */
	case CVIBuffer::BUFFER_GUN:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_GATLINGGUN:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_LAUNCHER:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	/* BULLET */
	case CVIBuffer::BUFFER_BULLET:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_MULTIBULLET:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_MISILE:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_BOMB:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	/* Object */

	case CVIBuffer::BUFFER_FENCE:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_TENT:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_WIREFENCE:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_BLACKCLOUD:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_FRONTCLOUD:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_MIDCLOUD:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_SMALLTREE:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	case CVIBuffer::BUFFER_TALLTREE:
		pResource = CHeliCol::Create(pDevice, vtxCnt, rVecVTX);
		break;

	}

	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_mapResource[eResourceType].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));

	return S_OK;

}


void Engine::CResourcesMgr::ResetDynamic(void)
{
	for_each(m_mapResource[RESOURCE_DYNAMIC].begin(), m_mapResource[RESOURCE_DYNAMIC].end()
		, CDelete_Map());
	m_mapResource[RESOURCE_DYNAMIC].clear();
}

void Engine::CResourcesMgr::Release(void)
{
	for(int i = 0; i < RESOURCE_END; ++i)
	{
		for_each(m_mapResource[i].begin(), m_mapResource[i].end(), CDelete_Map());
		m_mapResource[i].clear();
	}
}

void Engine::CResourcesMgr::GetTerrainVtxCnt(RESOURCETYPE eResourceType, const wstring& wstrResourceKey, DWORD* pdwCntX, DWORD* pdwCntZ)
{
	MAPRESOURCE::iterator		iter = m_mapResource[eResourceType].find(wstrResourceKey);
	if(iter == m_mapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"찾으려는 버퍼 없음");
		return ;
	}
	((CTerrainTex*)iter->second)->GetVtxCnt(pdwCntX, pdwCntZ);
}
