#include "stdafx.h"
#include "SkillWnd.h"
#include "Export_Function.h"

CSkillWnd::CSkillWnd(LPDIRECT3DDEVICE9 pGraphicDev, _int iIdx)
: Engine::CGameObject(pGraphicDev)
, m_iIdx(iIdx)
{

}

CSkillWnd::~CSkillWnd(void)
{

}

HRESULT CSkillWnd::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 25.f;
	m_fSizeY = 25.f;	
	m_fX = WINSIZEX * 0.5f + (float(m_iIdx) - 1.5f) * 95.f;
	m_fY = WINSIZEY * 0.9f;

	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);


	return S_OK;
}

HRESULT CSkillWnd::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_SkillWnd");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

_int CSkillWnd::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);


	return 0;
}

void CSkillWnd::Render(void)
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

CSkillWnd* CSkillWnd::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIdx)
{
	CSkillWnd*		pGameObject = new CSkillWnd(pGraphicDev, iIdx);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CSkillWnd Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CSkillWnd::Set_ContantTable(void)
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

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", m_iIdx);	
}

_ulong CSkillWnd::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

