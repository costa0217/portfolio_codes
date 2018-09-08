/*!
 * \file DynamicMesh.h
 * \date 2015/11/07 0:17
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CMesh�� ��ӹް� �ִϸ��̼��� �ִ� CDynamicMesh class�� CHierarchyLoader(Loader)�� �����Ͽ� ����Ѵ�.
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
	void AnimationMove(const char* pFrameName, CTransform* pTransformCom); //�ִϸ��̼� ��ũ�� ���� �����̴� �Լ�
	void AnimationMoveTwo(CTransform* pTransformCom, _bool bCompulsion, _bool bLeftAndRihgt, _bool bFrontAndBack, _float fDistanceSize = 1.f); //�ִϸ��̼� ��ũ�� ���� �����̴� �Լ�
	void SetLeftAndRihgtMatrix(const char* pLFrameName, const char* pRFrameName);

public:
	void Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, _matrix* pParentMatrix);
	void SetUp_BoneMatrixPointer(D3DXFRAME_DERIVED* pFrame);
	void Render_MeshContainer(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pFrame);

public:
	void Set_AnimationSet(const _uint& iAniIdx);		//�ٸ� �ִϸ��̼Ǹ� ���
	void Set_AnimationSetTwo(const _uint& iAniIdx);		//���� �ִϸ��̼��� ���
	void Set_AnimationSetSameFinish(const _uint& iAniIdx);	

	_bool Check_EndPeriod(void);
	_bool Check_EndPeriodMulTime(_float fMultiplication);
	const _matrix* Get_FrameMatrix(const char* pFrameName);
	_uint Get_CurrentAniIdx(void) { return m_pAniCtrl->Get_CurrentAniIdx(); }

	void Set_List_FrameName(list<char*>* pListFrameName, D3DXFRAME_DERIVED* pFrame);	//�Ű������� �Ѱ��ִ� ����Ʈ�� DynmicMesh�� ��� ������� ���鼭 �ش� ����� �̸��� push���ش�.
	LPD3DXFRAME Get_pRootFrame(void) { return m_pRootFrame; }
	_uint	Get_MaxAniIdx(void) { return m_pAniCtrl->Get_MaxAniIdx(); }	//�ش� ���̳��͸޽��� �����ִ� �ִϸ��̼��� �ִ� ������ ��ȯ�ϴ� �Լ�.
	_double Get_Period(void) { return m_pAniCtrl->Get_Period(); }

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName);

private:
	CHierarchyLoader*		m_pLoader;
	LPD3DXFRAME				m_pRootFrame;
	CAnimationCtrl*			m_pAniCtrl;	

	_uint					m_iCurrnetAniIdx;		//���ϴ� �ִϸ��̼��� ������ ���� ����

	//�ִϸ��̼� ��ũ ���� ������
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