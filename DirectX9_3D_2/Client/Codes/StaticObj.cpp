#include "stdafx.h"
#include "StaticObj.h"

#include "Export_Function.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "NaviMesh.h"

#include "Defines.h"

CStaticObj::CStaticObj(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pEffect(NULL)
, m_pFrustum(Engine::Get_Frustum())
, m_pvMyMin(NULL)
, m_pvMyMax(NULL)
, m_fRadian(0.f)
{
	ZeroMemory(m_wstrMeshName, sizeof(TCHAR) * 128);
	lstrcpy(m_wstrMeshName,pwstrMeshName);
}

CStaticObj::~CStaticObj(void)
{

}

HRESULT CStaticObj::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_MeshesStatic");
	if(NULL == m_pEffect)
		return E_FAIL;

	FAILED_CHECK(AddComponent());


	return S_OK;
}

_int CStaticObj::Update(const _float& fTimeDelta)
{

	if(m_fRadian == 0)
	{
		m_pMeshCom->Get_pMinMax(m_pvMyMin, m_pvMyMax);
		m_fRadian = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin));

		m_fRadian *= m_pTransCom->m_vScale.x * 0.5f;
	}
	


	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CStaticObj::Render(void)
{
	if(NULL == m_pEffect)
		return;


	if(!m_pFrustum->SphereInFrustum(&m_pTransCom->m_vPosition, m_fRadian))
		return;

	//return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
	m_pEffect->SetTexture("g_SpecularTexture", NULL);
}



CStaticObj* CStaticObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName)
{
	CStaticObj*	pMeshObj = new CStaticObj(pGraphicDev, pwstrMeshName);
	if(FAILED(pMeshObj->Initialize()))
		Engine::Safe_Release(pMeshObj);

	return pMeshObj;
}

HRESULT CStaticObj::AddComponent(void)
{
	HRESULT hr = NULL;

	Engine::CComponent*		pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, m_wstrMeshName);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	return S_OK;
}


_ulong CStaticObj::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

void CStaticObj::SetPos(D3DXVECTOR3 vMousePos)
{
	m_pTransCom->m_vPosition = vMousePos;
}

void CStaticObj::SetScale(D3DXVECTOR3 vScale)
{
	m_pTransCom->m_vScale = vScale;
}

void CStaticObj::SetRotation(D3DXVECTOR3 vRotation)
{
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = vRotation.x;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = vRotation.y;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = vRotation.z;
}


void CStaticObj::Set_ContantTable(void)
{	
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);	

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	D3DXMATRIX matCamera;
	D3DXMatrixInverse(&matCamera, NULL, &matView);
	_vec4 vCameraPos = _vec4(matCamera._41, matCamera._42, matCamera._43, 1.f);

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	
	m_pEffect->SetVector("g_vLightSpecular", &_vec4((_float*)&pLightInfo->Specular));	

	m_pEffect->SetVector("g_vWorldCamPos", &vCameraPos);
}
