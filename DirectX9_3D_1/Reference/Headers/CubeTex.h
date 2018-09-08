#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex
	: public CVIBuffer
{
private:
	CCubeTex(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCubeTex(void);

public:
	virtual HRESULT Create_Buffer(void);
	virtual CResources* Clone_Resource(void);

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual _ulong Release(void);
};

END

#endif // CubeTex_h__