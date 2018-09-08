#ifndef QuadTree_h__
#define QuadTree_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CFrustum;

class ENGINE_DLL CQuadTree
{
public:
	DECLARE_SINGLETON(CQuadTree)

public:
	enum CHILD {CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END};
	enum CORNER {CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END};
	enum NEIGHBOR {NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END};

private:
	CQuadTree(void);

public:
	~CQuadTree(void);

public:
	HRESULT InitQuadTree(const VTXTEX* pTerrainVtx, const int& iCntX, const int& iCntZ);
	void IsinFrustum(const VTXTEX* pTerrainVtx, const CFrustum* pFrustum);
	void CullingToQuadTree(LPDIRECT3DDEVICE9 pDevice, const VTXTEX* pTerrainVtx, const CFrustum* pFrustum
		, INDEX32* pIndex, DWORD* pdwTriCnt );

public:
	void SetChild(const VTXTEX* pTerrainVtx);
	CQuadTree* MakeChild(CHILD eChild);
	bool LevelofDetail(LPDIRECT3DDEVICE9 pDevice, const VTXTEX* pTerrainVtx);
	void Release(void);

private:
	bool MakeNeighbor(NEIGHBOR eNeighbor, int* pLT, int* pRT, int* pLB, int* pRB
		, const int& iVtxCntX, const int& iVtxCntZ);
	CQuadTree* FindNeighbor(const VTXTEX* pTerrainVtxInfo
		, const int& iLT, const int& iRT, const int& iLB, const int& iRB,const int& iCenter);

public:
	HRESULT InitNeighbor(const VTXTEX* pTerrainVtxInfo, CQuadTree* pParentQuad
		, const int& iVtxCntX, const int& iVtxCntZ);

private:
	CQuadTree*		m_pChild[CHILD_END];
	CQuadTree*		m_pNeighbor[NEIGHBOR_END];
	int				m_iCorner[CORNER_END];
	int				m_iCenter;
	float			m_fRadius;
	bool			m_isIn;
};

END

#endif // QuadTree_h__