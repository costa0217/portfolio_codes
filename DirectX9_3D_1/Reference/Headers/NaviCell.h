/*!
 * \file NaviCell.h
 * \date 2015/11/12 17:16
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef NaviCell_h__
#define NaviCell_h__

#include "Base.h"

BEGIN(Engine)

class CLine2D;

class ENGINE_DLL CNaviCell : public Engine::CBase
{
private:
	explicit CNaviCell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
public:
	virtual ~CNaviCell(void);

public:
	enum POINT {POINT_A, POINT_B, POINT_C, POINT_END,};
	enum LINE {LINE_AB, LINE_BC, LINE_CA, LINE_END,};
	enum NEIGHBOR {NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END,};

public:
	LPDIRECT3DDEVICE9		m_pDevice;
	CNaviCell*				m_pNeighbor[NEIGHBOR_END];
	CLine2D*				m_pLine2D[LINE_END];

private:
	D3DXVECTOR3				m_vPoint[POINT_END];
	LPD3DXLINE				m_pLine;
	_ulong					m_ulIndex;
	_vec3					m_vCenterPos;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	_ulong					m_dwVtxSize;
	_ulong					m_dwVtxCnt;
	_ulong					m_dwVtxFVF;


public:
	const D3DXVECTOR3* Get_Point(POINT PointFlag) {return &m_vPoint[PointFlag];}
	DWORD GetIndex(){return m_ulIndex;}

	CNaviCell* Get_Neighbor(NEIGHBOR eNeighbor){return m_pNeighbor[eNeighbor];}

	_vec3* Get_CenterPos(){return &m_vCenterPos;}


public:
	void Set_Neighbor(NEIGHBOR eNeighbor, CNaviCell* pNeighbor) 
	{m_pNeighbor[eNeighbor] = pNeighbor;};

	void Set_PosY(float fY, POINT PointFlag);
	void Set_Point(POINT PointFlag, _vec3* pPos);


public:
	HRESULT Init_Cell(LPDIRECT3DDEVICE9 pDevie, const DWORD& dwIdx);
	bool Compare_Point(const _vec3* pPointA, const _vec3* pPointB, CNaviCell* pNeighbor);
	bool Inside_Point(const _vec3* pPos);
	void Render(void);
	void Compute_CenterPos();
	bool CheckInFindNeighbor(_vec3* pPos, CNaviCell** ppNaviCell);
	CLine2D* Get_Line(_vec3* pPos);


public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 pDevice 
		, const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC
		, const _ulong& ulIdx);

public:
	virtual _ulong Release(void);
};

END

#endif // NaviCell_h__