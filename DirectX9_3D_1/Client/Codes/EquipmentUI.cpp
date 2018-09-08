#include "stdafx.h"
#include "EquipmentUI.h"

#include "Export_Function.h"
#include "DynamicCamera.h"
#include "SoundMgr.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CEquipmentUI)

CEquipmentUI::CEquipmentUI(void)
: Engine::CGameObject(Engine::Get_GraphicDev())
, m_bRender(false)
, m_fAlpha(0.f)
, m_bAlphaCheck(false)
, m_iTextureNumber(0)
{
	Initialize();
	ZeroMemory(m_matOrtho, sizeof(_matrix));
}

CEquipmentUI::~CEquipmentUI(void)
{

}

HRESULT CEquipmentUI::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest3)");

	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX	= 1200.f * 0.7f;
	m_fSizeY	= 100.f * 0.7f;

	m_fX	= 800.f;
	m_fY	= 215.f;

	return S_OK;
}

HRESULT CEquipmentUI::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_EquipmentUI");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_HeartBeatUI", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_EquipmentUI");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_EquipmentUI", pComponent));

	return S_OK;
}

_int CEquipmentUI::Update(const _float& fTimeDelta)
{
	if(m_bRender)
	{
		if(m_bAlphaCheck)
		{
			//1 ~ 2.5 까지
			m_fAlpha += fTimeDelta * 0.75f;

			if(m_fAlpha > 1.0f)
			{
				m_fAlpha = 1.0f;	//완전 투명
				m_bAlphaCheck = false;
				//제일 처음에 1.f로하고 여기 들어왔을때 m_bRender를 false로 하면될거같음
				m_bRender = false;
			}
		}
		else
		{
			m_fAlpha -= fTimeDelta * 0.75f;

			if(m_fAlpha < 0.0f)
			{
				m_fAlpha = 0.0f;
				m_bAlphaCheck = true;
			}
		}

		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);
	}
	else
	{
		m_bAlphaCheck	= false;
		m_fAlpha		= 1.f;
	}

	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CEquipmentUI::Render(void)
{
	if(NULL == m_pEffect)
		return;

	//그리는 로직 관련
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
	_matrix matView;
	D3DXMatrixIdentity(&matView);

	//사이즈
	matView._11 = m_fSizeX;
	matView._22 = m_fSizeY;
	matView._33 = 1.f;

	//위치
	matView._41 = m_fX - WINSIZEX * 0.5f;
	matView._42 = -m_fY + WINSIZEY * 0.5f;

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", m_iTextureNumber);

	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(1);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);

}

_ulong CEquipmentUI::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}