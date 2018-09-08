/*!
 * \file ResourceMgr.h
 * \date 2015/11/06 22:45
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Map STL�� ���ؼ� CResources class�� �����ϴ� Singleton Manager clsss.
 *
 * \note
*/

#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Defines.h"
#include "VIBuffer.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "PlayerDynamicMesh.h"
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)
private:
	explicit CResourceMgr(void);
	~CResourceMgr(void);

public: //getter
	CResources* Clone_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey);

public:
	HRESULT Reserve_ContainerSize(const _ushort& wSize); //��� ���ҽ� �� �����̳ʸ� ������ �����Ѵ�. 

public:
	HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx
		, CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey
		, const _ushort& wCntX = 0, const _ushort& wCntZ = 0, const _float& fItv = 1.f);

	HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev
		, const _ushort& wContainerIdx
		, CTexture::TEXTURETYPE eTextureType
		, const TCHAR* pResourceKey
		, const TCHAR* pFilePath
		, const _ushort& wCnt = 1);

	HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx
		, CMesh::MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName);

public:
	void Reset_Resource(const _ushort& wContainerIdx); //�ش� �ε����� ���ҽ� �� �����̳ʸ� Release �Ѵ�.

	HRESULT Delete_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey); //�ش� �ε����� ���ҽ� �� �����̳ʿ��� Key�� �ش��ϴ� ���ҽ��� ã�� �����Ѵ�.

private:


	boost::unordered_map<const TCHAR*, CResources*>*		m_pmapResource;
	typedef boost::unordered_map<const TCHAR*, CResources*>	MAPRESOURCE;




	_ushort									m_wReservedSize;

private:
	CResources* Find_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey);

public:
	void Release(void);
};

END

#endif // ResourceMgr_h__