#include "Management.h"

#include "Renderer.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
: m_pDevice(NULL)
, m_pRenderer(NULL)
, m_pScene(NULL)
{

}

Engine::CManagement::~CManagement(void)
{
	Release();
}

const Engine::VTXTEX* Engine::CManagement::GetTerrainVtx(const WORD& LayerID, const wstring& wstrObjKey)
{
	return m_pScene->GetTerrainVtx(LayerID, wstrObjKey);
}

HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	m_pRenderer = CRenderer::Create(pDevice);
	NULL_CHECK_RETURN(m_pRenderer, E_FAIL);

	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if(m_pScene != NULL)
		m_pScene->Update();
}

void Engine::CManagement::Render(float fTime)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render(fTime);
}

void Engine::CManagement::Release(void)
{
	Engine::Safe_Delete(m_pRenderer);
	Engine::Safe_Delete(m_pScene);
}
