#include "stdafx.h"
#include "SelectDye.h"

#include "Export_Function.h"

#include "Player.h"
#include "Sword.h"

#include "SoundMgr.h"

int CSelectDye::m_iPart = 0;
int CSelectDye::m_iRGB = 0;

CSelectDye::CSelectDye(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_iX(8)
, m_iY(5)
, m_eSelectRGB(RGB_END)
, m_eSelectPart(DYEPT_END)
, m_bPlay(false)
, m_fClick(0.f)
, m_fClickOverTime(0.f)
{
	ZeroMemory( m_vSelectColor, sizeof(_vec3) * RGB_END );

	for( int i = 0; i < DYEPT_END; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			//D3DXMatrixIdentity(&m_matColor[i]);
			memcpy(&m_matColor[i].m[j][0], &(_vec4(1.f, 1.f, 1.f, 0.f)), sizeof(_vec4));
		}
	}

}

CSelectDye::~CSelectDye(void)
{

}

HRESULT CSelectDye::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI(Alpha)");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	for( int i = 0; i < 5; ++i )
	{
		for( int j = 0; j < 8; ++j )
		{
			int fTop = 555 + (50*i);
			int iRight = 40 + ( 50 * j );
			SetRect(&m_rcUI[i][j], iRight , fTop, iRight + 40 , fTop + 40);
		}
	}

	Set_ColorValue();

	return S_OK;
}

HRESULT CSelectDye::Add_Component(void)
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
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_DYE, L"Buffer_SelectDyeColor");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_DYE, L"Texture_SelectDyeColor");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

_int CSelectDye::Update(const _float& fTimeDelta)
{
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

	POINT			ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	for( int i = 0; i < 5; ++i )
	{
		for( int j = 0; j < 8; ++j )
		{
			if(PtInRect(&m_rcUI[i][j], ptMouse) && !m_bPlay && ( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
			{
				if( !m_bPlay )
				{
					m_fClick = fTimeDelta;
					m_bPlay = true;
					printf("마우스 %d\n", (int)m_bPlay);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::Click, _vec3(0, 0, 0), 0.7f);
				}

				printf("dye 마우스 %d, %d\n",i, j);
				m_vSelectColor[m_iRGB] = m_vColor[i][j];

				memcpy(&m_matColor[m_iPart].m[m_iRGB][0], &m_vSelectColor[m_iRGB], sizeof(_vec3) );

				switch(m_iPart)
				{
				case 0:
					CPlayer::GetInstance()->Set_HairColor(m_matColor[0]);
					break;
				case 1:
					CPlayer::GetInstance()->Set_UpperColor(m_matColor[1]);
					break;
				case 2:
					CPlayer::GetInstance()->Set_HandColor(m_matColor[2]);
					break;
				case 3:
					CPlayer::GetInstance()->Set_LowerColor(m_matColor[3]);
					break;
				case 4:
					CPlayer::GetInstance()->Set_FootColor(m_matColor[4]);
					break;
				case 5:
					memcpy( g_tSwordInfo.matColor, m_matColor[5], sizeof(_matrix));
					break;
				}
			}
			if( m_bPlay )
				break;
		}
		if( m_bPlay )
			break;
	}

	return 0;
}

void CSelectDye::Render(void)
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


void CSelectDye::Set_ColorValue(void)
{
	m_vColor[0][0] = _vec3(0.f, 0.f, 0.f);			m_vColor[0][1] = _vec3(25.f, 25.f, 25.f);
	m_vColor[0][2] = _vec3(97.f, 97.f, 97.f);		m_vColor[0][3] = _vec3(230.f, 230.f, 230.f);
	m_vColor[0][4] = _vec3(255.f, 255.f, 255.f);	m_vColor[0][5] = _vec3(66.f, 41.f, 33.f);
	m_vColor[0][6] = _vec3(90.f, 57.f, 49.f);		m_vColor[0][7] = _vec3(199.f, 177.f, 124.f);

	m_vColor[1][0] = _vec3(140.f, 36.f, 35.f);		m_vColor[1][1] = _vec3(171.f, 40.f, 38.f);
	m_vColor[1][2] = _vec3(220.f, 15.f, 15.f);		m_vColor[1][3] = _vec3(200.f, 35.f, 35.f);
	m_vColor[1][4] = _vec3(220.f, 35.f, 35.f);		m_vColor[1][5] = _vec3(175.f, 30.f, 62.f);
	m_vColor[1][6] = _vec3(255.f, 122.f, 165.f);	m_vColor[1][7] = _vec3(242.f, 189.f, 189.f);

	m_vColor[2][0] = _vec3(17.f, 20.f, 47.f);		m_vColor[2][1] = _vec3(20.f, 20.f, 116.f);
	m_vColor[2][2] = _vec3(15.f, 15.f, 220.f);		m_vColor[2][3] = _vec3(35.f, 35.f, 200.f);
	m_vColor[2][4] = _vec3(35.f, 99.f, 200.f);		m_vColor[2][5] = _vec3(66.f, 164.f, 153.f);
	m_vColor[2][6] = _vec3(136.f, 170.f, 203.f);	m_vColor[2][7] = _vec3(167.f, 218.f, 255.f);

	m_vColor[3][0] = _vec3(36.f, 85.f, 59.f);		m_vColor[3][1] = _vec3(81.f, 220.f, 15.f);
	m_vColor[3][2] = _vec3(70.f, 195.f, 35.f);		m_vColor[3][3] = _vec3(33.f, 188.f, 78.f);
	m_vColor[3][4] = _vec3(156.f, 195.f, 89.f);		m_vColor[3][5] = _vec3(197.f, 186.f, 34.f);
	m_vColor[3][6] = _vec3(249.f, 249.f, 0.f);		m_vColor[3][7] = _vec3(239.f, 250.f, 46.f);

	m_vColor[4][0] = _vec3(72.f, 21.f, 108.f);		m_vColor[4][1] = _vec3(69.f, 28.f, 163.f);
	m_vColor[4][2] = _vec3(92.f, 12.f, 177.f);		m_vColor[4][3] = _vec3(121.f, 21.f, 110.f);
	m_vColor[4][4] = _vec3(81.f, 35.f, 200.f);		m_vColor[4][5] = _vec3(220.f, 84.f, 186.f);
	m_vColor[4][6] = _vec3(253.f, 110.f, 255.f);	m_vColor[4][7] = _vec3(255.f, 191.f, 255.f);

	for( int i = 0; i < m_iY; ++i )
	{
		for( int j = 0; j < m_iX; ++j )
		{
			m_vColor[i][j] = m_vColor[i][j] / 255.f;
		}
	}
}

CSelectDye* CSelectDye::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSelectDye*		pGameObj = new CSelectDye(pGraphicDev);

	if(FAILED(pGameObj->Initialize()))
	{
		MSG_BOX("pLogoBack Create Faild");
		::Safe_Release(pGameObj);
	}
	return pGameObj;
}

void CSelectDye::Set_Part(int iPart)
{
	m_iPart = iPart;
}

void CSelectDye::Set_RGB(int iRGB)
{
	m_iRGB = iRGB;
}

void CSelectDye::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView;
	D3DXMatrixIdentity(&matView);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matView._11 = 480.f;
	matView._22 = 480.f;
	matView._33 = 1.f;

	//matView._41 = 570.f;
	//matView._42 = -260.f;

	matView._41 = -550.f;
	matView._42 = -260.f;

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);
	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 0);

}

void CSelectDye::Set_ColorDefault()
{
	if( Engine::GetDIKeyState('F') & 0x80 )
	{
		//m_matColor[0]._11 = ;		m_matColor[0]._12 = ;		m_matColor[0]._13 = ;
		//m_matColor[0]._21 = ;		m_matColor[0]._22 = ;		m_matColor[0]._23 = ;
		//m_matColor[0]._31 = ;		m_matColor[0]._32 = ;		m_matColor[0]._33 = ;


		//m_matColor[1]._11 = ;		m_matColor[1]._12 = ;		m_matColor[1]._13 = ;
		//m_matColor[1]._21 = ;		m_matColor[1]._22 = ;		m_matColor[1]._23 = ;
		//m_matColor[1]._31 = ;		m_matColor[1]._32 = ;		m_matColor[1]._33 = ;

		//m_matColor[2]._11 = ;		m_matColor[2]._12 = ;		m_matColor[2]._13 = ;
		//m_matColor[2]._21 = ;		m_matColor[2]._22 = ;		m_matColor[2]._23 = ;
		//m_matColor[2]._31 = ;		m_matColor[2]._32 = ;		m_matColor[2]._33 = ;

		//m_matColor[0]._11 = ;		m_matColor[0]._12 = ;		m_matColor[0]._13 = ;
		//m_matColor[0]._21 = ;		m_matColor[0]._22 = ;		m_matColor[0]._23 = ;
		//m_matColor[0]._31 = ;		m_matColor[0]._32 = ;		m_matColor[0]._33 = ;

	}
}

_ulong CSelectDye::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}


