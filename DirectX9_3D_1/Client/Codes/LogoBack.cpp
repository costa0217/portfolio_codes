#include "stdafx.h"
#include "LogoBack.h"

#include "Export_Function.h"
#include "Loading.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fLoading(0.f)
{
	
}

CLogoBack::~CLogoBack(void)
{

}

HRESULT CLogoBack::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Logo");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vScale = _vec3(1.f, 1.f, 1.f);

	m_pVTXInfo = new Engine::VTXTEX[4];
	m_pBufferComFront->Get_VtxInfo(m_pVTXInfo);

	return S_OK;
}

HRESULT CLogoBack::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferComFront = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_LOGO, L"Buffer_RcTexFront");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufferFront", pComponent));

	pComponent = m_pBufferComBack = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_LOGO, L"Buffer_RcTexBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufferBack", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGO, L"Texture_Logo");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

_int CLogoBack::Update(const _float& fTimeDelta)
{
	m_fLoading += 100.f * fTimeDelta;

	if(m_fLoading >= CLoading::m_iLoadingNumber * 100.f)
	{
		m_fLoading = CLoading::m_iLoadingNumber * 100.f;
	}
	else if(m_fLoading <= CLoading::m_iLoadingNumber * 100.f)
	{
		m_fLoading += 1000.f * fTimeDelta;
	}

	m_pVTXInfo[1].vPos.x = -0.5f + m_fLoading / 1600.f;
	m_pVTXInfo[2].vPos.x = -0.5f + m_fLoading / 1600.f;

	m_pVTXInfo[1].vTexUV.x = m_fLoading / 1600.f;
	m_pVTXInfo[2].vTexUV.x = m_fLoading / 1600.f;
	m_pBufferComFront->Set_VtxInfo(m_pVTXInfo);

	Engine::CGameObject::Update(fTimeDelta);
	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CLogoBack::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	{
		m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", (_ulong)0);

		// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);

		m_pBufferComBack->Render(NULL);

		m_pEffect->EndPass();
		m_pEffect->End();	

		m_pEffect->SetTexture("g_BaseTexture", NULL);
	}

	{
		m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", (_ulong)1);

		// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);

		m_pBufferComFront->Render(NULL);

		m_pEffect->EndPass();
		m_pEffect->End();	

		m_pEffect->SetTexture("g_BaseTexture", NULL);
	}
}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoBack*		pLogoBack = new CLogoBack(pGraphicDev);

	if(FAILED(pLogoBack->Initialize()))
	{
		MSG_BOX("pLogoBack Create Faild");
		::Safe_Release(pLogoBack);
	}
	return pLogoBack;
}

void CLogoBack::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView;
	D3DXMatrixIdentity(&matView);

	matView._11 = 1600.f;
	matView._22 = 900.f;
	matView._33 = 0.5f;

	matView._41 = 0.f;
	matView._42 = 0.f;


	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);
}

_ulong CLogoBack::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		::Safe_Delete_Array(m_pVTXInfo);
		delete this;
	}

	return dwRefCnt;
}

