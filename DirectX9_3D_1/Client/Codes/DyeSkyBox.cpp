#include "stdafx.h"
#include "DyeSkyBox.h"
#include "Export_Function.h"

#include "Player.h"

CDyeSkyBox::CDyeSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pPlayerTransCom(NULL)
{

}

CDyeSkyBox::~CDyeSkyBox(void)
{

}

HRESULT CDyeSkyBox::Initialize(_int iIndex)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SkyBox");
	if(NULL == m_pEffect)		return E_FAIL;

	if(FAILED(Add_Component()))		return E_FAIL;

	m_pTransCom->m_vScale = _vec3(250.f, 250.f, 250.f);
	m_pTransCom->m_vPosition = _vec3(0.f, 0.f, 0.f);
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(90.f);

	m_iTexIndex = iIndex;

	return S_OK;

}

HRESULT CDyeSkyBox::Add_Component()
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
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_DYE, L"Buffer_Cube");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_DYE, L"Texture_Sky");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

_int CDyeSkyBox::Update(const _float& fTimeDelta)
{
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] -= D3DXToRadian(1.f*fTimeDelta);
	if(NULL == m_pPlayerTransCom)
	{
		m_pPlayerTransCom =  CPlayer::GetInstance()->Get_TransCom();
	}
	m_pTransCom->m_vPosition = m_pPlayerTransCom->m_vPosition;

	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_PRIORITY, this);

	return 0;
}

void CDyeSkyBox::Render()
{
	if(NULL == m_pEffect)		return;

	Set_ContantTable();

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CDyeSkyBox* CDyeSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
{
	CDyeSkyBox*		pSkyBox = new CDyeSkyBox(pGraphicDev);

	if(FAILED(pSkyBox->Initialize(iIndex)))
	{
		MSG_BOX("CSkyBox Create Faild");
		::Safe_Release(pSkyBox);
	}
	return pSkyBox;
}

void CDyeSkyBox::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)	return;
	_vec3	vLightDir =  _vec3(pLightInfo->Direction);
	D3DXVec3Normalize(&vLightDir,  &vLightDir);

	m_pEffect->SetVector("g_LightDir", &_vec4(vLightDir, 0.f));

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 0);
}
void CDyeSkyBox::Set_TexIndex(_int iIndex)
{
	m_iTexIndex = iIndex;
}
_ulong CDyeSkyBox::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}



