#ifndef RcTex2_h__
#define RcTex2_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex2
	: public CVIBuffer
{
private:
	explicit CRcTex2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRcTex2(void);

public:
	virtual HRESULT Create_Buffer(void);
	virtual CResources* Clone_Resource(void);

public:
	static CRcTex2* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

public:
	virtual _ulong Release(void);
};

END

#endif // RcTex2_h__