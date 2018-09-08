/*!
 * \file NaviMesh.h
 * \date 2015/11/12 17:25
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
#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "NaviCell.h"
#include "Base.h"

BEGIN(Engine)

class CNaviCell;

class ENGINE_DLL CNaviMesh : public Engine:: CBase
{
public:
	DECLARE_SINGLETON(CNaviMesh)

private:
	explicit CNaviMesh(void);
public:
	virtual ~CNaviMesh(void);

private:
	LPDIRECT3DDEVICE9	m_pDevice;

	typedef list<CNaviCell*>	LISTCELL;
	LISTCELL					m_listNaviCell;

	_ulong	m_ulIdxCnt;

public:
	void Ready_GraphicDev(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Add_Cell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	void Link_Cell(void);
	void Make_Cell(_vec3* pCenterPos, const _vec3* pPos, _float fMergeDist);

	void Render_NaviMesh(void);

	LISTCELL* Get_listNaviCell(){return &m_listNaviCell;}
	
	_bool bInsideCell(_vec3* pPos, _vec3* pCenterPos);//삼각형에 존재하는 pPos면 true, Center값 대입.
	CNaviCell* Get_LastCell(){return m_listNaviCell.back();} // 마지막에 추가된 삼각형 반환.
	void Delete_Cell(_vec3* pPos); //pPos가 Cell 내부에 존재하면 그 Cell 삭제.
	void Move_Cell(_vec3* pPos, _float fMergeDist); //pPos에 근접하는 포인트들을 움직인다.
	void Clear_Cell(); // 불러오기를 위해 모든 Cell을 삭제한다.
	void Delete_LastCell(); // 리스트에서 마지막 Cell을 삭제한다.
	void Find_PointsFromCenterPos(_vec3 vCenterPos, _vec3* pPoints);//centerpos를 이용해 Cell의 3개 점을 구한다.
	void Set_HeightPoint(_vec3* pPos, _float fHeight); // 점 높이조절
	void Set_HeightTri(_vec3* pPos, _float fHeight); // cell 높이조절
	void Set_AllCellHeight(_float fHeight);
	
public:
	virtual _ulong Release(void);


};
END
#endif // NaviMesh_h__