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

#ifndef PlayerDynamicMesh_h__
#define PlayerDynamicMesh_h__

#include "Mesh.h"
#include "AnimationCtrl.h"
#include "Engine_Defines.h"
#include "Transform.h"

BEGIN(Engine)

class CTexture;
class CHierarchyLoader;
class ENGINE_DLL CPlayerDynamicMesh : public CMesh
{
public:
	enum CLOTHTYPE { CTYPE_UPPER, CTYPE_LOWER, CTYPE_HAND, CTYPE_FOOT, CTYPE_END };
private:
	explicit CPlayerDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerDynamicMesh(const CPlayerDynamicMesh& Instance);
	virtual ~CPlayerDynamicMesh(void);

public:
	void Set_AnimationSet(const _uint& iAniIdx);
	void Set_AnimationSetTwo(const _uint& iAniIdx);
	void Set_AnimationSetSameFinish(const _uint& iAniIdx);
	void Set_AnimationSetThree(const _uint& iAniIdx);

	void Move_Frame(const _float& fTimeDelta);
	void AnimationMove(const char* pFrameName, CTransform* pTransformCom); //애니메이션 싱크에 따라 움직이는 함수
	void AnimationMoveTwo(const char* pLFrameName, const char* pRFrameName, CTransform* pTransformCom); //애니메이션 싱크에 따라 움직이는 함수

	_bool Check_EndPeriod(void);
	_bool Check_EndPeriodMulTime(_float fMultiplication);
	const _matrix* Get_FrameMatrix(const char* pFrameName);
	_uint Get_CurrentAniIdx(void) { return m_pAniCtrl->Get_CurrentAniIdx(); }

	void Set_List_FrameName(list<char*>* pListFrameName, D3DXFRAME_DERIVED* pFrame);	//매개변수로 넘겨주는 리스트에 DynmicMesh의 모든 본행렬을 돌면서 해당 행렬의 이름을 push해준다.
	LPD3DXFRAME Get_pRootFrame(void) { return m_pRootFrame; }
	_uint	Get_MaxAniIdx(void) { return m_pAniCtrl->Get_MaxAniIdx(); }	//해당 다이나믹메쉬가 갖고있는 애니메이션의 최대 개수를 반환하는 함수.
	_double Get_Period(void) { return m_pAniCtrl->Get_Period(); }
public:
	virtual CResources* Clone_Resource(void);
	virtual HRESULT	Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void Render_MeshForShader(LPD3DXEFFECT pEffect, Engine::CTexture* pTexture, _bool bColliderDraw = false);
	void Render_CollisionBox(_bool bDraw, _bool bSolid = false);

public:
	void Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, _matrix* pParentMatrix);
	void SetUp_BoneMatrixPointer(D3DXFRAME_DERIVED* pFrame);

	void Render_MeshContainer(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pFrame, Engine::CTexture* pTexture);
	
	virtual void Render_MeshForShaderShadow(LPD3DXEFFECT pEffect);
	void Render_MeshContainerShadow(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pFrame);

	virtual void Render_MeshForShaderVelocity(LPD3DXEFFECT pEffect);
	void Render_MeshContainerVelocity(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pFrame);

	///////////////////////////////////////////////////////////////
	D3DXMESHCONTAINER_DERIVED* m_pMeshCon;	// 의상의 반환용 메쉬컨테이너
	D3DXMESHCONTAINER_DERIVED* GetMeshCon() { return m_pMeshCon; }

	enum		DRAWBODY { DB_BODY, DB_HAND, DB_LOWER, DB_FOOT, DB_END };
	_bool		m_bDrawBody[DB_END];

	_bool		m_bDraw;	// 허리 그릴지
	_bool		m_bDrawHand2; // 팔뚝 그릴지

	void		Undraw() { m_bDraw = true; }
	void		Draw() { m_bDraw = false; }
	void		Draw_Hand2() { m_bDrawHand2 = false; }
	void		Undraw_Hand2() { m_bDrawHand2 = true; }

	void		Set_DrawBody(int iIdx) { m_bDrawBody[iIdx] = false; }

	_bool		m_bNew[DB_END];
	int			m_iChangePart;

	D3DXMESHCONTAINER_DERIVED* m_pHair;		// 머리 메쉬컨테이너
	D3DXMESHCONTAINER_DERIVED* m_pUpper;	// 상의 메쉬컨테이너
	D3DXMESHCONTAINER_DERIVED* m_pLower;	// 하의 메쉬컨테이너
	D3DXMESHCONTAINER_DERIVED* m_pHand;		// 장갑 메쉬컨테이너
	D3DXMESHCONTAINER_DERIVED* m_pFoot;		// 신발 메쉬컨테이너

	_matrix	matHairColor;	// 상의 메쉬컨테이너 색상
	_matrix	matUpperColor;	// 상의 메쉬컨테이너 색상
	_matrix	matLowerColor;	// 하의 메쉬컨테이너 색상
	_matrix	matHandColor;	// 장갑 메쉬컨테이너 색상
	_matrix	matFootColor;	// 신발 메쉬컨테이너 색상

	// 색상 설정
	void Set_HairColor(_matrix matHair) { memcpy(matHairColor, matHair, sizeof(_matrix)); }
	void Set_UpperColor(_matrix matUpper) { memcpy(matUpperColor, matUpper, sizeof(_matrix)); }
	void Set_LowerColor(_matrix matLower) { memcpy(matLowerColor, matLower, sizeof(_matrix)); }
	void Set_HandColor(_matrix matHand) { memcpy(matHandColor, matHand, sizeof(_matrix)); }
	void Set_FootColor(_matrix matFoot) { memcpy(matFootColor, matFoot, sizeof(_matrix)); }

	// 의상 메쉬컨테이너 초기 설정
	void Set_ClothMesh(D3DXFRAME_DERIVED* pFrame);
	// 의상 메쉬컨테이너 변경
	void Change_ClothMesh(int index, D3DXMESHCONTAINER_DERIVED* pMeshCon);
	// 의상 메쉬컨테이너 행렬 업데이트
	void Update_ClothMeshMatrix(D3DXFRAME_DERIVED* pFrame);
	// 메쉬컨테이너 복사
	void Copy_MeshContainer(D3DXMESHCONTAINER_DERIVED* pDest, D3DXMESHCONTAINER_DERIVED* pSour);
	///////////////////////////////////////////////////////////////

public:
	static CPlayerDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName);

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