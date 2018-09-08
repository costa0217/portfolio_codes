#include "stdafx.h"
#include "Stage2.h"

#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"

#include "Layer.h"
#include "Player.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "Transform.h"
#include "CollisionMgr.h"
#include "Monster.h"
#include "..\..\Engine\Resources\Code\CubeTex.h"
#include "Engine_Include.h"
#include "Cube.h"
#include "GameObject.h"
#include "Water.h"
#include "HPbar.h"
#include "BulletBar.h"
#include "TimeUI.h"
#include "CargoUI.h"
#include "LimitUI.h"
#include "HPValue.h"
#include "BulletValue.h"
#include "Man.h"

#include "Nemo.h"
#include "StaticObject.h"
#include "Scene.h"
#include "ObjDataMgr.h"

#include "AlphaBack.h"
#include "GoMainButton.h"
#include "RestartButton.h"
#include "BackButton.h"
#include "Rain.h"
#include "Retona.h"
#include "Dodge.h"
#include "Bunker.h"
#include "AirGun.h"
#include "Tank.h"

#include "ButtonMgr.h"
#include "StartButton.h"
#include "HeliStatus.h"
#include "KaiButton.h"
#include "ChinookButton.h"
#include "WeaponStatus.h"
#include "GunButton.h"
#include "GatlingButton.h"
#include "LauncherButton.h"
#include "Freight.h"
#include "Hook.h"
#include "ItemHp.h"
#include "ItemBullet.h"
#include "Tree.h"
#include "Cloud.h"
#include "ClearImage.h"


CStage2::CStage2(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_bInputUI(false)
, m_bbb(false)
{

}

CStage2::~CStage2(void)
{
	Release();
}

HRESULT CStage2::InitScene(void)
{
	StageNum = 2;
	SetUp();

	HRESULT		hr = NULL;

	//Collision_ProtoType
	m_pCollisionMgr->AddColObject(CCollisionMgr::COL_TERRAIN);


	CObjDataMgr::GetInstance()->LoadObjectPositionFromFile(L"../Data/Map/Tree.dat");


	FAILED_CHECK(Add_GameLogic_Layer());
	FAILED_CHECK(Add_Camera_Layer());
	FAILED_CHECK(Add_Rain_Layer());
	FAILED_CHECK(Add_Choice_Layer());
	FAILED_CHECK(Add_UI_Layer());
	FAILED_CHECK(Add_Pause_Layer());
	
	
	iCLEARNUM = 0;

	return S_OK;
}

void CStage2::Update(void)
{
	Engine::CScene::Update();

	if(!m_bInputUI && ((CStartButton*)m_StartButton)->GetBool())
	{
		m_bInputUI = true;
		m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, m_pUILayer));		
		m_bStart = true;
	}

	if(((CGoMainButton*)m_GoMainButton)->GetBool())
	{
		Engine::CScene::SetBool(false);
		Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_LOGO));
		return ;
	}

	if(((CRestartButton*)m_RestartButton)->GetBool())
	{
		Engine::CScene::SetBool(false);
		//m_pResourceMgr->ResetDynamic();
		Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_STAGE2));
		return;
	}

	if(HangCount >= 2)
	{
		if(!m_bbb)
		{
			pLayer->AddObject(L"ClearImage", pGameObject);
			m_bbb = true;
		}

		if(GetAsyncKeyState(VK_RETURN))
		{
			HangCount = 0;
			//Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_STAGE2));
			return ;
		}
	}

	if(GetAsyncKeyState('H'))
	{
		++HangCount;
	}
}

void CStage2::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	Engine::CScene::Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CStage2* CStage2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage2* pScene = new CStage2(pDevice);
	if(FAILED(pScene->InitScene()))
		Engine::Safe_Delete(pScene);

	return pScene;
}

HRESULT CStage2::Add_Pause_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CAlphaBack::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"AlphaBack 생성 실패");
	pLayer->AddObject(L"AlphaBack", pGameObject);

	pGameObject = CGoMainButton::Create(m_pDevice);
	m_GoMainButton = pGameObject;
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"GoMainButton 생성 실패");
	pLayer->AddObject(L"GoMainButton", pGameObject);

	pGameObject = CRestartButton::Create(m_pDevice);
	m_RestartButton = pGameObject;
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"RestartButton 생성 실패");
	pLayer->AddObject(L"RestartButton", pGameObject);

	pGameObject = CBackButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"BackButton 생성 실패");
	pLayer->AddObject(L"BackButton", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_PAUSE, pLayer));
	return S_OK;
}

HRESULT CStage2::Add_GameLogic_Layer(void)
{
	pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pPlayer		= NULL;
	pGameObject = NULL;
		
	pPlayer = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pPlayer, E_FAIL, L"CPlayer 생성 실패");
	((CPlayer*)pPlayer)->SetMapObjlist(pLayer->GetMapObjlist());
	((CPlayer*)pPlayer)->Initialize();
	pLayer->AddObject(L"Player", pPlayer);
	CButtonMgr::GetInstance()->AddButton(L"Player", pPlayer);

	CTerrain* pTerrain = NULL;
	pGameObject = pTerrain = CTerrain::Create(m_pDevice, TERRAIN_FRONT); // 0806
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTerrain 생성 실패");
	pLayer->AddObject(L"Terrain", pGameObject);

	CTerrain* pTerrain_Mid = NULL;
	pGameObject = pTerrain_Mid = CTerrain::Create(m_pDevice, TERRAIN_MID);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTerrain 생성 실패");
	pLayer->AddObject(L"Terrain", pGameObject);

	CTerrain* pTerrain_Back = NULL;
	pGameObject = pTerrain_Back = CTerrain::Create(m_pDevice, TERRAIN_BACK);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTerrain 생성 실패");
	pLayer->AddObject(L"Terrain", pGameObject);

	CTerrain* pTerrain_CliffLeft = NULL;
	pGameObject = pTerrain_CliffLeft = CTerrain::Create(m_pDevice, TERRAIN_CLIFFLEFT);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTerrain 생성 실패");
	pLayer->AddObject(L"Terrain", pGameObject);

	CTerrain* pTerrain_CliffRight = NULL;
	pGameObject = pTerrain_CliffRight = CTerrain::Create(m_pDevice, TERRAIN_CLIFFRIGHT);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTerrain 생성 실패");
	pLayer->AddObject(L"Terrain", pGameObject);

	CTerrain* pTerrain_Sky = NULL;
	pGameObject = pTerrain_Sky = CTerrain::Create(m_pDevice, TERRAIN_SKY);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTerrain 생성 실패");
	pLayer->AddObject(L"Terrain", pGameObject);


	float	fx		= 50.f;
	float	fz		= 50.f;

	bool	Right	= true;
	bool	Left	= false;

	for(int i=0; i<5; ++i)
	{
		for(int j=0; j<5; ++j)
		{

			D3DXVECTOR3		vPos = D3DXVECTOR3(fx, 10.f, fz);

			pGameObject = CWater::Create(m_pDevice);
			((CWater*)pGameObject)->SetPos(vPos);

			if(j % 2 ==0)
			{
				((CWater*)pGameObject)->SetMoveDir(Right);
			}
			else
			{
				((CWater*)pGameObject)->SetMoveDir(Left);
			}

			fx += 2;

			NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Water 생성 실패");
			pLayer->AddObject(L"Water", pGameObject);

		}
		fx = 50.f;
		fz += 2;
	}


	//Monster

	pGameObject = CAirGun::Create(m_pDevice);
	((CAirGun*)pGameObject)->SetPos(D3DXVECTOR3(320.f, 0.f, 130.f));
	((CAirGun*)pGameObject)->SetPlayer(pPlayer);
	((CAirGun*)pGameObject)->SetMapObjlist(pLayer->GetMapObjlist());
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CAirGun 생성 실패");
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CAirGun::Create(m_pDevice);
	((CAirGun*)pGameObject)->SetPos(D3DXVECTOR3(800.f, 0.f, 100.f));
	((CAirGun*)pGameObject)->SetPlayer(pPlayer);
	((CAirGun*)pGameObject)->SetMapObjlist(pLayer->GetMapObjlist());
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CAirGun 생성 실패");
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CAirGun::Create(m_pDevice);
	((CAirGun*)pGameObject)->SetPos(D3DXVECTOR3(1550.f, 0.f, 90.f));
	((CAirGun*)pGameObject)->SetPlayer(pPlayer);
	((CAirGun*)pGameObject)->SetMapObjlist(pLayer->GetMapObjlist());
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CAirGun 생성 실패");
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CTank::Create(m_pDevice);
	((CTank*)pGameObject)->SetPos(D3DXVECTOR3(400.f, 0.f, 80.f));
	((CTank*)pGameObject)->SetPlayer(pPlayer);
	((CTank*)pGameObject)->SetMapObjlist(pLayer->GetMapObjlist());
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTank 생성 실패");
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CTank::Create(m_pDevice);
	((CTank*)pGameObject)->SetPos(D3DXVECTOR3(1000.f, 0.f, 120.f));
	((CTank*)pGameObject)->SetPlayer(pPlayer);
	((CTank*)pGameObject)->SetMapObjlist(pLayer->GetMapObjlist());
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CTank 생성 실패");
	pLayer->AddObject(L"Monster", pGameObject);

	pGameObject = CRetona::Create(m_pDevice);
	((CRetona*)pGameObject)->SetPos(D3DXVECTOR3(950.f, 0.f, 90.f));
	((CRetona*)pGameObject)->SetMapObjlist(pLayer->GetMapObjlist());
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CRetona 생성 실패");
	pLayer->AddObject(L"Monster", pGameObject);


	for( int i = 0; i < 4; ++i)
	{
		pGameObject = CMan::Create(m_pDevice);
		((CMan*)pGameObject)->SetPos(D3DXVECTOR3(1500.f + float(rand()%120), 0.f, 100.f + float(rand()%40)));
		((CMan*)pGameObject)->SetMistMap(pLayer->GetMapObjlist());
		((CMan*)pGameObject)->SetPlayer(pPlayer);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CMan 생성 실패");
		pLayer->AddObject(L"Man", pGameObject);
	}

	for( int i = 0; i < 4; ++i)
	{
		pGameObject = CMan::Create(m_pDevice);
		((CMan*)pGameObject)->SetPos(D3DXVECTOR3(250.f + float(rand()%130), 0.f, 100.f + float(rand()%30)));
		((CMan*)pGameObject)->SetMistMap(pLayer->GetMapObjlist());
		((CMan*)pGameObject)->SetPlayer(pPlayer);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CMan 생성 실패");
		pLayer->AddObject(L"Man", pGameObject);
	}



	pGameObject = CCloud::Create(m_pDevice);
	((CCloud*)pGameObject)->SetBufferName(L"Buffer_FrontCloud");
	((CCloud*)pGameObject)->Initialize();
	((CCloud*)pGameObject)->SetPos(D3DXVECTOR3(1000.f, 200.f, 600.f));
	((CCloud*)pGameObject)->SetScale( D3DXVECTOR3(11.f, 8.f, 13.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Cloud 생성 실패");
	pLayer->AddObject(L"Object", pGameObject);

	pGameObject = CCloud::Create(m_pDevice);
	((CCloud*)pGameObject)->SetBufferName(L"Buffer_MidCloud");
	((CCloud*)pGameObject)->Initialize();
	((CCloud*)pGameObject)->SetPos(D3DXVECTOR3(500.f, 200.f, 800.f));
	((CCloud*)pGameObject)->SetScale(D3DXVECTOR3(11.f, 8.f, 13.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Cloud 생성 실패");
	pLayer->AddObject(L"Object", pGameObject);

	pGameObject = CCloud::Create(m_pDevice);
	((CCloud*)pGameObject)->SetBufferName(L"Buffer_MidCloud");
	((CCloud*)pGameObject)->Initialize();
	((CCloud*)pGameObject)->SetPos(D3DXVECTOR3(1300.f, 220.f, 1000.f));
	((CCloud*)pGameObject)->SetScale(D3DXVECTOR3(20.f, 10.f, 20.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Cloud 생성 실패");
	pLayer->AddObject(L"Object", pGameObject);

	pGameObject = CCloud::Create(m_pDevice);
	((CCloud*)pGameObject)->SetBufferName(L"Buffer_FrontCloud");
	((CCloud*)pGameObject)->Initialize();
	((CCloud*)pGameObject)->SetPos(D3DXVECTOR3(700.f, 190.f, 1100.f));
	((CCloud*)pGameObject)->SetScale(D3DXVECTOR3(20.f, 15.f, 20.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Cloud 생성 실패");
	pLayer->AddObject(L"Object", pGameObject);

	pGameObject = CCloud::Create(m_pDevice);
	((CCloud*)pGameObject)->SetBufferName(L"Buffer_Tent");
	((CCloud*)pGameObject)->Initialize();
	((CCloud*)pGameObject)->SetPos(D3DXVECTOR3(1370.f, 5.f, 118.f));
	((CCloud*)pGameObject)->SetAngleY(D3DXToRadian(90.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Tent 생성 실패");
	pLayer->AddObject(L"Object", pGameObject);

	pGameObject = CCloud::Create(m_pDevice);
	((CCloud*)pGameObject)->SetBufferName(L"Buffer_Tent");
	((CCloud*)pGameObject)->Initialize();
	((CCloud*)pGameObject)->SetPos(D3DXVECTOR3(1370.f, 5.f, 90.f));
	((CCloud*)pGameObject)->SetAngleY(D3DXToRadian(90.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Tent 생성 실패");
	pLayer->AddObject(L"Object", pGameObject);

	
	//나무생성
	///
	map<wstring, vector<D3DXVECTOR3>>* pMapTreePos = CObjDataMgr::GetInstance()->GetMapObPos();

	map<wstring, vector<D3DXVECTOR3>>::iterator mapiter = pMapTreePos->begin();
	map<wstring, vector<D3DXVECTOR3>>::iterator mapiter_end = pMapTreePos->end();

	for (; mapiter != mapiter_end; ++mapiter)
	{
		vector<D3DXVECTOR3> vecPos = mapiter->second;
		for(size_t i = 0; i < vecPos.size(); ++i)
		{
			CTree* pTree = CTree::Create(m_pDevice, mapiter->first);
			pTree->SetPos(vecPos[i]);

			pLayer->AddObject(L"Tree", pTree);
		}
	}
	///


	//화물,훅 생성
	Engine::CGameObject*	pHook = NULL;
	
	////화물과 화물훅 세트로생성.======================
	pHook = CHook::Create(m_pDevice);
	((CHook*)pHook)->SetAngle(D3DXToRadian(180.f));
	((CHook*)pHook)->SetPos(D3DXVECTOR3(1651.f,10.f,115.f));
	((CHook*)pHook)->SetState(FREIGHT_HOOK);
	NULL_CHECK_RETURN_MSG(pHook, E_FAIL, L"Freight_Hook 생성 실패");
	pLayer->AddObject(L"Freight_Hook",pHook);

	pGameObject = CFreight::Create(m_pDevice);
	((CFreight*)pGameObject)->SetParentInfo(((CHook*)pHook)->GetInfo());
	((CFreight*)pGameObject)->SetpHook((CHook*)(pHook));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CFreight 생성 실패");
	pLayer->AddObject(L"Freight", pGameObject);
	////=================================


	////화물과 화물훅 세트로생성.======================
	pHook = CHook::Create(m_pDevice);
	((CHook*)pHook)->SetAngle(D3DXToRadian(180.f));
	((CHook*)pHook)->SetPos(D3DXVECTOR3(277.f,10.f,115.f));
	((CHook*)pHook)->SetState(FREIGHT_HOOK);
	NULL_CHECK_RETURN_MSG(pHook, E_FAIL, L"Freight_Hook 생성 실패");
	pLayer->AddObject(L"Freight_Hook",pHook);

	pGameObject = CFreight::Create(m_pDevice);
	((CFreight*)pGameObject)->SetParentInfo(((CHook*)pHook)->GetInfo());
	((CFreight*)pGameObject)->SetpHook((CHook*)(pHook));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CFreight 생성 실패");
	pLayer->AddObject(L"Freight", pGameObject);
	////=================================

	/*pGameObject = CItemHp::Create(m_pDevice);
	((CItemHp*)pGameObject)->SetPlayer(pPlayer);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CItemHp 생성 실패");
	pLayer->AddObject(L"ItemHp", pGameObject);

	pGameObject = CItemBullet::Create(m_pDevice);
	((CItemBullet*)pGameObject)->SetPlayer(pPlayer);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CItemBullet 생성 실패");
	pLayer->AddObject(L"ItemBullet", pGameObject);*/

	pGameObject = CClearImage::Create(m_pDevice);
	((CClearImage*)pGameObject)->SetImage(1);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"ClearImage2 생성 실패");

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));
	
	return S_OK;
}

HRESULT CStage2::Add_Camera_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	MAPLAYER::iterator		iter = m_mapLayer.find(Engine::LAYER_GAMELOGIC);
	if(iter == m_mapLayer.end())
		return E_FAIL;

	const Engine::CComponent*		pTransform = iter->second->GetComponent(L"Player", L"Transform");

	pGameObject = CStaticCamera::Create(m_pDevice, dynamic_cast<const Engine::CTransform*>(pTransform));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"StaticCamera 생성 실패");
	pLayer->AddObject(L"StaticCamera", pGameObject);

	//플레이어의 셋리스트UI를해서 pLayer의 겟리스트를받아야함.
	list<Engine::CGameObject*> Playerlist =((*(iter->second->GetMapObjlist())->find(L"Player")).second);
	((CPlayer*)Playerlist.front())->SetSetMapObjlistUI(pLayer->GetMapObjlist());
	//

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_CAMERA, pLayer));
	return S_OK;
}

HRESULT CStage2::Add_Choice_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CHeliStatus::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"HeliStatus 생성 실패");
	pLayer->AddObject(L"HeliStatus", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"HeliStatus", pGameObject);

	pGameObject = CWeaponStatus::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"WeaponStatus 생성 실패");
	pLayer->AddObject(L"RWeaponStatus", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"RWeaponStatus", pGameObject);

	pGameObject = CKaiButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"KaiButton 생성 실패");
	pLayer->AddObject(L"KaiButton", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"KaiButton", pGameObject);

	pGameObject = CChinookButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"ChinookButton 생성 실패");
	pLayer->AddObject(L"ZChinookButton", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"ZChinookButton", pGameObject);

	pGameObject = CGunButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"ZGunButton 생성 실패");
	pLayer->AddObject(L"ZGunButton", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"ZGunButton", pGameObject);

	pGameObject = CGatlingButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"ZGatlingButton 생성 실패");
	pLayer->AddObject(L"ZGatlingButton", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"ZGatlingButton", pGameObject);

	pGameObject = CLauncherButton::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"ZLancherButton 생성 실패");
	pLayer->AddObject(L"ZLancherButton", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"ZLancherButton", pGameObject);

	pGameObject = CStartButton::Create(m_pDevice);
	m_StartButton = pGameObject;
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"StartButton 생성 실패");
	pLayer->AddObject(L"StartButton", pGameObject);
	CButtonMgr::GetInstance()->AddButton(L"StartButton", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_CHOICE, pLayer));
	return S_OK;
}

HRESULT CStage2::Add_UI_Layer(void)
{
	m_pUILayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CHPBar::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"HPBar 생성 실패");
	m_pUILayer->AddObject(L"HPBar", pGameObject);

	pGameObject = CHPValue::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"HPValue 생성 실패");
	m_pUILayer->AddObject(L"HPValue", pGameObject);

	pGameObject = CBulletBar::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"BulletBar 생성 실패");
	m_pUILayer->AddObject(L"BulletBar", pGameObject);

	pGameObject = CBulletValue::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"BulletValue 생성 실패");
	m_pUILayer->AddObject(L"BulletValue", pGameObject);

	pGameObject = CTimeUI::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"TimeUI 생성 실패");
	m_pUILayer->AddObject(L"TimeUI", pGameObject);

	pGameObject = CCargoUI::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CargoUI 생성 실패");
	m_pUILayer->AddObject(L"CargoUI", pGameObject);

	pGameObject = CLimitUI::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"LimitUI 생성 실패");
	m_pUILayer->AddObject(L"LimitUI", pGameObject);

	//m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));
	return S_OK;
}

HRESULT CStage2::Add_Rain_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CRain::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Rain 생성 실패");
	pLayer->AddObject(L"Rain", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_RAIN, pLayer));
	return S_OK;
}

void CStage2::SetUp(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	/*D3DXMATRIX		matView, matProj;

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(5.f, 5.f, -20.f)
		, &D3DXVECTOR3(5,0,0)
		, &D3DXVECTOR3(0.f, 1.f, 0.f));
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);*/
}

void CStage2::Release(void)
{
	if(!m_bInputUI)
		Engine::Safe_Delete(m_pUILayer);

	//m_pResourceMgr->ResetDynamic();
	m_pCollisionMgr->DestroyInstance();
	CObjDataMgr::GetInstance()->DestroyInstance();
}