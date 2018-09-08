#include "stdafx.h"
#include "Bloody.h"
#include "Export_Function.h"

CBloody::CBloody(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pBufferCom(NULL)
, m_pTextureCom(NULL)
, m_pFrame(NULL)
, m_fAccTime(0.f)
, m_bSwitch(false)
, m_fUIDepth(0.f)
, m_fX(0.f), m_fY(0.f)
, m_fSizeX(0.f), m_fSizeY(0.f)

, m_fAlpha(1.f)
{
	D3DXMatrixIdentity(&m_matOrtho);
}

CBloody::~CBloody(void)
{

}

HRESULT CBloody::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = WINSIZEX * 0.5f;
	m_fSizeY = WINSIZEY * 0.5f;	
	m_fX = WINSIZEX * 0.5f;
	m_fY = WINSIZEY * 0.5f;

	//m_iIndex = iIdx + 1;

	m_pFrame->Initialize(1, 1, 0.f);

	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	return S_OK;
}

HRESULT CBloody::Add_Component(void)
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
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Bloody");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	return S_OK;
}

_int CBloody::Update(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] +=  fTimeDelta * 5.f;
	
	//m_fSizeX += m_fAccTime * 20.f;

	if(m_fAccTime > 1.f)
	{
		m_fAlpha -= fTimeDelta * 0.5f;
	}

	Engine::CGameObject::Update(fTimeDelta);

	if(m_fAccTime > 3.f)
	{
		return -1;
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this, 1.4f);

	return 0;
}

void CBloody::Render(void)
{
	if(NULL == m_pEffect)
		return;
	//if(!m_bSwitch) return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CBloody* CBloody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{	
	CBloody*		pGameObject = new CBloody(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CBloody Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CBloody::Set_ContantTable(void)
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

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);

	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);
	m_pEffect->SetVector("g_vColor", &_vec4(1.f, 1.f, 1.f, 1.f));

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

void CBloody::Set_SettingData(_vec3 vPos)
{
	m_pTransCom->m_vPosition = vPos;
	m_fAccTime	 = 0.f;
}

_ulong CBloody::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}

