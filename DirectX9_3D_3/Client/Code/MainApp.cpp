#include "StdAfx.h"
#include "MainApp.h"

#include "Include.h"
#include "Export_Function.h"

#include "SceneSelector.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
CMainApp::CMainApp(void)
: m_pGraphicDev(Engine::Get_GraphicDev())
, m_pManagement(Engine::Get_Management())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pKeyMgr(Engine::Get_KeyMgr())
, m_pDevice(NULL)
{
}

CMainApp::~CMainApp(void)
{
	Release();
}



HRESULT CMainApp::Initialize(void)
{
	HRESULT		hr = NULL;
	hr = m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY);
	FAILED_CHECK_MSG(hr, L"그래픽 장치 초기화 실패");
	m_pDevice = m_pGraphicDev->GetDevice();

	hr = m_pManagement->InitManagement(m_pDevice);
	FAILED_CHECK_MSG(hr, L"매니지먼트 초기화 실패");

	hr = m_pManagement->SceneChange(CSceneSelector(SCENE_TITLE));
	FAILED_CHECK_MSG(hr, L"씬 전환 실패");

	m_pTimeMgr->InitTime();

	return S_OK;
}

void CMainApp::Update(void)
{
	m_pKeyMgr->GetInstance()->KeyCheck();
	m_pTimeMgr->SetTime();
	m_pManagement->Update();
}

void CMainApp::Render(void)
{
	m_pManagement->Render(m_pTimeMgr->GetTime());
}

void CMainApp::Release(void)
{
	Engine::Safe_Single_Destory(m_pGraphicDev);
	Engine::Safe_Single_Destory(m_pTimeMgr);

	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Get_InfoSubject()->DestroyInstance();

	Engine::Get_ResourcesMgr()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	m_pKeyMgr->GetInstance()->DestroyInstance();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pMainApp = new CMainApp;
	if(FAILED(pMainApp->Initialize()))
	{
		delete pMainApp;
		pMainApp = NULL;
	}

	return pMainApp;
}