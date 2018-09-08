#ifndef PlayerEyeTex_h__
#define PlayerEyeTex_h__
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPlayerEyeTex
	: public CVIBuffer
{
private:
	explicit CPlayerEyeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerEyeTex(void);

public:
	virtual CResources*	Clone_Resource(void);
	virtual HRESULT Create_Buffer(void);

public:
	static CPlayerEyeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual _ulong Release(void);

};
END

#endif // PlayerEyeTex_h__
