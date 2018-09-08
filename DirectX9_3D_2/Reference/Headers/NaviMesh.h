#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CNaviCell;
class ENGINE_DLL CNaviMesh
{
public:
	DECLARE_SINGLETON(CNaviMesh)

private:
	CNaviMesh(void);
	~CNaviMesh(void);

public:
	void		Set_NaviCenterPos(_vec3* pvOutPos, _ulong dwIndex);
	void		Set_Jump(_bool bJump){m_bJump = bJump; }
public:
	void		CheckPickThird(D3DXVECTOR3 vPickPos);
	void		NextPickingNevi(D3DXVECTOR3 vPickPos, float fAttachLevel = 1.f);

	DWORD		FindIndex(D3DXVECTOR3 vPickPos);

	DWORD		FindDotIndex(D3DXVECTOR3 vPos, CNaviCell* pNaviCell);

	bool		LineCollsion(D3DXVECTOR3 vCurrentPos, D3DXVECTOR3 vDestPos, D3DXVECTOR3 pPoint1, D3DXVECTOR3 pPoint2);
	bool		FindNeighbor(D3DXVECTOR3 vMiraclePos, DWORD dwCurrentIdx, DWORD& dwNeighborIdx);
	D3DXVECTOR3	Search_SlideVec(CNaviCell* pNaviCell, D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir, int& rLineIndex);
	D3DXVECTOR3 Search_ReflcVec(D3DXVECTOR3* pSlideVec, D3DXVECTOR3* pDir);


	float		Search_StraightLine(D3DXVECTOR3 vLine1, D3DXVECTOR3 vLine2, D3DXVECTOR3 vDestLine);

	DWORD		MovingNaviMesh(D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir
		, const DWORD& dwCurrentIdx
		, const float& fSpeed
		, const float& fTimeDelta);

public:
	void		Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pGraphicDev, const DWORD& dwSize);
	HRESULT		Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);
	void		Link_Cell(void);

	void		TurnBack(void);
	void		AllDelete(void);
	void		Refresh_Data(void);

	void		Render_NaviMesh(void);

public:
	D3DXVECTOR3	Find_NaviPoint(D3DXVECTOR3 vPickPos);
	void		Modify_NaviPoint(_ushort iIdx, float fDataDelta);


	_float		Compute_Height(const _vec3& vObjPos, const DWORD& dwCurrentIdx);

private:
	void		Set_Edge(void);
	

public:
	void		Release(void);

public:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	DWORD				m_dwReserveSize;

	typedef vector<CNaviCell*>		VECCELL;
	VECCELL		m_vecNaviCell;

private:
	DWORD			m_dwIdxCnt;

	int				m_iPickCnt;
	D3DXVECTOR3		m_vPickPos[3];

	int				m_iFindNeviIdx;
	DWORD			m_dwPlayerIdx;

	_bool			m_bJump;

private:
	list<_vec3*>		m_SamePosPointsList;
	list<CNaviCell*>	m_ResetInfoList;

};

END

#endif // NaviMesh_h__