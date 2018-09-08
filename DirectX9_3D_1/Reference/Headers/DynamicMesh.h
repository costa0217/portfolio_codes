/*!
 * \file DynamicMesh.h
 * \date 2015/11/07 0:17
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CMesh를 상속받고 애니메이션이 있는 CDynamicMesh class로 CHierarchyLoader(Loader)과 연동하여 사용한다.
 *
 * \note
*/

#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "AnimationCtrl.h"
#include "Transform.h"

BEGIN(Engine)

class CHierarchyLoader;
class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& Instance);
	virtual ~CDynamicMesh(void);

public:
	virtual CResources* Clone_Resource(void);
	virtual HRESULT	Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void Render_MeshForShader(LPD3DXEFFECT pEffect, _bool bColliderDraw = false);

	virtual void Render_MeshForShaderShadow(LPD3DXEFFECT pEffect);
	void Render_MeshContaineShadowr(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pFrame);


	void Render_CollisionBox(_bool bDraw, _bool bSolid = false);

	void Move_Frame(const _float& fTimeDelta);
	void AnimationMove(const char* pFrameName, CTransform* pTransformCom); //애니메이션 싱크에 따라 움직이는 함수
	void AnimationMoveTwo(CTransform* pTransformCom, _bool bCompulsion, _bool bLeftAndRihgt, _bool bFrontAndBack, _float fDistanceSize = 1.f); //애니메이션 싱크에 따라 움직이는 함수
	void SetLeftAndRihgtMatrix(const char* pLFrameName, const char* pRFrameName);

public:
	void Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, _matrix* pParentMatrix);
	void SetUp_BoneMatrixPointer(D3DXFRAME_DERIVED* pFrame);
	void Render_MeshContainer(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pFrame);

public:
	void Set_AnimationSet(const _uint& iAniIdx);		//다른 애니메이션만 재생
	void Set_AnimationSetTwo(const _uint& iAniIdx);		//같은 애니메이션을 재생
	void Set_AnimationSetSameFinish(const _uint& iAniIdx);	

	_bool Check_EndPeriod(void);
	_bool Check_EndPeriodMulTime(_float fMultiplication);
	const _matrix* Get_FrameMatrix(const char* pFrameName);
	_uint Get_CurrentAniIdx(void) { return m_pAniCtrl->Get_CurrentAniIdx(); }

	void Set_List_FrameName(list<char*>* pListFrameName, D3DXFRAME_DERIVED* pFrame);	//매개변수로 넘겨주는 리스트에 DynmicMesh의 모든 본행렬을 돌면서 해당 행렬의 이름을 push해준다.
	LPD3DXFRAME Get_pRootFrame(void) { return m_pRootFrame; }
	_uint	Get_MaxAniIdx(void) { return m_pAniCtrl->Get_MaxAniIdx(); }	//해당 다이나믹메쉬가 갖고있는 애니메이션의 최대 개수를 반환하는 함수.
	_double Get_Period(void) { return m_pAniCtrl->Get_Period(); }

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName);

private:
	CHierarchyLoader*		m_pLoader;
	LPD3DXFRAME				m_pRootFrame;
	CAnimationCtrl*			m_pAniCtrl;	

	_uint					m_iCurrnetAniIdx;		//원하는 애니메이션을 돌리기 위한 변수

	//애니메이션 싱크 관련 변수들
	const _matrix*			m_pMatrix;
	_vec3					m_vCurrentOrignalPos;
	_vec3					m_vCalculationAfterPos;

	const _matrix*			m_pLMatrix;
	const _matrix*			m_pRMatrix;
	_vec3					m_vLCurrentOrignalPos;
	_vec3					m_vLCalculationAfterPos;
	_vec3					m_vRCurrentOrignalPos;
	_vec3					m_vRCalculationAfterPos;

	_ulong					m_dwCurrentTrack;
	_bool					m_bAnimationChange;
public:
	virtual _ulong Release(void);

};

END

#endif // DynamicMesh_h__