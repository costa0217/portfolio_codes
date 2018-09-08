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
	virtual HRESULT Create_Buffer(const _ushort& wCntX, const _ushort& wCntZ, const _float& wItv);
	virtual CResources* Clone_Resource(void);

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const _ushort& wCntX, const _ushort& wCntZ, const _float& fItv);

public:
	virtual _ulong Release(void);
};

END

#endif // TerrainTex_h__