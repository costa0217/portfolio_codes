#include "stdafx.h"
#include "Terrain.h"
#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pMtrlCom(NULL)
, m_pBufferCom(NULL)
, m_pTextureCom(NULL)
, m_pEffect(NULL)
, m_pTerrainVtxInfo(NULL)
, m_pIndex(NULL)
, m_dwTriCnt(0)
, m_pQuadTree(Engine::Get_QuadTree())
, m_pFrustum(Engine::Get_Frustum())
, m_pAlphaTextureCom(NULL)
{

}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Terrain");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	/*m_pTerrainVtxInfo = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];
	m_pBufferCom->Get_VtxInfo(m_pTerrainVtxInfo);

	m_pIndex = new Engine::INDEX32[(VTXCNTX - 1) * (VTXCNTZ - 1) * 2];
	m_pQuadTree->InitQuadTree((Engine::VTXTEX*)m_pTerrainVtxInfo, VTXCNTX, VTXCNTZ);
	m_pQuadTree->InitNeighbor((Engine::VTXTEX*)m_pTerrainVtxInfo, m_pQuadTree, VTXCNTX, VTXCNTZ);*/

	return S_OK;
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Material Component
	D3DMATERIAL9			MtrlInfo;
	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(MtrlInfo);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Material", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	
	pComponent = m_pNormalTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_TerrainNormal");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_NormalTexture", pComponent));

	pComponent = m_pAlphaTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Alpha");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_AlphaTexture", pComponent));

		
	return S_OK;
}

_int CTerrain::Update(const _float& fTimeDelta)
{

	/*m_pFrustum->Transform_Frustum_Local(&m_pTransCom->m_matWorld);

	m_pQuadTree->IsinFrustum((Engine::VTXTEX*)m_pTerrainVtxInfo, m_pFrustum);
	m_pQuadTree->CullingToQuadTree(m_pGraphicDev, (Engine::VTXTEX*)m_pTerrainVtxInfo, m_pFrustum
		, (Engine::INDEX32*)m_pIndex, &(m_dwTriCnt = 0));*/


	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CTerrain::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	//m_pBufferCom->Set_IdxInfo(m_pIndex, &m_dwTriCnt);
	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*		pGameObject = new CTerrain(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CTerrain Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
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
	m_pNormalTextureCom->Set_ConstantTable(m_pEffect, "g_NormalTexture1");	

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_TileTexture1", 1);	
	m_pNormalTextureCom->Set_ConstantTable(m_pEffect, "g_NormalTexture2", 1);	

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_TileTexture2", 2);	
	m_pNormalTextureCom->Set_ConstantTable(m_pEffect, "g_NormalTexture3", 2);	

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_TileTexture3", 3);	
	m_pNormalTextureCom->Set_ConstantTable(m_pEffect, "g_NormalTexture4", 3);	


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

	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_MtrlInfo();
	m_pEffect->SetVector("g_vTerrainDiffuse", &_vec4((_float*)&pMtrlInfo->Diffuse));
	m_pEffect->SetVector("g_vTerrainAmbient", &_vec4((_float*)&pMtrlInfo->Ambient));
	m_pEffect->SetVector("g_vTerrainSpecular", &_vec4((_float*)&pMtrlInfo->Specular));

	m_pEffect->SetVector("g_vWorldCamPos", &vCameraPos);

	m_pAlphaTextureCom->Set_ConstantTable(m_pEffect, "g_ColorHeightTexture");
}


_ulong CTerrain::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		//Engine::Safe_Delete_Array(m_pTerrainVtxInfo);
		//Engine::Safe_Delete_Array(m_pIndex);

		delete this;
	}
	return dwRefCnt;
}
