#include "stdafx.h"
#include "BossFaceUI.h"
#include "Export_Function.h"
#include "Monster.h"
#include "DynamicCamera.h"

CBossFaceUI::CBossFaceUI(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_iFaceNumber(0)
, m_bRender(false)
{
	ZeroMemory(m_matOrtho, sizeof(_matrix));

}

CBossFaceUI::~CBossFaceUI(void)
{

}

HRESULT CBossFaceUI::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest3)");

	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX	= 200.f;
	m_fSizeY	= 94.f;

	m_fX	= 425.f;
	m_fY	= 84.f;

	return S_OK;
}

HRESULT CBossFaceUI::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_BossFace");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_BossFace", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_BossFace");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_BossFace", pComponent));

	return S_OK;
}

_int CBossFaceUI::Update(const _float& fTimeDelta)
{
	if(!CDynamicCamera::GetInstance()->GetModeCheck() && m_bRender)
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);
	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CBossFaceUI::Render(void)
{
	if(NULL == m_pEffect)
		return;

	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
	_matrix matView;
	D3DXMatrixIdentity(&matView);

	//사이즈
	matView._11 = m_fSizeX;
	matView._22 = m_fSizeY;
	matView._33 = 1.f;

	m_fX	= 425.f;
	m_fY	= 84.f;

	//위치
	matView._41 = m_fX - WINSIZEX * 0.5f;
	matView._42 = -m_fY + WINSIZEY * 0.5f;

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pEffect->SetFloat("g_fAlpha", 0.f);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", m_iFaceNumber);

	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CBossFaceUI* CBossFaceUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossFaceUI*		pGameObject = new CBossFaceUI(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("pGameObject Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

_ulong CBossFaceUI::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}