#include "stdafx.h"

#include "DirectXFramework.h"

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd)
{
	srand((unsigned)time(NULL));
	InitSingleton(hInstance, hWnd);
	_tcscpy_s(m_pszBuffer, _T("Impossible Boss ("));		//	GameTitle
}

void CDirectXFramework::End() {
	//camera end
	if (m_pCamera) { m_pCamera->End(); }
	m_pCamera = nullptr;

	//singleton End
	RESOURCEMGR->End();
	DEBUGER->End();
	INPUTMGR->End();
	UPDATER->End();
	SCENEMGR->End();
	NETWORKMGR->End();
	RCSELLER->End();
	RENDERER->End();
	CAMMGR->End();

	GLOBALVALUEMGR->ReleseInstance();
	TIMEMGR->ReleseInstance();
	RESOURCEMGR->ReleseInstance();
	RCSELLER->ReleseInstance();
	DEBUGER->ReleseInstance();
	INPUTMGR->ReleseInstance();
	DIRECTORYFINDER->ReleseInstance();
	IMPORTER->ReleseInstance();
	RENDERER->ReleseInstance();
	UPDATER->ReleseInstance();
	SCENEMGR->ReleseInstance();
	CAMMGR->ReleseInstance();
	NETWORKMGR->ReleseInstance();

	CEffectMgr::GetInstance()->End();
	CEffectMgr::GetInstance()->ReleseInstance();
	CNaviObjectManager::End();
	CPositionInfoManager::End();
	CSoundManager::End();
}
void CDirectXFramework::FrameAdvance()
{
	TIMEMGR->Tick();

	Update(TIMEMGR->GetTimeElapsed());
	Render();

	DEBUGER->AddText(25.f, 100.f, 700.f, YT_Color(255.f, 0.f, 0.f), m_pszBuffer);
	TIMEMGR->GetFrameRate(m_pszBuffer + 17, 35);
	::SetWindowText(GLOBALVALUEMGR->GethWnd(), m_pszBuffer);
}
void CDirectXFramework::Update(float fTimeElapsed) {
	//postprocessinglayer 적응값 set
	if (nullptr != SCENEMGR->GetPresentScene())
	{
		if (false == SCENEMGR->GetPresentScene()->GetIsLoading())
		{
			RENDERER->Update(fTimeElapsed);
			ProcessInput(fTimeElapsed);
			UPDATER->PreUpdate(fTimeElapsed);
		}

		CSoundManager::Update();
		SCENEMGR->GetPresentScene()->Animate(fTimeElapsed);
		if (false == SCENEMGR->GetPresentScene()->GetIsLoading())
		{
			UPDATER->Update(fTimeElapsed);
			UPDATER->PhisicsUpdate(fTimeElapsed);
		}
	}
}


void CDirectXFramework::Render() {
	//-----------------------------카메라 버퍼 set------------------------------------

	if (nullptr != SCENEMGR->GetPresentScene())
	{
		if (true == SCENEMGR->GetPresentScene()->GetIsLoading())
			RENDERER->PreRender();
		else if (SCN_HEROSEL == SCENEMGR->GetSceneID())
		{
			RENDERER->NoPostProcessRender(m_pCamera);
		}
		else
		{
			//m_pCamera->SetShaderState();
			RENDERER->Render(m_pCamera);
		}
	}
}


void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------현재 씬 실행--------------------------------------
	if (nullptr != SCENEMGR->GetPresentScene())
		SCENEMGR->GetPresentScene()->ProcessInput(fTimeElapsed);
}
LRESULT CALLBACK CDirectXFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	INPUTMGR->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
	return(0);
}

CDirectXFramework::~CDirectXFramework() {

}

void CDirectXFramework::InitSingleton(HINSTANCE hInstance, HWND hWnd)
{
	//singleton Init 
	GLOBALVALUEMGR->Begin(hInstance, hWnd);
#ifdef NO_SERVER
	NETWORKMGR->AddServerPlayerInfo(0);
#endif
	Make_Camera();

	RESOURCEMGR->Begin();
	RCSELLER->Begin();
	DEBUGER->Begin();
	INPUTMGR->Begin();
	RENDERER->Begin();
	CAMMGR->Begin();
	UPDATER->Begin();
	SCENEMGR->Begin(this);
	NETWORKMGR->Begin();

	CNaviObjectManager::Begin();
	CPositionInfoManager::Begin();
	CSoundManager::Begin();

#ifdef NO_SERVER
	NETWORKMGR->GetServerPlayerInfos()[NETWORKMGR->GetSLOT_ID()].CHARACTER = 0;
	SCENEMGR->ChangeScene(SCN_ORITOWN);	//SCENEMGR->ChangeScene(SCN_ALDENAD);
	//SCENEMGR->ChangeScene(SCN_BOSS);
	//SCENEMGR->ChangeScene(SCN_TITLE);
	return;
#endif

	SCENEMGR->ChangeScene(SCN_TITLE);
}

void CDirectXFramework::Make_Camera()
{

	//camera default
	m_pCamera = new CFreeCamera();
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(60.0f * XM_PI / 180.0f		// FOV Y 값 : 클 수록 멀리까지 볼 수 있다.
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)// 종횡비
		, NEAR_PLANE, FAR_PLANE);// 최대 거리

	XMVECTOR eye = { 0.0f, 12.5f, 13.0f, 0.0f };
	XMVECTOR at = { 0.0f, 2.30f, 0.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_pCamera->SetLookAt(eye, at, up);
	m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	CAMMGR->SetCamera(CAM_FREE, m_pCamera);


	CCamera* pCamera = new CFreeCamera;
	pCamera->Begin();
	pCamera->GenerateProjectionMatrix(60.0f * XM_PI / 180.0f		// FOV Y 값 : 클 수록 멀리까지 볼 수 있다.
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)// 종횡비
		, NEAR_PLANE, FAR_PLANE);// 최대 거리
	eye = { 0.0f, 0.f, 0.0f, 0.0f };
	at = { 0.0f, 0.0f, 1.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	pCamera->SetLookAt(eye, at, up);
	pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	CAMMGR->SetCamera(CAM_LIGHT, pCamera);
}
