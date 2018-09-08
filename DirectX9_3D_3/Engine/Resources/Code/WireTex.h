#ifndef WireTex_h__
#define WireTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CWireTex
	: public CVIBuffer
{
private:
	explicit CWireTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWireTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CWireTex* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // WireTex_h__
