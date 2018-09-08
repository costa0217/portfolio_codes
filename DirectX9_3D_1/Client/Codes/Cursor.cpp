#include "stdafx.h"
#include "Cursor.h"
#include "Export_Function.h"

#include "Player.h"
#include "DynamicCamera.h"

IMPLEMENT_SINGLETON(CCursor)

CCursor::CCursor(void)
: Engine::CGameObject(Engine::Get_GraphicDev())
{
	ZeroMemory(m_matOrtho, sizeof(_matrix));
	Initialize();
}

CCursor::~CCursor(void)
{

}

HRESULT CCursor::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest3)");

	if(NULL == m_pEffect)
	{
		printf("m_pEffect == NULL\n");
		return E_FAIL;
	}

	if(FAILED(Add_Component()))
	{
		printf("Add_Component failed\n");
		return E_FAIL;
	}

	GetCursorPos(&m_pt);
	ScreenToClient(g_hWnd, &m_pt);
	m_fSizeX = 32.f;
	m_fSizeY = 32.f;
	m_fX = (_float)m_pt.x + (m_fSizeX / 2.f);
	m_fY = (_float)m_pt.y + (m_fSizeY / 2.f);

	m_bRender = true;

	return S_OK;
}

HRESULT CCursor::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	// for.trans Component
	pComponent	= m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
	{
		printf("m_pTransCom failed\n");
		return E_FAIL;
	}
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// for.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Cursor");
	if(NULL == pComponent)
	{
		printf("m_pBufferCom failed\n");
		return E_FAIL;
	}
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufCursor", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Cursor");
	if(NULL == pComponent)
	{
		printf("m_pTextureCom failed\n");
		return E_FAIL;
	}
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TextureCursor", pComponent));

	return S_OK;
}

_int CCursor::Update(const _float& fTimeDelta)
{
	if(!CPlayer::GetInstance()->GetInGame())
	{
		Engine::CGameObject::Update(fTimeDelta);

		GetCursorPos(&m_pt);
		ScreenToClient(g_hWnd, &m_pt);
		m_fX = (_float)m_pt.x + (m_fSizeX / 2.f);
		m_fY = (_float)m_pt.y + (m_fSizeY / 2.f);

		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

		//직교투영행렬
		D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	}
	else
	{
		if(CDynamicCamera::GetInstance()->GetModeCheck())
		{
			return 0;
		}
		else
		{
			//m_bFixCamera 가 true이면 마우스를 그리지 않고 false이면 그린다.
			if(CDynamicCamera::GetInstance()->GetbFixMouse())
			{
				return 0;
			}
			else
			{
				/*if(Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON)&0x80)
				{
					CDynamicCamera::GetInstance()->SetbFixMouse(true);
					return 0;
				}*/
				Engine::CGameObject::Update(fTimeDelta);

				GetCursorPos(&m_pt);
				ScreenToClient(g_hWnd, &m_pt);
				m_fX = (_float)m_pt.x + (m_fSizeX / 2.f);
				m_fY = (_float)m_pt.y + (m_fSizeY / 2.f);

				Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

				//직교투영행렬
				D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
			}
		}
	}
	
	return 0;
}

void CCursor::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(1);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}
void CCursor::Set_ContantTable(void)
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

	m_pEffect->SetFloat("g_fAlpha", 1.f);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}
_ulong CCursor::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

