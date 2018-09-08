#include "stdafx.h"
#include "Shock.h"
#include "Export_Function.h"


CShock::CShock(LPDIRECT3DDEVICE9 pGraphicDev)
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

CShock::~CShock(void)
{

}

HRESULT CShock::Initialize(_vec3 vPos)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlack");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pFrame->Initialize(1, 1, 0.f);
	m_pTransCom->m_vPosition = vPos + _vec3(0.f, 2.f, 0.f);

	//int iRand = rand()%10;
	m_pTransCom->m_vScale = _vec3(1.f, 1.f, 1.f);

	m_iIndex = rand()%4;

	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = D3DXToRadian(float(rand()%180));

	return S_OK;
}

HRESULT CShock::Add_Component(void)
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
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Shock");
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

_int CShock::Update(const _float& fTimeDelta)
{

	m_fAccTime += fTimeDelta;
	if(m_fAccTime > 0.1f)
	{
		m_bSwitch = true;
	}

	

	Engine::CGameObject::Update(fTimeDelta);

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pTransCom->m_vScale += _vec3(2.f, 2.f, 2.f) * m_fAccTime;

	//
	_matrix		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = matView;

	//matBill = *m_pCameraObserver->GetView();

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransCom->m_matWorld = matBill * m_pTransCom->m_matWorld;//

	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 


	//if(m_fAccTime > 0.2f)
	//{
	//	m_fAlpha -= m_fAccTime * 2.f;
	//}
	//
	if(m_fAccTime > 0.2f)
	{
		return -1;
	}
	/*if(m_pFrame->m_bEnd)
	{
		return -1;
	}*/

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);



	return 0;
}

void CShock::Render(void)
{
	if(NULL == m_pEffect)	return;
	if(!m_bSwitch)	return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CShock* CShock::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CShock*		pEffect = new CShock(pGraphicDev);

	if(FAILED(pEffect->Initialize(vPos)))
	{
		MSG_BOX("CBlood Create Failed");
		::Safe_Release(pEffect);
	}
	return pEffect;
}

void CShock::Set_ContantTable(void)
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


	m_pEffect->SetVector("g_vColor", &_vec4(1.8f, 1.8f, 1.4f, 1.f));
	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", m_iIndex);	
}

_ulong CShock::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

void CShock::Set_SettingData(_vec3 vPos)
{
	m_pTransCom->m_vPosition = vPos;
}



