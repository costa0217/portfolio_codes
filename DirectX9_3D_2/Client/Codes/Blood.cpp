#include "stdafx.h"
#include "Blood.h"
#include "Export_Function.h"


CBlood::CBlood(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_pBufferCom(NULL)
, m_pTextureCom(NULL)
, m_pFrame(NULL)
, m_vViewPos(0.f, 0.f, 0.f)
, m_bSwitch(false)
{

}

CBlood::~CBlood(void)
{

}

HRESULT CBlood::Initialize(_vec3 vPos)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Sprite");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pFrame->Initialize(4, 4, 60.f);

	int iRand = rand()%21;
	float fRand = ((10 - iRand) * 0.1f) + 3.f;
	m_pTransCom->m_vPosition = vPos + _vec3(0.f, fRand, 0.f);
	m_pTransCom->m_vScale = _vec3(0.5f, 0.5f, 0.5f);

	return S_OK;
}

HRESULT CBlood::Add_Component(void)
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
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Blood_Single");
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

_int CBlood::Update(const _float& fTimeDelta)
{
	if(m_pFrame->m_bEnd)
	{
		return -1;
	}
	
	Engine::CGameObject::Update(fTimeDelta);
	
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//
	_matrix		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = matView;

	//matBill = *m_pCameraObserver->GetView();

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransCom->m_matWorld = matBill * m_pTransCom->m_matWorld;//

	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);

	

	return 0;
}

void CBlood::Render(void)
{
	if(NULL == m_pEffect)	return;
	//if(!m_bSwitch)	return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CBlood* CBlood::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBlood*		pEffect = new CBlood(pGraphicDev);

	if(FAILED(pEffect->Initialize(vPos)))
	{
		MSG_BOX("CBlood Create Failed");
		::Safe_Release(pEffect);
	}
	return pEffect;
}
CBlood* CBlood::CreateScale(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float fScale)
{
	CBlood*		pEffect = new CBlood(pGraphicDev);

	if(FAILED(pEffect->Initialize(vPos)))
	{
		MSG_BOX("CBlood Create Failed");
		::Safe_Release(pEffect);
	}
	pEffect->Set_Scale(fScale);
	pEffect->Set_SettingData(vPos);

	return pEffect;
}



void CBlood::Set_ContantTable(void)
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

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

_ulong CBlood::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

void CBlood::Set_SettingData(_vec3 vPos)
{
	m_pTransCom->m_vPosition = vPos;
}

void CBlood::Set_Scale(_float fScale)
{
	m_pTransCom->m_vScale = _vec3(fScale, fScale, fScale);
}

