#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex
	: public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRcTex(void);

public:
	virtual HRESULT Create_Buffer(void);
	virtual CResources* Clone_Resource(void);

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

public:
	virtual _ulong Release(void);
};

END

#endif // RcTex_h__