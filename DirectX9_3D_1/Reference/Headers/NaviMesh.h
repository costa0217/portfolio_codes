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
	
	_bool bInsideCell(_vec3* pPos, _vec3* pCenterPos);//�ﰢ���� �����ϴ� pPos�� true, Center�� ����.
	CNaviCell* Get_LastCell(){return m_listNaviCell.back();} // �������� �߰��� �ﰢ�� ��ȯ.
	void Delete_Cell(_vec3* pPos); //pPos�� Cell ���ο� �����ϸ� �� Cell ����.
	void Move_Cell(_vec3* pPos, _float fMergeDist); //pPos�� �����ϴ� ����Ʈ���� �����δ�.
	void Clear_Cell(); // �ҷ����⸦ ���� ��� Cell�� �����Ѵ�.
	void Delete_LastCell(); // ����Ʈ���� ������ Cell�� �����Ѵ�.
	void Find_PointsFromCenterPos(_vec3 vCenterPos, _vec3* pPoints);//centerpos�� �̿��� Cell�� 3�� ���� ���Ѵ�.
	void Set_HeightPoint(_vec3* pPos, _float fHeight); // �� ��������
	void Set_HeightTri(_vec3* pPos, _float fHeight); // cell ��������
	void Set_AllCellHeight(_float fHeight);
	
public:
	virtual _ulong Release(void);


};
END
#endif // NaviMesh_h__