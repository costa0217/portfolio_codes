#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "SceneSelector.h"
#include "MathMgr.h"
#include "EffectMgr.h"
#include "CullingMgr.h"
#include "SoundMgr.h"
#include "Player.h"

#include "DynamicCamera.h"
#include "PlayerHeartBeatUI.h"
#include "FadeInOut.h"
#include "Cursor.h"

#include "PlayerBeShotUI.h"
#include "EquipmentUI.h"
#include "SkillFailFont.h"

//#include <boost/mersenne_twister.h>

CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT	CMainApp::Initialize(void)
{
	srand(unsigned(time(NULL)));
	//mt19937 engine((unsigned int)time(NULL));       // MT19937 난수 엔진

	// 장치를 초기화한다.
	if(FAILED(Engine::Ready_GraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINSIZEX, WINSIZEY)))
	{
		MSG_BOX("Ready_GraphicDev Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Management(Engine::Get_GraphicDev())))
	{
		MSG_BOX("Ready_Management Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Font(Engine::Get_GraphicDev(), L"Font_Default", 15, 25, FW_HEAVY)))
	{
		MSG_BOX("Ready_Font Failed");
		return E_FAIL;
	}

	CSoundMgr::GetInstance()->Initialize();
	CSoundMgr::GetInstance()->SoundLoading();
	Engine::Reserve_ContainerSize(RESOURCE_END);

	//페이드 인 아웃 버퍼랑 텍스쳐 로딩
	if(FAILED(Engine::Ready_Buffer(Engine::Get_GraphicDev(), RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_FadeInOut")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return E_FAIL;
	}


	if(FAILED(Engine::Ready_Texture(Engine::Get_GraphicDev(), RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FadeInOut", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/FadeInOut%d.png", 1)))
	{
		MSG_BOX("Texture_FadeInOut Ready Failed");
		return E_FAIL;
	}

	CLoading::m_eLoadingType = CLoading::LOADING_DYE;
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

void CMainApp::Render(const _float& fTimeDelta)
{
	Engine::Render_Management(fTimeDelta);
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
		//Engine::CLightMgr::GetInstance()->DestroyInstance();
		CPlayer::GetInstance()->DestroyInstance();
		CPlayerHeartBeatUI::GetInstance()->DestroyInstance();
		CFadeInOut::GetInstance()->DestroyInstance();
		CDynamicCamera::GetInstance()->DestroyInstance();
		CCursor::GetInstance()->DestroyInstance();
		CPlayerBeShotUI::GetInstance()->DestroyInstance();
		CEquipmentUI::GetInstance()->DestroyInstance();
		CSkillFailFont::GetInstance()->DestroyInstance();

		CEffectMgr::GetInstance()->DestroyInstance();
		CMathMgr::GetInstance()->DestroyInstance();
		CCullingMgr::GetInstance()->DestroyInstance();
		CSoundMgr::GetInstance()->DestroyInstance();
		Engine::CSceneNumMgr::GetInstance()->DestroyInstance();

		Engine::Release_Utility();
		Engine::Release_Resource();		
		Engine::Release_System();

		delete this;
	}

	return dwRefCnt;
}



