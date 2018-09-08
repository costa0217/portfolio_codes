#include "stdafx.h"
#include "DyeIcon.h"

#include "Export_Function.h"
#include "SelectDye.h"

TCHAR*	CDyeIcon::m_pTexName = NULL;
int		CDyeIcon::m_iPosX = 0;
int		CDyeIcon::m_iPosY = 0;

CDyeIcon::CDyeIcon(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_iSelect(0)
{

}

CDyeIcon::~CDyeIcon(void)
{

}

HRESULT CDyeIcon::Initialize(TCHAR* pTexName, int iPosX, int iPosY, int iSizeX, int iSizeY)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI(Alpha)");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component(pTexName, iPosX, iPosY, iSizeX, iSizeY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDyeIcon::Add_Component(TCHAR* pTexName, int iPosX, int iPosY, int iSizeX, int iSizeY)
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
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_DYE, L"Buffer_DyeIcon");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	TCHAR TexName[128];
	lstrcpy( TexName, pTexName );
	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_DYE, TexName/*L"Texture_DyeIcon"*/);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	m_iX = iPosX;
	m_iY = iPosY;

	m_iSizeX = iSizeX;
	m_iSizeY = iSizeY;

	return S_OK;
}

_int CDyeIcon::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	return 0;
}

void CDyeIcon::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CDyeIcon* CDyeIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pTexName, int iPosX, int iPosY, int iSizeX, int iSizeY)
{
	CDyeIcon*		pGameObj = new CDyeIcon(pGraphicDev);

	//m_pTexName = new TCHAR[128];
	//lstrcpy( m_pTexName, pTexName );
	//m_iPosX = iPosX;
	//m_iPosY = iPosY;

	if(FAILED(pGameObj->Initialize(pTexName, iPosX, iPosY, iSizeX, iSizeY)))
	{
		MSG_BOX("pLogoBack Create Faild");
		::Safe_Release(pGameObj);
	}
	return pGameObj;
}

void CDyeIcon::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView;
	D3DXMatrixIdentity(&matView);

 	matView._11 = (float)m_iSizeX;
 	matView._22 = (float)m_iSizeY;
	matView._33 = 1.f;

	matView._41 = (float)m_iX;
	matView._42 = (float)m_iY;
	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);
	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", m_iSelect);
	//printf("%d\n", (int)m_bSelect);

}

_ulong CDyeIcon::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		Engine::Safe_Delete_Array(m_pTexName);
		delete this;
	}

	return dwRefCnt;
}

