#include "stdafx.h"
#include "GroundHit.h"
#include "Export_Function.h"
#include "Mesh.h"
#include "Player.h"

CGroundHit::CGroundHit(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_bSwitch(false)

, m_fAccTime(0.f)
, m_vViewPos(0.f, 0.f, 0.f)
, m_bRender(false)
{

}

CGroundHit::~CGroundHit(void)
{

}

void CGroundHit::Get_Switch(void)
{
}

void CGroundHit::Set_Pos(_vec3 vPos)
{
	m_pTransCom->m_vPosition = vPos;
}

void CGroundHit::Set_DataInit(void)
{
	m_fAccTime = 0.f;
	m_pTransCom->m_vScale = _vec3(0.07f, 0.07f, 0.07f);
	m_bRender = false;
}

HRESULT CGroundHit::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlackAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);
	m_pTransCom->m_vScale = _vec3(0.07f, 0.07f, 0.07f);

	m_pFrame->Initialize(1, 1, 0.f);

	m_vColor = _vec4(0.7f, 0.4f, 0.f, 1.f);

	return S_OK;
}

HRESULT CGroundHit::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransCom", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Ground");
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	return S_OK;
}

_int CGroundHit::Update(const _float& fTimeDelta)
{
	if(!m_bSwitch)
	{
		return 0;
	}
	m_fAccTime += fTimeDelta;

	if(m_fAccTime > 0.3f)
	{
		m_bRender = true;
		m_pTransCom->m_vScale += _vec3(fTimeDelta, fTimeDelta, fTimeDelta) * 0.1f;
	}

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 


	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = (D3DX_PI * 2.f)* m_fAccTime * 2.f;


	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);
	Engine::CGameObject::Update(fTimeDelta);
	//m_vColor.w = 1.f - m_fAccTime;
	if(m_fAccTime > 0.6f)
	{
		m_bSwitch = false;
		m_bRender = false;
	}

	return 0;
}

void CGroundHit::Render(void)
{
	if(NULL == m_pEffect)
		return;

	if(!m_bSwitch) return;
	if(!m_bRender) return;

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

CGroundHit* CGroundHit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGroundHit*		pGameObject = new CGroundHit(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CGroundHit Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CGroundHit::Set_ContantTable(void)
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

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4(1.f, 1.f, 1.f, 1.f));	

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);

	m_pEffect->SetFloat("g_fAlpha", 1.f - (m_fAccTime * 2.2f));


	m_pEffect->SetVector("g_vColor", &m_vColor/*m_vColor*/);

}

_ulong CGroundHit::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}
	return dwRefCnt;
}

