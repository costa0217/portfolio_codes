#include "stdafx.h"
#include "Logo.h"

#include "Include.h"
#include "Engine_Include.h"
#include "Export_Function.h"

#include "SceneSelector.h"

#include "RcColor.h"
#include "Layer.h"
#include "GameObject.h"
#include "LogoBack.h"
#include "StaticCamera.h"
#include "Transform.h"
#include "CollisionMgr.h"
#include "ExitButton.h"
#include "OptionButton.h"
#include "TutorialButton.h"
#include "PlayButton.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
{

}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT CLogo::InitScene(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	HRESULT hr = NULL;
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	//Collision_ProtoType
	//m_pCollisionMgr->AddColObject(CCollisionMgr::COL_MOUSE);


	FAILED_CHECK(Add_GameLogic_Layer());
	FAILED_CHECK(Add_Camera_Layer());

	return S_OK;
}

void CLogo::Update(void)
{
	Engine::CScene::Update();


	if(((CPlayButton*)m_playbutton)->GetBool())
	{
		Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_STAGE));
		return ;
	}
}

void CLogo::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	Engine::CScene::Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pScene = new CLogo(pDevice);
	if(FAILED(pScene->InitScene()))
		Engine::Safe_Delete(pScene);

	return pScene;
}

HRESULT CLogo::Add_GameLogic_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CLogoBack::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"LogoBack 积己 角菩");
	pLayer->AddObject(L"LogoBack", pGameObject);

	pGameObject = CExitButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"ExitButton 积己 角菩");
	pLayer->AddObject(L"ExitButton", pGameObject);

	pGameObject = COptionButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"OptionButton 积己 角菩");
	pLayer->AddObject(L"OptionButton", pGameObject);

	pGameObject = CTutorialButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"TutorialButton 积己 角菩");
	pLayer->AddObject(L"TutorialButton", pGameObject);

	pGameObject = CPlayButton::Create(m_pDevice);
	m_playbutton = pGameObject;
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"PlayButton 积己 角菩");
	pLayer->AddObject(L"PlayButton", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CLogo::Add_Camera_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	MAPLAYER::iterator		iter = m_mapLayer.find(Engine::LAYER_GAMELOGIC);
	if(iter == m_mapLayer.end())
		return E_FAIL;

	const Engine::CComponent*		pTransform = iter->second->GetComponent(L"LogoBack", L"Transform");

	pGameObject = CStaticCamera::Create(m_pDevice, dynamic_cast<const Engine::CTransform*>(pTransform));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"StaticCamera 积己 角菩");
	pLayer->AddObject(L"StaticCamera", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_CAMERA, pLayer));
	return S_OK;
}

void CLogo::Release(void)
{
	//m_pResourceMgr->ResetDynamic();
}