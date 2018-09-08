#include "stdafx.h"
#include "MainApp.h"
#include "SceneSelector.h"
#include "Export_Function.h"

#include "SDevice.h"

CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT	CMainApp::Initialize(void)
{
	// 장치를 초기화한다.
	if(FAILED(Engine::Ready_GraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINSIZEX, WINSIZEY)))
	{
		MSG_BOX("Ready_GraphicDev Failed");
		return E_FAIL;
	}

	SoundRoad();

	if(FAILED(Engine::Ready_Management(Engine::Get_GraphicDev())))
	{
		MSG_BOX("Ready_Management Failed");
		return E_FAIL;
	}
	
	if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_LOGO))))
	{
		MSG_BOX("Change_Scene(SCENE_LOGO) Failed");
		return E_FAIL;
	}
	
	
	return S_OK;
}

_int CMainApp::Update(const _float& fTimeDelta)
{
	return Engine::Update_Management(fTimeDelta);
}

void CMainApp::Render(void)
{
	Engine::Render_Management();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*		pMainApp = new CMainApp;

	if(FAILED(pMainApp->Initialize()))
	{
		MSG_BOX("CMainApp Create Failed");
		::Safe_Release(pMainApp);
	}

	return pMainApp;
}

_ulong CMainApp::Release(void)
{
	_ulong dwRefCnt = CBase::Release();

	if(0 == dwRefCnt)
	{
		Engine::Release_Utility();
		Engine::Release_Resource();		
		Engine::Release_System();
		CSDevice::GetInstance()->DestroyInstance();

		delete this;
	}

	return dwRefCnt;
}

void CMainApp::SoundRoad(void)
{
	CSDevice::GetInstance()->Init();

	CSDevice::GetInstance()->LoadWave(L"../Sound/Loading.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BackGround.wav");
	

	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Skill1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Skill2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Skill3.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Skill4.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Skill5.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Attack1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Attack2.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Openning.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/BossShockGround.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BossSwordShot.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BrokenIce.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/CreateIce.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/MagicCircle.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/MateoShot.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/MateoWait1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Attack1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Attack2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Attack3.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/SkillSwirl.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Wang.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Wind.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/FireTrail.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_PassOut.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BossDMG1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BossDMG2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Jump.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Skill1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Skill2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Skill3.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_Skill4.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_DMG1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_DMG2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Player_DMG3.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_In.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Out.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/PlayerRun1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/PlayerRun2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/PlayerRun3.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Boss_Death.wav");
}


