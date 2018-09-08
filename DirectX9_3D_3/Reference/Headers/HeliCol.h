#ifndef HeliCol_h__
#define HeliCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CHeliCol
	: public CVIBuffer
{
private:
	explicit CHeliCol(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CHeliCol(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(int iAllVtxCnt, vector<VTXCOL>& vecVTX);	

public:
	static CHeliCol* Create(LPDIRECT3DDEVICE9 pDevice,
		int iAllVtxCnt, vector<VTXCOL>& vecVTX);
};

END

#endif // HeliCol_h__
