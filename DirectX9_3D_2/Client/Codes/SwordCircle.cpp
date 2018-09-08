#include "stdafx.h"
#include "SwordCircle.h"
#include "Export_Function.h"


CSwordCircle::CSwordCircle(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pBufferCom(NULL)
, m_pTextureCom(NULL)
, m_pFrame(NULL)
, m_vViewPos(0.f, 0.f, 0.f)
, m_bSwitch(false)
, m_fAccTime(0.f)
, m_fAlpha(1.f)
{

}

CSwordCircle::~CSwordCircle(void)
{

}

HRESULT CSwordCircle::Initialize(_vec3 vPos)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlackAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pFrame->Initialize(1, 1, 0.f);
	m_pTransCom->m_vPosition = vPos + _vec3(0.f, 0.1f, 0.f);

	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = D3DXToRadian(90.f);
	m_pTransCom->m_vScale = _vec3(2.f, 2.f, 2.f);

	return S_OK;
}

HRESULT CSwordCircle::Add_Component(void)
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
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_SwordCircle");
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

_int CSwordCircle::Update(const _float& fTimeDelta)
{	
	Engine::CGameObject::Update(fTimeDelta);

	m_fAccTime += fTimeDelta;

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//
	_matrix		matBill;
	D3DXMatrixIdentity(&matBill);
	matBill = matView;
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransCom->m_matWorld = matBill * m_pTransCom->m_matWorld;//

	

	if(m_fAccTime < 0.5f)
	{
		m_pTransCom->m_vScale += _vec3(m_fAccTime, m_fAccTime, m_fAccTime) * 0.05f;
	}


	if( m_fAccTime > 5.f )
	{
		m_fAlpha -= fTimeDelta;
	}
	if( m_fAccTime > 6.f )
	{
		return -1;
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);
	return 0;
}

void CSwordCircle::Render(void)
{
	if(NULL == m_pEffect)	return;

	if(!Engine::Get_Frustum()->SphereInFrustum(&m_pTransCom->m_vPosition, 3.f))
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

CSwordCircle* CSwordCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSwordCircle*		pEffect = new CSwordCircle(pGraphicDev);

	if(FAILED(pEffect->Initialize(vPos)))
	{
		MSG_BOX("CSwordCircle Create Failed");
		::Safe_Release(pEffect);
	}
	return pEffect;
}

void CSwordCircle::Set_ContantTable(void)
{	
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);
	

	m_pEffect->SetFloat("g_iAlpha", m_fAlpha);

	m_pEffect->SetVector("g_vColor", &_vec4(1.f,1.f,1.f,1.f));
	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

_ulong CSwordCircle::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}

