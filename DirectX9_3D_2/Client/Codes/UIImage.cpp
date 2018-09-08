#include "stdafx.h"
#include "UIImage.h"
#include "Export_Function.h"

CUIImage::CUIImage(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pFrame(NULL)
{

}

CUIImage::~CUIImage(void)
{

}

HRESULT CUIImage::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlack");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 30.f;
	m_fSizeY = 20.f;	
	m_fX = WINSIZEX * 0.05f;
	m_fY = WINSIZEY * 0.08f;

	// ������������� ������ִ� ����� �Լ���.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	m_pFrame->Initialize(1, 1, 0.f);
	m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);


	return S_OK;
}

HRESULT CUIImage::Add_Component(void)
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
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Class");
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

_int CUIImage::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	

	
	return 0;
}

void CUIImage::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	// ������ɷ����������������� ��������ʰ�, ���̴��� �׸��ڽ��ϴ�.
	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CUIImage* CUIImage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIImage*		pGameObject = new CUIImage(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("pUI Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CUIImage::Set_ContantTable(void)
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


	m_pEffect->SetVector("g_vColor", &m_vColor);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

_ulong CUIImage::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

