#ifndef MonTrail_h__
#define MonTrail_h__
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMonTrail
	: public CVIBuffer
{
private:
	explicit CMonTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonTrail(void);

public:
	virtual CResources*	Clone_Resource(void);
	virtual HRESULT Create_Buffer(void);

public:
	static CMonTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual _ulong Release(void);

};
END

#endif // MonTrail_h__
