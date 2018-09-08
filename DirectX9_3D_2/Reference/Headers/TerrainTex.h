/*!
 * \file TerrainTex.h
 * \date 2015/08/17 20:49
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

#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex
	: public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrainTex(void);
public:
	virtual CResources* Clone_Resource(void);

public:
	virtual HRESULT Create_Buffer(const _ulong& wCntX, const _ulong& wCntZ, const _ulong& wItv, vector<_vec3>* pVecPos);
public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const _ulong& wCntX, const _ulong& wCntZ, const _ulong& wItv
		, vector<_vec3>* pVecPos);
public:
	virtual _ulong Release(void);


};

END

#endif // TerrainTex_h__