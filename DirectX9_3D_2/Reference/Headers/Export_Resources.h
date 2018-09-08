/*!
 * \file Export_Resources.h
 * \date 2015/04/05 5:18
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
#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourceMgr.h"

BEGIN(Engine)
//For.Getter------------------------------------------------------------------------------------------------------------
inline CResourceMgr* Get_ResourceMgr(void);


//For.Setter------------------------------------------------------------------------------------------------------------



//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey, const _ulong& wCntX = 0, const _ulong& wCntZ = 0, const _ulong& wItv = 1, vector<_vec3>* pVecPos = NULL);
inline HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pDevice, const _ushort& wContainerIdx, const TCHAR* pResourceKey, CTexture::TEXTURETYPE eTextureType, const TCHAR* pFilePath, const _ushort& wCnt = 1);
inline HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pDevice, const _ushort& wContainerIdx, CMesh::MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName);
inline CResources* Clone_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey);
inline void Reset_Resource(const _ushort& wContainerIdx);

//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_Resource(void);

#include "Export_Resources.inl"

END

#endif // Export_Resources_h__