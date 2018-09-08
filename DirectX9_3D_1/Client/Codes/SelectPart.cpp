#include "stdafx.h"
#include "SelectPart.h"

#include "Export_Function.h"
#include "SelectDye.h"

#include "SoundMgr.h"

bool CSelectPart::m_bNext = false;

CSelectPart::CSelectPart(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_iSelect(-1)
, m_iSelectPart(-1)
, m_bPlay(false)
, m_fClick(0.f)
, m_fClickOverTime(0.f)
{

}

CSelectPart::~CSelectPart(void)
{

}

HRESULT CSelectPart::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI(Alpha)");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelectPart::Add_Component(void)
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
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_DYE, L"Buffer_SelectPart");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_DYE, L"Texture_SelectPart");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	Initialize_DyeIcon();
	Create_DyeIcon();

	return S_OK;
}

_int CSelectPart::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	if( m_bPlay )
	{
		m_fClickOverTime += fTimeDelta;

		if( m_fClickOverTime >= m_fClick + 0.5f )
		{
			m_fClick = 0.f;
			m_fClickOverTime = 0.f;
			m_bPlay = false;
		}
	}

	Update_DyeIcon(fTimeDelta);

	Select_DyeIcon();

	if( Engine::GetDIKeyState(DIK_V) & 0x80 )
	{
		m_pPartIcon[0]->Set_Select(1);
		CSelectDye::Set_RGB(0);
		m_pPartIcon[1]->Set_Select(0);
		m_pPartIcon[2]->Set_Select(0);

	}
	else if( Engine::GetDIKeyState(DIK_B) & 0x80 )
	{
		m_pPartIcon[1]->Set_Select(1);
		CSelectDye::Set_RGB(1);
		m_pPartIcon[0]->Set_Select(0);
		m_pPartIcon[2]->Set_Select(0);
	}
	else if( Engine::GetDIKeyState(DIK_N) & 0x80 )
	{
		m_pPartIcon[2]->Set_Select(1);
		CSelectDye::Set_RGB(1);
		m_pPartIcon[0]->Set_Select(0);
		m_pPartIcon[1]->Set_Select(0);
	}
	return 0;
}
void  CSelectPart::Render(void)
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

	Render_DyeIcon();
}

void CSelectPart::Initialize_DyeIcon()
{
	SetRect(&m_rcStart, 1445, 800, 1550 , 860);
	
	SetRect(&m_rcUI[0], 1380, 100, 1435 , 155);
	SetRect(&m_rcUI[1], 1380, 170, 1435 , 225);
	SetRect(&m_rcUI[2], 1450, 240, 1605 , 295);
	SetRect(&m_rcUI[3], 1380, 240, 1435 , 295);
	SetRect(&m_rcUI[4], 1380, 310, 1435 , 365);
	SetRect(&m_rcUI[5], 1310, 170, 1365 , 225);

	SetRect(&m_rcPart[0], 1320, 380, 1390, 430);
	SetRect(&m_rcPart[1], 1400, 380, 1470, 430);
	SetRect(&m_rcPart[2], 1480, 380, 1550, 430);
}

void CSelectPart::Select_DyeIcon()
{
	POINT			ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	for( int i = 0; i < 6; ++i )
	{
		if(PtInRect(&m_rcUI[i], ptMouse) && !m_bPlay && ( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
		{
			m_iSelect = i;

			if( !m_bPlay )
			{
				m_fClick = m_fTimeDelta;
				m_bPlay = true;
				printf("마우스 %d\n", (int)m_bPlay);
				CSoundMgr::GetInstance()->PlayEffect(SOUND::Click, _vec3(0, 0, 0), 0.7f);
			}
		}
	}

	for( int i = 0; i < 3; ++i )
	{
		if(PtInRect(&m_rcPart[i], ptMouse) && !m_bPlay && ( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
		{
			m_iSelectPart = i;
			if( !m_bPlay )
			{
				m_fClick = m_fTimeDelta;
				m_bPlay = true;
				printf("마우스 %d\n", (int)m_bPlay);
				CSoundMgr::GetInstance()->PlayEffect(SOUND::Click, _vec3(0, 0, 0), 0.7f);
			}
		}
	}

	if(PtInRect(&m_rcStart, ptMouse)) 
	{
		m_pStartIcon->Set_Select(1);


		if( ( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
		{
			//// 게임 넘어가기
			m_bNext = true;
			if( !m_bPlay )
			{
				m_fClick = m_fTimeDelta;
				m_bPlay = true;
				printf("마우스 %d\n", (int)m_bPlay);
				CSoundMgr::GetInstance()->PlayEffect(SOUND::Click, _vec3(0, 0, 0), 0.7f);
			}
		}
	}
	else
		m_pStartIcon->Set_Select(0);

	

	if( m_iSelect != -1 )
	{
		for( int i = 0; i < 6; ++i )
		{
			if( i != m_iSelect )
				m_pDyeIcon[i]->Set_Select(0);
		}
		m_pDyeIcon[m_iSelect]->Set_Select(1);
		CSelectDye::Set_Part(m_iSelect);
	}

	if( m_iSelectPart != -1 )
	{
		for( int i = 0; i < 3; ++i )
		{
			if( i != m_iSelectPart )
				m_pPartIcon[i]->Set_Select(0);
		}
		m_pPartIcon[m_iSelectPart]->Set_Select(1);
		CSelectDye::Set_RGB(m_iSelectPart);
	}
}

void CSelectPart::Create_DyeIcon()
{
	m_pStartIcon = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_StartIcon", 700, -400, 120, 90);

	m_pDyeIcon[0] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconHead", 620, 310, 75, 75);
	m_pDyeIcon[1] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconUpper", 620, 240, 75, 75);
	m_pDyeIcon[2] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconHand", 690, 170, 75, 75);
	m_pDyeIcon[3] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconLower", 620, 170, 75, 75);
	m_pDyeIcon[4] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconFoot", 620, 100, 75, 75);
	m_pDyeIcon[5] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconSword", 550, 240, 75, 75);

	m_pPartIcon[0] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconPartA", 535, 35, 85, 75);
	m_pPartIcon[1] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconPartB", 625, 35, 85, 75);
	m_pPartIcon[2] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_DyeIconPartC", 715, 35, 85, 75);

}
void CSelectPart::Update_DyeIcon(const _float& fTimeDelta)
{
	m_pStartIcon->Update(fTimeDelta);

	m_pDyeIcon[0]->Update(fTimeDelta);
	m_pDyeIcon[1]->Update(fTimeDelta);
	m_pDyeIcon[2]->Update(fTimeDelta);
	m_pDyeIcon[3]->Update(fTimeDelta);
	m_pDyeIcon[4]->Update(fTimeDelta);
	m_pDyeIcon[5]->Update(fTimeDelta);

	m_pPartIcon[0]->Update(fTimeDelta);
	m_pPartIcon[1]->Update(fTimeDelta);
	m_pPartIcon[2]->Update(fTimeDelta);
}
void CSelectPart::Render_DyeIcon()
{
	m_pStartIcon->Render();

	m_pDyeIcon[0]->Render();
	m_pDyeIcon[1]->Render();
	m_pDyeIcon[2]->Render();
	m_pDyeIcon[3]->Render();
	m_pDyeIcon[4]->Render();
	m_pDyeIcon[5]->Render();

	m_pPartIcon[0]->Render();
	m_pPartIcon[1]->Render();
	m_pPartIcon[2]->Render();
}


CSelectPart* CSelectPart::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSelectPart*		pGameObj = new CSelectPart(pGraphicDev);

	if(FAILED(pGameObj->Initialize()))
	{
		MSG_BOX("pLogoBack Create Faild");
		::Safe_Release(pGameObj);
	}
	return pGameObj;
}

void CSelectPart::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView;
	D3DXMatrixIdentity(&matView);

	matView._11 = 500.f;
	matView._22 = 500.f;
	matView._33 = 1.f;

	matView._41 = 715.f;
	matView._42 = 190.f;
	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);
	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 0);
}

_ulong CSelectPart::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		Engine::Safe_Release(m_pStartIcon);

		for( int i = 0; i < 6; ++i )
			Engine::Safe_Release(m_pDyeIcon[i]);

		for( int i = 0; i < 3; ++i )
			Engine::Safe_Release(m_pPartIcon[i]);

		delete this;
	}

	return dwRefCnt;
}


