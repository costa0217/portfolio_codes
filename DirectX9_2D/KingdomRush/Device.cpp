#include "StdAfx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice(void)
: m_p3D(NULL)
, m_pDevice(NULL)
, m_pSprite(NULL)
, m_pFont(NULL)
, m_pUIFont(NULL)
, m_pStageFont(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::InitDevice(void)
{

	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if(FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, & DeviceCaps)))
	{
		//AfxMessageBox();

		//MessageBox(g_hWnd ,L"장치 생성 실패", L"system error", NULL);

		return E_FAIL;
	}
	
	DWORD		vp;	 // 상태값을 저장하기 위한 플래그 용도의 변수

	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) // 점과 조명에 대한 연산을 수행하는 지에 대한 판단
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		 // 버텍스 프로세싱을 지원한다면 그래픽 카드를 통해서
		
	else	
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// 지원하지 않는다며 CPU를 통해서


	D3DPRESENT_PARAMETERS		d3dpp;					// 장치를 생성할 때 개발자가 세팅해줘야 할 구조체
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);	

	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		//AfxMessageBox(L"Device Create Failed");
		ERR_MSG(L"Device Create Failed");
		return E_FAIL;
	}

	// Sprite com Object Create

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"Sprite Create Failed");
		return E_FAIL;
	}

	// Font com Create

	D3DXFONT_DESCW			FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESCW));

	FontInfo.Height  = 20;
	FontInfo.Width   = 10;
	FontInfo.Weight  = FW_BOLD;
	FontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(FontInfo.FaceName, L"맑은고딕");

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &FontInfo, &m_pFont)))
	{
		ERR_MSG(L"Font Create Failed");
		return E_FAIL;
	}

	// UIFont com Create
	D3DXFONT_DESCW			FontInfo2;
	ZeroMemory(&FontInfo2, sizeof(D3DXFONT_DESCW));

	FontInfo2.Height  = 10;
	FontInfo2.Width   = 7;
	FontInfo2.Weight  = FW_ULTRALIGHT;
	FontInfo2.CharSet = DEFAULT_CHARSET;
	lstrcpy(FontInfo2.FaceName, L"Microsoft PhagsPa");

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &FontInfo2, &m_pUIFont)))
	{
		ERR_MSG(L"Font Create Failed");
		return E_FAIL;
	}

	// StageFont com Create
	D3DXFONT_DESCW			FontInfo3;
	ZeroMemory(&FontInfo3, sizeof(D3DXFONT_DESCW));

	FontInfo3.Height  = 18;
	FontInfo3.Width   = 7;
	FontInfo3.Weight  = FW_NORMAL;
	FontInfo3.CharSet = DEFAULT_CHARSET;
	lstrcpy(FontInfo3.FaceName, L"Plantagenet Cherokee");

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &FontInfo3, &m_pStageFont)))
	{
		ERR_MSG(L"Font Create Failed");
		return E_FAIL;
	}


	return S_OK;
}

void CDevice::Release(void)
{
	m_pStageFont->Release();
	m_pUIFont->Release();
	m_pFont->Release();
	m_pSprite->Release();
	m_pDevice->Release();
	m_p3D->Release();	
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth		= WINCX;
	d3dpp.BackBufferHeight		= WINCY;
	d3dpp.BackBufferFormat		= D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount		= 1;

	d3dpp.MultiSampleType		= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality	= 0;

	d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow			= g_hWnd;	
	d3dpp.Windowed				= TRUE;

	d3dpp.EnableAutoDepthStencil= TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CDevice::Render_Begin(void)
{
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(void)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	
}
LPDIRECT3DDEVICE9 CDevice::GetDevice(void)
{
	return m_pDevice;
}

LPD3DXSPRITE CDevice::GetSprite(void)
{
	return m_pSprite;
}

LPD3DXFONT CDevice::GetFont(void)
{
	return m_pFont;
}
LPD3DXFONT CDevice::GetUIFont(void)
{
	return m_pUIFont;
}
LPD3DXFONT CDevice::GetStageFont(void)
{
	return m_pStageFont;
}

