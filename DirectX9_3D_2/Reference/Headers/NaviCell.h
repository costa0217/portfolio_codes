#ifndef NaviCell_h__
#define NaviCell_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CLine2D;
class ENGINE_DLL CNaviCell
{
public:
	enum POINT {POINT_A, POINT_B, POINT_C, POINT_END,};
	enum LINE {LINE_AB, LINE_BC, LINE_CA, LINE_END,};
	enum NEIGHBOR {NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END,};


private:
	explicit CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);

public:
	~CNaviCell(void);

public:
	const D3DXVECTOR3* Get_Point(POINT PointFlag) {return &m_vPoint[PointFlag];}

public:
	void Set_Neighbor(NEIGHBOR eNeighbor, CNaviCell* pNeighbor) 
	{m_pNeighbor[eNeighbor] = pNeighbor;}

public:
	HRESULT Init_Cell(LPDIRECT3DDEVICE9 pDevie, const DWORD& dwIdx);
	bool Compare_Point(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, CNaviCell* pNeighbor);
	void Render(D3DXCOLOR Color);

public:
	bool	Check_IsIn(D3DXVECTOR3 vPickPos);

	void	Set_Info(void);

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 pGraphicDev 
		, const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC
		, const DWORD& dwIdx);

private:
	void Release(void);

public:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	CNaviCell*				m_pNeighbor[NEIGHBOR_END];
	CLine2D*				m_pLine2D[LINE_END];

	vector<CNaviCell*>		m_VecPoint[POINT_END];


public:
	D3DXVECTOR3				m_vPoint[POINT_END];
	D3DXVECTOR3				m_vEdge[LINE_END];
	D3DXVECTOR3				m_vCenterPos;
	LPD3DXLINE				m_pLine;
	DWORD					m_dwIndex;


};

END

#endif // NaviCell_h__