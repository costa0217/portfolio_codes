#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum
{
public:
	DECLARE_SINGLETON(CFrustum)

private:
	CFrustum(void);
	~CFrustum(void);

public:
	HRESULT InitFrustum(LPDIRECT3DDEVICE9 pDevice);
	void Transform_Frustum_ToWorld(LPDIRECT3DDEVICE9 pDevice);
	void Transform_Frustum_Local(const D3DXMATRIX* pWorld);
	void CullingForTerrain(LPDIRECT3DDEVICE9 pDevice, INDEX16* pIndex, DWORD* pTriCnt
		, const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);

public:
	bool SphereInFrustum(const D3DXVECTOR3* pCenter, const float& fRadius) const;
	bool VertexInFrustum(const D3DXVECTOR3* pPoint) const;

private:
	void Release(void);

private:
	D3DXVECTOR3		m_vOriPoint[8];
	D3DXVECTOR3		m_vConPoint[8];

	D3DXPLANE		m_Plane[6];
};

END

#endif // Frustum_h__