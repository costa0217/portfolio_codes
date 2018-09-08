#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pPlaneActor(NULL)
, m_pTextureNormalCom(NULL)
{
	
}

CTerrain::~CTerrain(void)
{

}

_int CTerrain::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CTerrain::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();

	m_pEffect->End();

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCETYPE eResourceType)
{
	CTerrain* pGameObject = new CTerrain(pGraphicDev);

	if(FAILED(pGameObject->Initialize(eResourceType)))
	{
		MSG_BOX("CTerrain Create Faild");
		::Safe_Release(pGameObject);
	}

	return pGameObject;
}

HRESULT CTerrain::Initialize(RESOURCETYPE eResourceType)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Terrain");
	if(NULL == m_pEffect)
		return E_FAIL;

	m_eResourceType =eResourceType;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pPlaneActor = Engine::CreateGroundPlane();

	return S_OK;
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;
	
	//VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(m_eResourceType, L"Buffer_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(m_eResourceType, L"Texture_Terrain");
	if(NULL == pComponent)		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	pComponent = m_pTextureNormalCom = (Engine::CTexture*)Engine::Clone_Resource(m_eResourceType, L"Texture_TerrainNor");
	if(NULL == pComponent)		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TextureNor", pComponent));

	return S_OK;
}

void CTerrain::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
	m_pTextureNormalCom->Set_ConstantTable(m_pEffect, "g_NormalTexture");	
}

_ulong CTerrain::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}