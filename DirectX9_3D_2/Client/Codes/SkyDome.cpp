#include "stdafx.h"
#include "SkyDome.h"
#include "Export_Function.h"

CSkyDome::CSkyDome(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pMeshCom(NULL)
, m_pTransCom(NULL)
{

}

CSkyDome::~CSkyDome(void)
{

}

HRESULT CSkyDome::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SkyBox");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vScale = _vec3(0.003f, 0.003f, 0.003f);

	return S_OK;
}

HRESULT CSkyDome::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_SkyDome");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));


	return S_OK;
}

_int CSkyDome::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(1.f) * fTimeDelta;

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_PRIORITY, this);

	return 0;
}

void CSkyDome::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pEffect);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
	m_pEffect->SetTexture("g_SpecularTexture", NULL);
}

CSkyDome* CSkyDome::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyDome*		pSkyDome = new CSkyDome(pGraphicDev);

	if(FAILED(pSkyDome->Initialize()))
	{
		MSG_BOX("CSkyDome Create Faild");
		::Safe_Release(pSkyDome);
	}
	return pSkyDome;
}

void CSkyDome::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIX matCamera;
	D3DXMatrixInverse(&matCamera, NULL, &matView);
	m_pTransCom->m_vPosition = _vec3(matCamera._41, /*matCamera._42*/0.f, matCamera._43);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	//m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

_ulong CSkyDome::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
