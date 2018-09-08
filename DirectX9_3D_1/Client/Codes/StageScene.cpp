#include "stdafx.h"
#include "StageScene.h"

#include "Export_Function.h"

#include "Terrain.h"
#include "DynamicCamera.h"
#include "StaticObject.h"
#include "Player.h"
#include "Sword.h"
#include "PlayerSwordTrail.h"
#include "SkyBox.h"

#include "EffectMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

#include "PlayerState.h"
#include "PlayerSkill.h"
#include "BloodLord.h"
#include "ComboUI.h"
#include "DamageFont.h"

#include "PlayerHeartBeatUI.h"
#include "FadeInOut.h"
#include "Cursor.h"
#include "BLTrail.h"

#include "PlayerBeShotUI.h"
#include "PlayerEyeTrail.h"
#include "EquipmentUI.h"
#include "SkillFailFont.h"

CStageScene::CStageScene(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{
}

CStageScene::~CStageScene(void)
{
	Engine::CNaviMesh::GetInstance()->DestroyInstance();//ㅊㅂㅇ
	Engine::CCameraNaviMesh::GetInstance()->DestroyInstance();
	Engine::CPhysxSDK::GetInstance()->DestroyInstance();

	CCollisionMgr::GetInstance()->DestroyInstance();
}

HRESULT CStageScene::Init_Scene(void)
{
	//Scene Num
	Engine::CSceneNumMgr::GetInstance()->Set_CurrentScene(4);	//스테이지 씬

	//Player Create
	CPlayer::GetInstance()->SetEnvironmentTexCom(NULL);
	CPlayer::GetInstance()->SetNaviCell(NULL);
	CPlayer::GetInstance()->SetInGame(true);
	CPlayer::GetInstance()->SetPos(_vec3(182.f, 0.f, 135.f));
	CPlayer::GetInstance()->SetAngleY(D3DXToRadian(90.f));
	CPlayer::GetInstance()->SetInfoInitialize();

	CDynamicCamera::GetInstance()->SetNaviCell(NULL);

	CFadeInOut::GetInstance()->SetFadeInOut(false, 2.0f);

	if(FAILED(Engine::Ready_PhysxSDK()))
		return E_FAIL;

	if(FAILED(Ready_Lighting()))
		return E_FAIL;

	if(FAILED(Ready_Environment()))
		return E_FAIL;

	if(FAILED(Ready_GameLogic()))
		return E_FAIL;

	if(FAILED(Ready_Effect()))
		return E_FAIL;	

	Load_EnvEffect();

	CDynamicCamera::GetInstance()->Initialize();

	return S_OK;
}

_int CStageScene::Update(const _float& fTimeDelta)
{
	CCollisionMgr::GetInstance()->Update(fTimeDelta);
	Engine::UpdateSimulate(fTimeDelta);

	CPlayer::GetInstance()->Update(fTimeDelta);

	CDynamicCamera::GetInstance()->Update(fTimeDelta);

	//UI관련 업데이트 순서 지켜야 원하는 효과를 볼수 있음
	CPlayerHeartBeatUI::GetInstance()->Update(fTimeDelta);

	CSoundMgr::GetInstance()->Update(fTimeDelta);		//주기적으로 사운드 장치를 업데이트 한다.
	
	CEquipmentUI::GetInstance()->Update(fTimeDelta);
	CPlayerBeShotUI::GetInstance()->Update(fTimeDelta);

	CSkillFailFont::GetInstance()->Update(fTimeDelta);

	Engine::CScene::Update(fTimeDelta);

	CCursor::GetInstance()->Update(fTimeDelta);

	CFadeInOut::GetInstance()->Update(fTimeDelta);//가장 마지막에 업데이트해야한다.

	return 0;
}

void CStageScene::Render(void)
{
	//void CPlayer::Render_BoudingSphere()
	//에 있는 CStageScene::Render 임시 대체 라는 곳으로 옴김
}

CStageScene* CStageScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageScene*	pScene = new CStageScene(pGraphicDev);

	if(FAILED(pScene->Init_Scene()))
	{
		MSG_BOX("CStageScene Initialized Failed");
		::Safe_Release(pScene);
	}
	return pScene;
}

HRESULT CStageScene::Ready_Lighting(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(0.5f, -0.5f, 0.f); // In WorldSpace

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;	

#pragma region 점조명 준비
HANDLE   hFile = CreateFile(L"../Bin/Resources/Data/Map/PointLight_BloodLord_3.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  	DWORD		dwByte;
  	int iIndex = 1;
  	while(TRUE)
  	{
  		D3DLIGHT9* pInfo = new D3DLIGHT9;
		
  		ReadFile(hFile, pInfo, sizeof(D3DLIGHT9), &dwByte, NULL);
  
  		if(dwByte == 0)
  		{
  			Engine::Safe_Delete(pInfo);
  			break;
  		}
  		if(FAILED(Engine::Ready_Light(m_pGraphicDev, pInfo, iIndex))) 
  			return E_FAIL;
  
  		++iIndex;
  
  		Engine::Safe_Delete(pInfo);
  	}
  	CloseHandle(hFile);
#pragma endregion
	return S_OK;
}

HRESULT CStageScene::Ready_Environment(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	m_mapLayer.insert(MAPLAYER::value_type(L"Environment", pLayer));

	Engine::CGameObject*		pGameObject = NULL;

	//SkyBox ㅊㅂㅇ
	pGameObject = CSkyBox::Create(m_pGraphicDev, 0, RESOURCE_STAGE);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Sky", pGameObject);

	// Player HP/ SP / Stamina
	pGameObject = CPlayerState::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"PlayerState", pGameObject);

	// Player_Skill_List
	pGameObject = CPlayerSkill::Create(m_pGraphicDev, 2);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"PlayerSkill2", pGameObject);
	CPlayer::GetInstance()->m_pPlayerSkill = (CPlayerSkill*)pGameObject;

	// Player_Combo
	pGameObject = CComboUI::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"ComboUI", pGameObject);

	CDamageFont::m_pLayer = pLayer;

	return S_OK;
}

HRESULT CStageScene::Ready_GameLogic(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	m_mapLayer.insert(MAPLAYER::value_type(L"GameLogic", pLayer));

	Engine::CGameObject*		pGameObject	= NULL;	
	Engine::CGameObject*		pBloodLord		= NULL;	

	//Terrain Create
 	pGameObject = CTerrain::Create(m_pGraphicDev, RESOURCE_STAGE);
 	if(NULL == pGameObject)
 		return E_FAIL;
 	pLayer->Ready_Object(L"Terrain",pGameObject);

#pragma region 오브젝트 로드
	//StaticObject
	{
		HANDLE	hFile = CreateFile(L"../Bin/Resources/Data/Map/StaticMesh_BloodLord_3.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD		dwByte;

		while(true)
		{
			Engine::OBJINFO	CreateObjectInfo;

			ReadFile(hFile, &CreateObjectInfo, sizeof(Engine::OBJINFO), &dwByte, NULL);

			if(dwByte == 0)
				break;


			if(0 == CreateObjectInfo.iOption) //Default StaticObject 생성
			{
				pGameObject = CStaticObject::Create(m_pGraphicDev, CreateObjectInfo.pName, CStaticObject::SOT_Default);
				if(NULL == pGameObject)
					return E_FAIL;
			}
			else if(1 == CreateObjectInfo.iOption) // Destroy StaticObject 생성
			{
				pGameObject = CStaticObject::Create(m_pGraphicDev, CreateObjectInfo.pName, CStaticObject::SOT_Destroy);
				if(NULL == pGameObject)
					return E_FAIL;
			}

			((CStaticObject*)pGameObject)->SetObjectPos(CreateObjectInfo.vPosition);
			((CStaticObject*)pGameObject)->SetObjectScale(CreateObjectInfo.vScale);
			((CStaticObject*)pGameObject)->SetObjectAngle(CreateObjectInfo.vRotate);
			((CStaticObject*)pGameObject)->SetObjectPassIndex(CreateObjectInfo.iPassIndex);
			((CStaticObject*)pGameObject)->InitializeInfo();
			((CStaticObject*)pGameObject)->Set_Layer(pLayer);
			pLayer->Ready_Object(CreateObjectInfo.pName, pGameObject);
		}

		CloseHandle(hFile);
	}	
#pragma endregion
	
 	//Player Sword Create
	pGameObject = CSword::Create(m_pGraphicDev, g_tSwordInfo.matColor, g_tSwordInfo.szMeshName);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"PlayerSword", pGameObject);

	//Player Sword Trail
	pGameObject = CPlayerSwordTrail::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"PlayerSwordTrail", pGameObject);

	//Player Sword Trail
	pGameObject = CPlayerEyeTrail::Create(m_pGraphicDev, 0);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"CPlayerEyeTrail", pGameObject);

	//Monster - BloodLord Create
	pBloodLord = CBloodLord::Create(m_pGraphicDev);
	if(NULL == pBloodLord)		return E_FAIL;
	pLayer->Ready_Object(L"BloodLord", pBloodLord);
	((CBloodLord*)pBloodLord)->Set_Layer(pLayer);

	for(int i = 0; i < 6; ++i)
	{
		pGameObject = CBLTrail::Create(m_pGraphicDev, i/*위치 인덱스*/);
		if(NULL == pGameObject)		return E_FAIL;
		((CBLTrail*)pGameObject)->Set_BloodLord(pBloodLord);
		pLayer->Ready_Object2(L"BLTrail", pGameObject);
	}

 	return S_OK;
}

HRESULT CStageScene::Ready_Effect(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);

	CEffectMgr::GetInstance()->Initialize(m_pGraphicDev, pLayer);

	m_mapLayer.insert(MAPLAYER::value_type(L"Effect", pLayer));

	return S_OK;
}

void CStageScene::Load_EnvEffect()
{
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(78.f, 10.f, 95.f), true);		//큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire",  _vec3(78.f, 10.f, 95.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f, 1.5f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(78.f, 6.f, 102.f), true);		//큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(78.f, 6.f, 102.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.3f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(78.f, 27.f, 108.f), true);		//큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(78.f, 27.f, 108.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(77.f, 27.f, 95.f), true);		//큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(77.f, 27.f, 95.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.1f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(77.f, 29.f, 121.f), true);		//큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(77.f, 11.f, 121.f), true);		//큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(77.f, 11.f, 121.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.2f);						// 정문 왼쪽 

	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(275.f, 0.f, 271.f), true);		//큰불
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(275.f, 0.f, 271.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.4f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(75.f, 37.f, 111.f), true);		//큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(222.f, 12.f, 109.f), true);		//집 - 큰불
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(75.f, 37.f, 111.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.2f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(222.f, 12.f, 109.f), true);		//집 - 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(218.f, 12.f, 109.f), true);		//집 - 큰불
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(211.f, 19.4f, 62.1f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.0f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(218.f, 19.f, 111.f), true);		//집 - 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(211.f, 19.4f, 62.1f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.3f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(221.f, 21.f, 111.f), true);		//집 - 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(221.f, 21.f, 111.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.7f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(223.f, 20.8f, 111.f), true);		//집 - 큰불
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(223.f, 20.8f, 111.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(231.f, 21.f, 111.f), true);		//집 - 큰불
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(231.f, 21.f, 111.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.3f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(234.f, 21.9f, 110.f), true);		//집 - 큰불
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(234.f, 21.9f, 110.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(241.f, 34.f, 95.f), true);		//집 연기불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(232.f, 33.9f, 95.f), true);		//집 연기불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(232.f, 33.9f, 95.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.5f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(217.f, 33.f, 95.f), true);		//집 연기불
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(217.f, 33.f, 95.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.3f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(228.f, 20.f, 109.f), true);		//집 연기불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(238.f, 12.f, 109.f), true);		//집 연기불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(238.f, 12.f, 109.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.2f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(322.f, 45.8f, 117.f), true);		//집 연기불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(322.f, 45.8f, 117.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.8f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(319.f, 50.f, 152.f), true);		//집 연기불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(307.f, 35.9f, 119.f), true);		//집 큰불//
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(307.f, 35.9f, 119.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.7f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(311.f, 44.f, 102.f), true);		//집 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(311.f, 44.f, 102.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.5f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(71.f, 3.f, 229.f), true);		//바닥 언덕 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(71.f, 3.f, 229.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.2f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(67.f, 0.f, 186.f), true);		//나무 밑 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(67.f, 0.f, 186.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.8f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(64.f, 2.6f, 187.f), true);		//나무 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(64.f, 2.6f, 187.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.7f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(63.f, 4.6f, 188.f), true);		//나무 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(63.f, 4.6f, 188.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,4.f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(65.f, 0.f, 145.f), true);		//나무 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(65.f, 3.9f, 152.f), true);		//나무 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(65.f, 3.9f, 152.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(62.f, 9.f, 153.f), true);		//나무 큰불
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(62.f, 9.f, 153.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.4f);						// 정문 왼쪽 

	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(216.477f, 10.5947f, 297.625f), true);	//큰촛불	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(197.384f, 10.5949f, 313.836f), true);	//큰촛불	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(167.991f, 10.5949f, 313.765f), true);	//큰 촛불	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(148.678f, 10.5952f, 297.641f), true);	//큰 촛불	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(182.189f, 10.2945f, 345.224f), true);	//큰 촛불
		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(156.695f, 7.2f, 302.743f), true);	//화로
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp2", _vec3(159.f, 7.0f, 307.0f), true, true, _vec3(2.f, 10.f, 2.f), _vec3(0.f, -45.f, 0.f));
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(163.499f, 7.2f, 309.011f), true);	//화로
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp2", _vec3(158.f, 3.0f, 305.5f), true, true, _vec3(10.f, 2.f, 2.f), _vec3(0.f, -45.f, 0.f));
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(201.693f, 7.2f, 308.865f), true);	//화로	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp2", _vec3(208.0f, 3.0f, 305.5f), true, true, _vec3(13.f, 2.f, 2.f), _vec3(0.f, 45.f, 0.f));
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(209.268f, 7.2f, 302.288f), true);	//화로	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp2", _vec3(208.0f, 8.0f, 306.0f), true, true, _vec3(2.f, 13.f, 2.f), _vec3(0.f, 45.f, 0.f));

	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(81.5f, 2.5f, 235.f), true);	// 벽 불
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(81.f, 7.f, 250.f), true);	// 벽 불	
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire",_vec3(81.f, 7.f, 250.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.7f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(81.f, 6.6f, 249.f), true);	// 벽 불	
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(81.f, 6.6f, 249.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.9f);						// 정문 왼쪽 
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(81.f, 7.7f, 260.f), true);	// 벽 불	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(81.f, 8.f, 264.f), true);	// 벽 불	
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(81.f, 8.f, 264.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.f);						// 정문 왼쪽 

	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(193.885f, 22.9295f, 111.985f), true);		// 가로등 화로
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(173.015f, 22.9157f, 113.987f), true);		// 가로등 화로
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(301.828f, 22.9225f, 232.826f), true);		// 가로등 화로

	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(79.7f, 10.3f, 255.f), 0.7f); // 벽
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(223.f, 20.f, 123.f), 0.7f);	// 집 지붕
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(79.1f, 30.1f, 107.f), 0.7f);	// 지붕없는집 1
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(77.3f, 35.5f, 116.f), 0.7f);	// 지붕없는집 2
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(65.6f, 37.f, 115.f), 0.7f);	// 지붕없는집 3
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(63.2f, 15.2f, 231.f), 0.7f);	// 나무1
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(64.6f, 9.17f, 186.f), 0.7f);	// 나무2
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(65.2f, 8.18f, 152.f), 0.7f);	// 나무3
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(311.f, 30.8f, 120.f), 0.7f);	// 불2개있는 지붕

	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(156.695f, 7.2f, 302.743f), 0.5f);	// 화로
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(163.499f, 7.2f, 309.011f), 0.5f);	// 화로
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(201.693f, 7.2f, 308.865f), 0.5f);	// 화로
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(209.268f, 7.2f, 302.288f), 0.5f);	// 화로
}

_ulong CStageScene::Release(void)
{
	_ulong dwRefCnt = Engine::CScene::Release();

	if(0 == dwRefCnt)
	{
		printf("CStageScene Delete\n");
		//ㅊㅂㅇ 동적할당했던 이름들 삭제
		size_t iSize = vecObjName.size();
		for(size_t i = 0; i < iSize; ++i)
			Engine::Safe_Delete(vecObjName[i]);
		vecObjName.clear();
		vector<TCHAR*> vecTemp;
		vecObjName.swap(vecTemp);

		Engine::Release_Light();
		Engine::Reset_Resource(RESOURCE_STAGE);

		delete this;
	}
	return dwRefCnt;
}