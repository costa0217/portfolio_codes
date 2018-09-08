/*!
 * \file ResourcesMgr.h
 * \date 2015/07/06 12:21
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Include.h"
#include "VIBuffer.h"

BEGIN(Engine)

class CResources;

class ENGINE_DLL CResourcesMgr
{
public:
	DECLARE_SINGLETON(CResourcesMgr)

private:
	int		iLoadCnt;
public:
	int		GetCnt(){return iLoadCnt;}

private:
	CResourcesMgr(void);
	~CResourcesMgr(void);

public:
	CResources* CloneResource(RESOURCETYPE eResourceType, const wstring& wstrResourceKey);
	void GetVertexInfo(RESOURCETYPE eResourceType, const wstring& wstrResourceKey
		, void* pVertex);

public:
	void SetVertexInfo(RESOURCETYPE eResourceType, const wstring& wstrResourceKey
		, void* pVertex);

public:
	HRESULT AddBuffer(LPDIRECT3DDEVICE9 pDevice
		, RESOURCETYPE eResourceType
		, CVIBuffer::BUFFERTYPE eBufferType
		, const wstring& wstrResourceKey
		, const WORD& wCntX = 0, const WORD& wCntZ = 0, const WORD& wItv = 1);

	//문제가 있어서 하나 더 만듦
	HRESULT AddBuffer(LPDIRECT3DDEVICE9 pDevice
		, RESOURCETYPE eResourceType
		, CVIBuffer::BUFFERTYPE eBufferType
		, const wstring& wstrResourceKey, const wstring& wstrPath
		/*, const WORD& wCntX = 0, const WORD& wCntZ = 0, const WORD& wItv = 1*/);

	HRESULT AddTexture(LPDIRECT3DDEVICE9 pDevice
		, RESOURCETYPE eResourceType
		, TEXTURETYPE eTextureType
		, const wstring& wstrResourceKey
		, const wstring& wstrFilePath
		, const WORD& wCnt);

	HRESULT AddDataBuffer(LPDIRECT3DDEVICE9 pDevice 
		, RESOURCETYPE eResourceType 
		, CVIBuffer::BUFFERTYPE eBufferType 
		, const wstring& wstrResourceKey
		, vector<VTXCOL>& rVecVTX);

public:
	void ResetDynamic(void);
	void GetTerrainVtxCnt(RESOURCETYPE eResourceType, const wstring& wstrResourceKey, DWORD* pdwCntX, DWORD* pdwCntZ);

private:
	void Release(void);

private:
	typedef map<wstring, CResources*>		MAPRESOURCE;
	MAPRESOURCE		m_mapResource[RESOURCE_END];
};

END

#endif // ResourcesMgr_h__