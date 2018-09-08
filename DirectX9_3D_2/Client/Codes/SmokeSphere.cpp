#include "stdafx.h"
#include "SmokeSphere.h"
#include "Export_Function.h"
#include "Mesh.h"


CSmokeSphere::CSmokeSphere(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_bSwitch(false)

, m_fAccTime(0.f)
, m_vViewPos(0.f, 0.f, 0.f)
, m_fAlpha(1.f)
, m_iRand(0)
{

}

CSmokeSphere::~CSmokeSphere(void)
{

}

void CSmokeSphere::Get_Switch(void)
{

}

void CSmokeSphere::Set_Pos(_vec3 vPos)
{

}

void CSmokeSphere::Set_DataInit(void)
{

}

HRESULT CSmokeSphere::Initialize(_vec3 vPos)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = vPos + _vec3(0.f, 1.f, 0.f);

	m_pFrame->Initialize(8, 4, 40.f);

	m_pTransCom->m_vScale =_vec3(0.08f, 0.08f, 0.08f);

	m_iRand = rand()%5 - 2;


	return S_OK;
}

HRESULT CSmokeSphere::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransCom", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Smoke");
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	return S_OK;
}

_int CSmokeSphere::Update(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += fTimeDelta * m_iRand;

	if(m_fAccTime > 3.f)
	{
		m_fAlpha -= fTimeDelta;
	}
	if(m_fAccTime > 4.f)
	{
		return -1;
	}
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);
	Engine::CGameObject::Update(fTimeDelta);

	

	return 0;
}

void CSmokeSphere::Render(void)
{
	if(NULL == m_pEffect)	return;
	//if(!m_bSwitch)	return;

	if(!Engine::Get_Frustum()->SphereInFrustum(&m_pTransCom->m_vPosition, 3.f))
		return;

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

CSmokeSphere* CSmokeSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSmokeSphere*		pEffect = new CSmokeSphere(pGraphicDev);

	if(FAILED(pEffect->Initialize(vPos)))
	{
		MSG_BOX("CSmokeSphere Create Failed");
		::Safe_Release(pEffect);
	}
	return pEffect;
}

void CSmokeSphere::Set_ContantTable(void)
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

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	

	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);
}

void CSmokeSphere::Set_SettingData(void)
{

}

_ulong CSmokeSphere::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}
	return dwRefCnt;
}

