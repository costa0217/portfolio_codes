#include "stdafx.h"
#include "Sea.h"
#include "Export_Function.h"

CSea::CSea(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pMtrlCom(NULL)
, m_pBufferCom(NULL)
, m_pTextureCom(NULL)
, m_pEffect(NULL)
, m_fTimeDelta(0.f)
, m_fAccTime(0.f)
{

}

CSea::~CSea(void)
{

}

HRESULT CSea::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Sea");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition	= _vec3(0.f, -1.1f, 0.f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = D3DXToRadian(90.f);
	m_pTransCom->m_vScale		= _vec3(1000.f, 1000.f, 1000.f);



	return S_OK;
}

HRESULT CSea::Add_Component(void)
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
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(MtrlInfo);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Material", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Sea");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

_int CSea::Update(const _float& fTimeDelta)
{

	m_fAccTime += fTimeDelta * 0.5f;
	int iRand = rand()%4;
	//float fRand = iRand * 0.001f;
	m_fTimeDelta += fTimeDelta * (cosf(D3DX_PI * m_fAccTime) * iRand) * 0.005f;

	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CSea::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CSea* CSea::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSea*		pGameObject = new CSea(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CTerrain Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CSea::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	D3DXMATRIX matCamera;
	D3DXMatrixInverse(&matCamera, NULL, &matView);
	_vec4 vCameraPos = _vec4(matCamera._41, matCamera._42, matCamera._43, 1.f);

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Specular));

	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_MtrlInfo();
	m_pEffect->SetVector("g_vTerrainDiffuse", &_vec4((_float*)&pMtrlInfo->Diffuse));
	m_pEffect->SetVector("g_vTerrainAmbient", &_vec4((_float*)&pMtrlInfo->Ambient));
	m_pEffect->SetVector("g_vTerrainSpecular", &_vec4((_float*)&pMtrlInfo->Specular));

	m_pEffect->SetVector("g_vWorldCamPos", &vCameraPos);

	m_pEffect->SetFloat("g_fTime", m_fTimeDelta);
}


_ulong CSea::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}
