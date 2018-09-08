#include "stdafx.h"
#include "PlayerHeartBeatUI.h"

#include "Export_Function.h"
#include "DynamicCamera.h"
#include "SoundMgr.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CPlayerHeartBeatUI)

CPlayerHeartBeatUI::CPlayerHeartBeatUI(void)
: Engine::CGameObject(Engine::Get_GraphicDev())
, m_bRender(false)
, m_fAlpha(0.f)
, m_fPlayerMaxHp(0.f)
, m_fPlayerHp(0.f)
, m_bAlphaCheck(false)
{
	Initialize();
	ZeroMemory(m_matOrtho, sizeof(_matrix));
}

CPlayerHeartBeatUI::~CPlayerHeartBeatUI(void)
{

}

HRESULT CPlayerHeartBeatUI::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest3)");

	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX	= 1700.f;
	m_fSizeY	= 900.f;

	m_fX	= 800.f;
	m_fY	= 450.f;

	m_pPlayer		= CPlayer::GetInstance();
	m_fPlayerMaxHp	= m_pPlayer->GetPlayerMaxHP();

	return S_OK;
}

HRESULT CPlayerHeartBeatUI::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_HeartBeatUI");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_HeartBeatUI", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerHeartBeatUI");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerHeartBeatUI", pComponent));

	return S_OK;
}

_int CPlayerHeartBeatUI::Update(const _float& fTimeDelta)
{
	m_fPlayerHp = m_pPlayer->GetPlayerHP();

	if(!CDynamicCamera::GetInstance()->GetModeCheck() && m_bRender)
	{
		if(m_bAlphaCheck)
		{
			//1 ~ 2.5 까지
			m_fAlpha += fTimeDelta * (1 + (1.0f - (m_fPlayerHp / m_fPlayerMaxHp) * 2.5f));

			if(m_fAlpha > 1.0f)
			{
				m_fAlpha = 1.0f;
				m_bAlphaCheck = false;
				CSoundMgr::GetInstance()->PlayEffect(SOUND::heart_beat);
			}
		}
		else
		{
			m_fAlpha -= fTimeDelta * (1 + (1.0f - (m_fPlayerHp / m_fPlayerMaxHp) * 2.5f));

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

void CPlayerHeartBeatUI::Render(void)
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

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");

	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(1);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	
}

_ulong CPlayerHeartBeatUI::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}