#include "stdafx.h"
#include "DyeScene.h"

#include "Export_Function.h"
#include "SceneSelector.h"
#include "Loading.h"

#include "SelectCloth.h"
#include "SelectDye.h"
#include "SelectPart.h"

#include "Player.h"
#include "DyeSkyBox.h"
#include "DyeCamera.h"
#include "DynamicCamera.h"

#include "SoundMgr.h"

#include "PlayerState.h"
#include "FadeInOut.h"
#include "Cursor.h"
#include "EffectMgr.h"
CDyeScene::CDyeScene(LPDIRECT3DDEVICE9 pGrpahicDev)
: CScene(pGrpahicDev)
, m_fAccTime(0.f)
{
}

CDyeScene::~CDyeScene(void)
{

}

HRESULT CDyeScene::Init_Scene(void)
{
	//Scene Num
	Engine::CSceneNumMgr::GetInstance()->Set_CurrentScene(2);	// Dye

	if(FAILED(Ready_Lighting()))
	{
		MSG_BOX("Ready_Lighting Failed");
		return E_FAIL;
	}

	/*if(FAILED(Ready_Resources()))
	{
		MSG_BOX("Ready_Resources Failed");
		return E_FAIL;
	}*/


	if(FAILED(Ready_GameLogic()))
	{
		MSG_BOX("Ready_GameLogic Failed");
		return E_FAIL;
	}
	if(FAILED(Ready_Environment()))
	{
		MSG_BOX("Ready_Environment Failed");
		return E_FAIL;
	}
	Ready_Effect();

	CSoundMgr::GetInstance()->PlayBGM(SOUND::BGM_StartScene);
	CFadeInOut::GetInstance()->SetFadeInOut(false, 2.f);

	return S_OK;
}

_int CDyeScene::Update(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	CPlayer::GetInstance()->Update(fTimeDelta);
	CDynamicCamera::GetInstance()->Update(fTimeDelta);
	
	CScene::Update(fTimeDelta);

	CCursor::GetInstance()->Update(fTimeDelta);

	CFadeInOut::GetInstance()->Update(fTimeDelta);

	if(m_fAccTime > 0.2f)
	{
		CEffectMgr::GetInstance()->Play_Effect(L"LogoLight",_vec3(float(rand()%50 - 20), float(rand()%10),float(rand()%20)));
		CEffectMgr::GetInstance()->Play_Effect(L"LogoLight",_vec3(float(rand()%50 - 20), float(rand()%10 - 5),float(rand()%20)));
		CEffectMgr::GetInstance()->Play_Effect(L"LogoLight",_vec3(float(rand()%50 - 20), float(rand()%10 - 5),float(rand()%20)));
		CEffectMgr::GetInstance()->Play_Effect(L"LogoLight",_vec3(float(rand()%50 - 20), float(rand()%10 - 5),float(rand()%20)));

		m_fAccTime = 0.f;
	}

	if( CSelectPart::m_bNext )	// 염색화면에서 버튼 누르면 장면 전환되도록
	{
		if( lstrcmp(g_tSwordInfo.szMeshName, L"") == 0 )
			lstrcpy(g_tSwordInfo.szMeshName, L"Mesh_Sword3");
		CLoading::m_eLoadingType = CLoading::LOADING_MIDDLE_STAGE;
		if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_LOGO))))
		{
			MSG_BOX("Change_Scene(SCENE_LOGO) Failed");
			return E_FAIL;
		}
	}

	return  0;
}

void CDyeScene::Render(void)
{
	
}

HRESULT CDyeScene::Ready_Lighting(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(0.f, 0.f, 1.f);

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CDyeScene::Ready_Resources(void)
{
	return S_OK;
}

HRESULT CDyeScene::Ready_GameLogic(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pGraphicDev);

	Engine::CGameObject*		pGameObject = NULL;

	//Player Create
	CPlayer::GetInstance()->SetEnvironmentTexCom(NULL);
	CPlayer::GetInstance()->SetInGame(false);
	CPlayer::GetInstance()->SetAngleY(D3DXToRadian(270.f));

	m_mapLayer.insert(MAPLAYER::value_type(L"GameLogic", pLayer));

	return S_OK;		
}

HRESULT CDyeScene::Ready_Environment(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pGraphicDev);
	m_mapLayer.insert(MAPLAYER::value_type(L"Environment", pLayer));

	Engine::CGameObject*		pGameObject = NULL;

 	//// For.DyeCamera
 	/*pGameObject = CDyeCamera::Create(m_pGraphicDev);
 	if(NULL == pGameObject)
 		return E_FAIL;
 	pLayer->Ready_Object(L"Camera", pGameObject);*/

	//SkyBox ㅊㅂㅇ
	pGameObject = CDyeSkyBox::Create(m_pGraphicDev, 0);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Sky", pGameObject);

	//// For.CSelectCloth Instance
	pGameObject = CSelectCloth::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"SelectCloth", pGameObject);

	//// For.CSelectDye Instance
	pGameObject = CSelectDye::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"SelectDye", pGameObject);

	//// For.CSelectDye Instance
	pGameObject = CSelectPart::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"SelectPart", pGameObject);

	return S_OK;
}

CDyeScene* CDyeScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDyeScene*	pScene = new CDyeScene(pGraphicDev);

	if(FAILED(pScene->Init_Scene()))
	{
		MSG_BOX("pScene Initialized Failed");
		::Safe_Release(pScene);
	}

	return pScene;
}

_ulong CDyeScene::Release(void)
{
	_ulong dwRefCnt = Engine::CScene::Release();

	if(0 == dwRefCnt)
	{
		CSoundMgr::GetInstance()->StopBGM();
		//CSoundMgr::GetInstance()->EnvironmentStopEFF();

		printf("CDyeScene Delete\n");
		Engine::Release_Light();
		Engine::Reset_Resource(RESOURCE_DYE);
		delete this;
	}

	return dwRefCnt;
}

HRESULT CDyeScene::Ready_Effect(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);

	CEffectMgr::GetInstance()->Initialize(m_pGraphicDev, pLayer);

	m_mapLayer.insert(MAPLAYER::value_type(L"Effect", pLayer));

	return S_OK;
}

