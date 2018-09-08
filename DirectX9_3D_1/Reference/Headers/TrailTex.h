#ifndef TrailTex_h__
#define TrailTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrailTex
	: public CVIBuffer
{
private:
	explicit CTrailTex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrailTex(void);

public:
	virtual CResources*	Clone_Resource(void);
	virtual HRESULT Create_Buffer(void);

public:
	static CTrailTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual _ulong Release(void);

};

END

#endif // TrailTex_h__
