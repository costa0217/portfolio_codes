#include "stdafx.h"
#include "BossFace.h"
#include "Export_Function.h"
#include "Boss.h"

CBossFace::CBossFace(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pMtrlCom(NULL)
, m_pBufferCom(NULL)
, m_pTextureCom(NULL)
, m_pFrame(NULL)
, m_pBoss(NULL)
, m_bRender(false)
{

}

CBossFace::~CBossFace(void)
{

}

HRESULT CBossFace::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 55.f;
	m_fSizeY = 45.f;
	m_fX = WINSIZEX * 0.4f;
	m_fY = WINSIZEY * 0.1f;

	m_pFrame->Initialize(1, 1, 0.f);

	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	return S_OK;
}

HRESULT CBossFace::Add_Component(void)
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
	MtrlInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
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
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_BossFace");
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

_int CBossFace::Update(const _float& fTimeDelta)
{
	if(((CBoss*)m_pBoss)->Get_Start())
	{
		m_bRender = true;
	}

	Engine::CGameObject::Update(fTimeDelta);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this, 1.3f);

	return 0;
}

void CBossFace::Render(void)
{
	if(NULL == m_pEffect)	return;
	if(!m_bRender )		return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);



	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CBossFace* CBossFace::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossFace*		pLogoBack = new CBossFace(pGraphicDev);

	if(FAILED(pLogoBack->Initialize()))
	{
		MSG_BOX("pLogoBack Create Faild");
		::Safe_Release(pLogoBack);
	}
	return pLogoBack;
}

void CBossFace::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView;
	D3DXMatrixIdentity(&matView);


	matView._11 = m_fSizeX;
	matView._22 = m_fSizeY;
	matView._33 = 1.f;

	matView._41 = m_fX - WINSIZEX * 0.5f;
	matView._42 = -m_fY + WINSIZEY * 0.5f;

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);

	m_pEffect->SetFloat("g_fAlpha", 1.f);
	m_pEffect->SetVector("g_vColor", &_vec4(1.f, 1.f, 1.f, 1.f));


	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

_ulong CBossFace::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
