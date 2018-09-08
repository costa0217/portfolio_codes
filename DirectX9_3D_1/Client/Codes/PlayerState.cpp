#include "stdafx.h"
#include "PlayerState.h"
#include "Export_Function.h"
#include "Player.h"
#include "DynamicCamera.h"

#include "Player.h"

CPlayerState::CPlayerState(LPDIRECT3DDEVICE9 pGraphicDev) 
: Engine::CGameObject(pGraphicDev)
{
	
	ZeroMemory(m_matOrtho, sizeof(_matrix));
}

CPlayerState::~CPlayerState(void)
{
	
}

HRESULT CPlayerState::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest)");

	if(NULL == m_pEffect)
		return E_FAIL;
	
	if(FAILED(Add_Component()))
		return E_FAIL;

	//위치 조절 지역 변수
	_float fHPX = 800.f;
	_float fHpY = 25.f;

	_float fStaminamX = 800.f;
	_float fStaminamY = 50.f;

	_float fSPX = 800.f;
	_float fSPY = 70.f;

	//사이즈 조절용
	_float	fGapX = 1.f;
	_float	fGapY = 2.f;

	//모든 위치
	_float fAllSizeX = 0.7f;


	//HP
	m_fSizeX[HP_Front]	= (610.f - fGapX) * fAllSizeX;
	m_fSizeY[HP_Front]	= (16.f - fGapY);
	m_fX[HP_Front]		= fHPX;
	m_fY[HP_Front]		= fHpY - 3.0f;

	m_fSizeX[HP_Back]	= (610.f - fGapX) * fAllSizeX;
	m_fSizeY[HP_Back]	= (16.f - fGapY);
	m_fX[HP_Back]		= fHPX;
	m_fY[HP_Back]		= fHpY - 3.0f;

	m_fSizeX[HP_Bar]	= 620.f * fAllSizeX;
	m_fSizeY[HP_Bar]	= 32.f;
	m_fX[HP_Bar]		= fHPX;
	m_fY[HP_Bar]		= fHpY;

	m_fSizeX[HP_Bar2]	= 620.f * fAllSizeX;
	m_fSizeY[HP_Bar2]	= 32.f;
	m_fX[HP_Bar2]		= fHPX;
	m_fY[HP_Bar2]		= fHpY;

	//SP
	m_fSizeX[SP_Front]	= ((230.f - fGapX) * 1.5f) * fAllSizeX;
	m_fSizeY[SP_Front]	= 16.f - fGapY;
	m_fX[SP_Front]		= fSPX;
	m_fY[SP_Front]		= fSPY - 5.f;

	m_fSizeX[SP_Back]	= ((230.f - fGapX) * 1.5f) * fAllSizeX;
	m_fSizeY[SP_Back]	= (16.f - fGapY);
	m_fX[SP_Back]		= fSPX;
	m_fY[SP_Back]		= fSPY - 5.f;

	m_fSizeX[SP_Bar]	= 240.f * 1.5f * fAllSizeX;
	m_fSizeY[SP_Bar]	= 32.f;
	m_fX[SP_Bar]		= fSPX - 1.5f;
	m_fY[SP_Bar]		= fSPY - 3.f;

	m_fSizeX[SP_Bar2]	= 240.f * 1.5f * fAllSizeX;
	m_fSizeY[SP_Bar2]	= 32.f;
	m_fX[SP_Bar2]		= fSPX;
	m_fY[SP_Bar2]		= fSPY;

	//Staminam
	m_fSizeX[Stamina_Front]	= (610.f - fGapX) * fAllSizeX;
	m_fSizeY[Stamina_Front]	= (16.f - fGapY);
	m_fX[Stamina_Front]		= fStaminamX;
	m_fY[Stamina_Front]		= fStaminamY - 3.0f;

	m_fSizeX[Stamina_Back]	= (610.f - fGapX) * fAllSizeX;
	m_fSizeY[Stamina_Back]	= (16.f - fGapY);
	m_fX[Stamina_Back]		= fStaminamX;
	m_fY[Stamina_Back]		= fStaminamY - 3.0f;

	m_fSizeX[Stamina_Bar]	= 620.f * fAllSizeX;
	m_fSizeY[Stamina_Bar]	= 32.f;
	m_fX[Stamina_Bar]		= fStaminamX;
	m_fY[Stamina_Bar]		= fStaminamY;

	m_fSizeX[Stamina_Bar2]	= 620.f * fAllSizeX;
	m_fSizeY[Stamina_Bar2]	= 32.f;
	m_fX[Stamina_Bar2]		= fStaminamX;
	m_fY[Stamina_Bar2]		= fStaminamY;

	for(int i = 0; i < PSUIEND; ++i)
	{
		m_pVTXInfo[i] = new Engine::VTXTEX[4];
		m_pBufferCom[i]->Get_VtxInfo(m_pVTXInfo[i]);
	}

	m_pPlayer = CPlayer::GetInstance();

	m_fHPFront			= m_pPlayer->GetPlayerHP();
	m_fHPBack			= m_pPlayer->GetPlayerHP();
	m_fSPFront			= m_pPlayer->GetPlayerSP();
	m_fSPBack			= m_pPlayer->GetPlayerSP();
	m_fStaminaFront		= m_pPlayer->GetPlayerStamina();
	m_fStaminaBack		= m_pPlayer->GetPlayerStamina();

	return S_OK;
}

HRESULT CPlayerState::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	//HP
	pComponent = m_pBufferCom[HP_Front] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerHPFront");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerHPFront", pComponent));

	pComponent = m_pBufferCom[HP_Back] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerHPBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerHPBack", pComponent));

	pComponent = m_pBufferCom[HP_Bar] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerState");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerState0", pComponent));

	pComponent = m_pBufferCom[HP_Bar2] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerState");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerState1", pComponent));

	//SP
	pComponent = m_pBufferCom[SP_Front] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerSPFront");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerSPFront", pComponent));

	pComponent = m_pBufferCom[SP_Back] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerSPBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerSPBack", pComponent));

	pComponent = m_pBufferCom[SP_Bar] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerState");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerState2", pComponent));

	pComponent = m_pBufferCom[SP_Bar2] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerState");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerState3", pComponent));

	//Stamina
	pComponent = m_pBufferCom[Stamina_Front] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerStaminaFront");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerStaminaFront", pComponent));

	pComponent = m_pBufferCom[Stamina_Back] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerStaminaBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerStaminaBack", pComponent));

	pComponent = m_pBufferCom[Stamina_Bar] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerState");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerState4", pComponent));

	pComponent = m_pBufferCom[Stamina_Bar2] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerState");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_PlayerState5", pComponent));

	// For.Texture Component
	//HP
	pComponent = m_pTextureCom[HP_Front] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerHP");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerHPF", pComponent));

	pComponent = m_pTextureCom[HP_Back] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerHP");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerHPB", pComponent));

	pComponent = m_pTextureCom[HP_Bar] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerHPBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerHPBack", pComponent));

	pComponent = m_pTextureCom[HP_Bar2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerHPBack2");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerHPBack2", pComponent));

	//SP
	pComponent = m_pTextureCom[SP_Front] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerSP");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerSPF", pComponent));

	pComponent = m_pTextureCom[SP_Back] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerSP");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerSPB", pComponent));

	pComponent = m_pTextureCom[SP_Bar] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerSPBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerSPBack", pComponent));

	pComponent = m_pTextureCom[SP_Bar2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerSPBack2");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerSPBack2", pComponent));

	//Stamina
	pComponent = m_pTextureCom[Stamina_Front] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerStamina");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerStaminaF", pComponent));

	pComponent = m_pTextureCom[Stamina_Back] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerStamina");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerStaminaB", pComponent));

	pComponent = m_pTextureCom[Stamina_Bar] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerStaminaBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerStaminaBack", pComponent));

	pComponent = m_pTextureCom[Stamina_Bar2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerStaminaBack2");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_PlayerStaminaBack2", pComponent));

	return S_OK;
}

_int CPlayerState::Update(const _float& fTimeDelta)
{
	//PlayerState UI의 정보를 업데이한다.
	_float	fPlayerHp		= m_pPlayer->GetPlayerHP();
	_float	fPlayerSP		= m_pPlayer->GetPlayerSP();
	_float	fPlayerStamina	= m_pPlayer->GetPlayerStamina();

	//HP
	if(m_fHPFront != fPlayerHp)
	{
		m_fHPFront += (fPlayerHp - m_fHPFront) * fTimeDelta * 30;

		if(abs(m_fHPFront - fPlayerHp) < 1.f)
			m_fHPFront = fPlayerHp;
	}

	if(m_fHPBack != fPlayerHp)
	{
		m_fHPBack += (fPlayerHp - m_fHPBack) * fTimeDelta * 5;

		if(abs(m_fHPBack - fPlayerHp) < 1.f)
			m_fHPBack = fPlayerHp;
	}

	//SP
	if(m_fSPFront != fPlayerSP)
	{
		m_fSPFront += (fPlayerSP - m_fSPFront) * fTimeDelta * 30;

		if(abs(m_fSPFront - fPlayerSP) < 1.f)
			m_fSPFront = fPlayerSP;
	}

	if(m_fSPBack != fPlayerSP)
	{
		m_fSPBack += (fPlayerSP - m_fSPBack) * fTimeDelta * 5;

		if(abs(m_fSPBack - fPlayerSP) < 1.f)
			m_fSPBack = fPlayerSP;
	}

	//Stamina
	if(m_fStaminaFront != fPlayerStamina)
	{
		m_fStaminaFront += (fPlayerStamina - m_fStaminaFront) * fTimeDelta * 30;

		if(abs(m_fStaminaFront - fPlayerStamina) < 1.f)
			m_fStaminaFront = fPlayerStamina;
	}

	if(m_fStaminaBack != fPlayerStamina)
	{
		m_fStaminaBack += (fPlayerStamina - m_fStaminaBack) * fTimeDelta * 5;

		if(abs(m_fStaminaBack - fPlayerStamina) < 1.f)
			m_fStaminaBack = fPlayerStamina;
	}

	//업데이트 된 정보를 바탕으로 버텍스의 정보 값을 바꾼다.
	for(int i = 0; i < PSUIEND; ++i)
	{
		if(i == HP_Front)
		{
			m_pVTXInfo[i][2].vTexUV.y = 0.5f;
			m_pVTXInfo[i][3].vTexUV.y = 0.5f;

			m_pVTXInfo[i][1].vPos.x = -0.5f + (m_fHPFront / m_pPlayer->GetPlayerMaxHP());
			m_pVTXInfo[i][2].vPos.x = -0.5f + (m_fHPFront / m_pPlayer->GetPlayerMaxHP());

			m_pVTXInfo[i][1].vTexUV.x = (m_fHPFront / m_pPlayer->GetPlayerMaxHP());
			m_pVTXInfo[i][2].vTexUV.x = (m_fHPFront / m_pPlayer->GetPlayerMaxHP());
		}
		else if(i == HP_Back)
		{
			m_pVTXInfo[i][0].vTexUV.y = 0.5f;
			m_pVTXInfo[i][1].vTexUV.y = 0.5f;

			m_pVTXInfo[i][1].vPos.x = -0.5f + (m_fHPBack / m_pPlayer->GetPlayerMaxHP());
			m_pVTXInfo[i][2].vPos.x = -0.5f + (m_fHPBack / m_pPlayer->GetPlayerMaxHP());

			m_pVTXInfo[i][1].vTexUV.x = (m_fHPBack / m_pPlayer->GetPlayerMaxHP());
			m_pVTXInfo[i][2].vTexUV.x = (m_fHPBack / m_pPlayer->GetPlayerMaxHP());
		}
		else if(i == SP_Front)
		{
			m_pVTXInfo[i][2].vTexUV.y = 0.5f;
			m_pVTXInfo[i][3].vTexUV.y = 0.5f;

			m_pVTXInfo[i][1].vPos.x = -0.5f + (m_fSPFront / m_pPlayer->GetPlayerMaxSP());
			m_pVTXInfo[i][2].vPos.x = -0.5f + (m_fSPFront / m_pPlayer->GetPlayerMaxSP());

			m_pVTXInfo[i][1].vTexUV.x = (m_fSPFront / m_pPlayer->GetPlayerMaxSP());
			m_pVTXInfo[i][2].vTexUV.x = (m_fSPFront / m_pPlayer->GetPlayerMaxSP());
		}
		else if(i == SP_Back)
		{
			m_pVTXInfo[i][0].vTexUV.y = 0.5f;
			m_pVTXInfo[i][1].vTexUV.y = 0.5f;

			m_pVTXInfo[i][1].vPos.x = -0.5f + (m_fSPBack / m_pPlayer->GetPlayerMaxSP());
			m_pVTXInfo[i][2].vPos.x = -0.5f + (m_fSPBack / m_pPlayer->GetPlayerMaxSP());

			m_pVTXInfo[i][1].vTexUV.x = (m_fSPBack / m_pPlayer->GetPlayerMaxSP());
			m_pVTXInfo[i][2].vTexUV.x = (m_fSPBack / m_pPlayer->GetPlayerMaxSP());
		}
		else if(i == Stamina_Front)
		{
			m_pVTXInfo[i][2].vTexUV.y = 0.5f;
			m_pVTXInfo[i][3].vTexUV.y = 0.5f;

			m_pVTXInfo[i][1].vPos.x = -0.5f + (m_fStaminaFront / m_pPlayer->GetPlayerMaxStamina());
			m_pVTXInfo[i][2].vPos.x = -0.5f + (m_fStaminaFront / m_pPlayer->GetPlayerMaxStamina());

			m_pVTXInfo[i][1].vTexUV.x = (m_fStaminaFront / m_pPlayer->GetPlayerMaxStamina());
			m_pVTXInfo[i][2].vTexUV.x = (m_fStaminaFront / m_pPlayer->GetPlayerMaxStamina());
		}
		else if(i == Stamina_Back)
		{
			m_pVTXInfo[i][0].vTexUV.y = 0.5f;
			m_pVTXInfo[i][1].vTexUV.y = 0.5f;

			m_pVTXInfo[i][1].vPos.x = -0.5f + (m_fStaminaBack / m_pPlayer->GetPlayerMaxStamina());
			m_pVTXInfo[i][2].vPos.x = -0.5f + (m_fStaminaBack / m_pPlayer->GetPlayerMaxStamina());

			m_pVTXInfo[i][1].vTexUV.x = (m_fStaminaBack / m_pPlayer->GetPlayerMaxStamina());
			m_pVTXInfo[i][2].vTexUV.x = (m_fStaminaBack / m_pPlayer->GetPlayerMaxStamina());
		}
		else
		{
			
		}

		m_pBufferCom[i]->Set_VtxInfo(m_pVTXInfo[i]);
	}

	if(!CDynamicCamera::GetInstance()->GetModeCheck())
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);
	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CPlayerState::Render(void)
{
	if(NULL == m_pEffect)
		return;

	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	for(int i = 0; i < PSUIEND; ++i)
	{
		_matrix matView;
		D3DXMatrixIdentity(&matView);

		//사이즈
		matView._11 = m_fSizeX[i];
		matView._22 = m_fSizeY[i];
		matView._33 = 1.f;

		//위치
		matView._41 = m_fX[i] - WINSIZEX * 0.5f;
		matView._42 = -m_fY[i] + WINSIZEY * 0.5f;

		m_pEffect->SetMatrix("g_matView", &matView);
		m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

		m_pTextureCom[i]->Set_ConstantTable(m_pEffect, "g_BaseTexture");

		m_pEffect->Begin(NULL, 0);

		m_pEffect->BeginPass(0);

		m_pBufferCom[i]->Render(NULL);

		m_pEffect->EndPass();

		m_pEffect->End();	

		m_pEffect->SetTexture("g_BaseTexture", NULL);
	}
		
	return;
}

CPlayerState* CPlayerState::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerState*	pGameObject = new CPlayerState(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("PlayerStateUI Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

_ulong CPlayerState::Release(void)
{
	_ulong	dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		for(int i = 0; i < PSUIEND; ++i)
		{
			::Safe_Delete_Array(m_pVTXInfo[i]);
		}
		delete this;
	}

	return dwRefCnt;
}