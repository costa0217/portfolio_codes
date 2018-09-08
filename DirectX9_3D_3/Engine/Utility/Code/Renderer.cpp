#include "Renderer.h"
#include "Scene.h"

#include "Export_Function.h"


Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pD3DXFont(NULL)
, m_fTime(0.f)
, m_iFrameCnt(0)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
}

Engine::CRenderer::~CRenderer(void)
{

}

void Engine::CRenderer::SetSceneChange(CScene* pScene)
{
	m_pScene = pScene;
}

HRESULT Engine::CRenderer::InitScene(void)
{
	D3DXFONT_DESC		hFont;
	ZeroMemory(&hFont, sizeof(D3DXFONT_DESCW));
	hFont.Width = 9;
	hFont.Height = 20;
	hFont.Weight = FW_NORMAL;
	hFont.CharSet = DEFAULT_CHARSET;
	lstrcpy(hFont.FaceName, L"Plantagenet Cherokee");

	FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &hFont, &m_pD3DXFont));
	return S_OK;
}

void Engine::CRenderer::Render(float fTime)
{
	RECT		rc	= {100, 100, 800, 600};
	RECT		rc2	= {100, 500, 800, 600};

	CResourcesMgr*		m_pResourceMgr = Get_ResourcesMgr();
	int iCnt = m_pResourceMgr->GetCnt();
	
	wsprintf(m_szCnt, L"Cnt : %d",iCnt );

	++m_iFrameCnt;
	m_fTime += fTime;

	if(1.f < m_fTime)
	{
		wsprintf(m_szFps, L"FPS : %d", m_iFrameCnt);
		m_fTime = 0.f;
		m_iFrameCnt = 0;
	}

	m_pDevice->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 34, 116, 28)
		, 1.f, 0);
	m_pDevice->BeginScene();

	//m_pD3DXFont->DrawText(NULL, m_szFps, lstrlen(m_szFps), &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));
	//m_pD3DXFont->DrawText(NULL, m_szCnt, lstrlen(m_szCnt), &rc2, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(m_pScene != NULL)
		m_pScene->Render();

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer*	pRenderer = new CRenderer(pDevice);
	if(FAILED(pRenderer->InitScene()))
		Engine::Safe_Delete(pRenderer);

	return pRenderer;
}