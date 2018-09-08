#ifndef CubePlayer_h__
#define CubePlayer_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubePlayer
	: public CVIBuffer
{
private:
	explicit CCubePlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCubePlayer(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CCubePlayer* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // CubePlayer_h__
