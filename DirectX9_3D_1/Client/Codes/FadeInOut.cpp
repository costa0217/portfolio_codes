#include "stdafx.h"
#include "FadeInOut.h"

#include "Export_Function.h"
#include "DynamicCamera.h"
#include "SoundMgr.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CFadeInOut)

CFadeInOut::CFadeInOut(void)
: Engine::CGameObject(Engine::Get_GraphicDev())
, m_bRender(false)
, m_fAlpha(0.f)
, m_bAlphaCheck(false)
, m_fSpeed(0.f)
{
	Initialize();
	ZeroMemory(m_matOrtho, sizeof(_matrix));
}

CFadeInOut::~CFadeInOut(void)
{

}

HRESULT CFadeInOut::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest3)");

	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX	= 1600.f;
	m_fSizeY	= 900.f;

	m_fX	= 800.f;
	m_fY	= 450.f;

	return S_OK;
}

HRESULT CFadeInOut::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_FadeInOut");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_FadeInOut", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_FadeInOut");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_FadeInOut", pComponent));

	return S_OK;
}

_int CFadeInOut::Update(const _float& fTimeDelta)
{
	//그리기를 시작하면 시작
	if(m_bRender)
	{
		if(m_bAlphaCheck)
		{
			//밝아지는 부분
			if(m_fAlpha < 0.5f)
				m_fAlpha += fTimeDelta * m_fSpeed / 5.f;
			else
				m_fAlpha += fTimeDelta * m_fSpeed;

			//printf("밝아진다 밝아진다\n");
			if(m_fAlpha > 1.0f)
			{
				//printf("다 밝아짐\n");
				m_fAlpha = 1.0f;
				m_bAlphaCheck = false;
				//여기가 전부 밝아졌을때
				m_bRender = false;
			}
		}
		else
		{
			//검해지는 부분
			if(m_fAlpha > 0.7f)
				m_fAlpha -= fTimeDelta * m_fSpeed;
			else if(m_fAlpha > 0.35f)
				m_fAlpha -= fTimeDelta * m_fSpeed / 5.f;
			else
				m_fAlpha -= fTimeDelta * m_fSpeed / 13.f;

			//printf("어두어진다.\n");			
			if(m_fAlpha < 0.0f)
			{
				//printf("다 어두어짐\n");
				m_fAlpha = 0.0f;
				m_bAlphaCheck = true;
				//전부 검해지면?
				m_bRender = false;
			}
		}

		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);
	}

	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CFadeInOut::Render(void)
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

	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);

}

_ulong CFadeInOut::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}