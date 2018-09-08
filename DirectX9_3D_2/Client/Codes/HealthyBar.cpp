#include "stdafx.h"
#include "HealthyBar.h"
#include "Export_Function.h"

CHealthyBar::CHealthyBar(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pHpBarBufferCom(NULL)
, m_pTextureCom(NULL)
{

}

CHealthyBar::~CHealthyBar(void)
{

}

HRESULT CHealthyBar::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 120.f;
	m_fSizeY = 7.f;
	m_fX = WINSIZEX * 0.1f;
	m_fY = WINSIZEY * 0.125f;

	// ������������� ������ִ� ����� �Լ���.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	return S_OK;
}

HRESULT CHealthyBar::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	//pComponent = m_pHpBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	//if(NULL == pComponent)
	//	return E_FAIL;
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_HpBuffer", pComponent));

	pComponent = m_pHpBarBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Healthy");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

_int CHealthyBar::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);


	return 0;
}

void CHealthyBar::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	// ������ɷ����������������� ��������ʰ�, ���̴��� �׸��ڽ��ϴ�.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	//m_pHpBufferCom->Render(NULL);
	m_pHpBarBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CHealthyBar* CHealthyBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHealthyBar*		pGameObject = new CHealthyBar(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("pHpBar Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CHealthyBar::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView;
	D3DXMatrixIdentity(&matView);


	matView._11 = m_fSizeX;
	matView._22 = m_fSizeY;
	matView._33 = 1.f;

	matView._41 = m_fX - WINSIZEX * 0.5f;
	matView._42 = -m_fY + WINSIZEY * 0.5f;


	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

_ulong CHealthyBar::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
