#include "stdafx.h"
#include "MiddleScene.h"

#include "Export_Function.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

#include "DynamicCamera.h"
#include "SkyBox.h"
#include "PlayerState.h"
#include "PlayerSkill.h"
#include "ComboUI.h"
#include "DamageFont.h"
#include "Terrain.h"
#include "Player.h"
#include "Sword.h"
#include "PlayerSwordTrail.h"
#include "Glitter.h"
#include "Moonchopper.h"
#include "RoyalGuard.h"
#include "BloodJelly.h"

#include "Loading.h"
#include "SceneSelector.h"

#include "PlayerHeartBeatUI.h"
#include "FadeInOut.h"

#include "MCTrail.h"
#include "Cursor.h"

#include "PlayerBeShotUI.h"
#include "PlayerEyeTrail.h"
#include "EquipmentUI.h"
#include "SkillFailFont.h"

CMiddleScene::CMiddleScene(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_bLoad(false)
{
}

CMiddleScene::~CMiddleScene(void)
{
	Engine::CNaviMesh::GetInstance()->DestroyInstance();//������
	Engine::CCameraNaviMesh::GetInstance()->DestroyInstance();
	Engine::CPhysxSDK::GetInstance()->DestroyInstance();

	CCollisionMgr::GetInstance()->DestroyInstance();
}

HRESULT CMiddleScene::Init_Scene(void)
{
	//Scene Num
	Engine::CSceneNumMgr::GetInstance()->Set_CurrentScene(3);	//�̵� ��

	//Player Create
	CPlayer::GetInstance()->SetEnvironmentTexCom(NULL);
	CPlayer::GetInstance()->SetNaviCell(NULL);
	CPlayer::GetInstance()->SetInGame(true);
	CPlayer::GetInstance()->SetPos(_vec3(190.f, 0.f, 70.f));
	CPlayer::GetInstance()->SetAngleY(D3DXToRadian(90.f));

	//Camera
	CDynamicCamera::GetInstance()->ControlPointMgrInfoLoad(0,L"../Bin/Resources/Data/Camera/StageOneBoss.dat");
	CDynamicCamera::GetInstance()->SetNaviCell(NULL);

	//PlayerHeartBeatUI
	CPlayerHeartBeatUI::GetInstance();

	//PlayerBeShotUI
	CPlayerBeShotUI::GetInstance();

	//EquipmentUI
	CEquipmentUI::GetInstance();

	CSkillFailFont::GetInstance();

	CFadeInOut::GetInstance()->SetFadeInOut(false, 2.0f);

	CSoundMgr::GetInstance()->PlayBGM(SOUND::BGM_MiddleStage, 0.7f);

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

_int CMiddleScene::Update(const _float& fTimeDelta)
{
	CCollisionMgr::GetInstance()->Update(fTimeDelta);
	Engine::UpdateSimulate(fTimeDelta);
	
	CPlayer::GetInstance()->Update(fTimeDelta);

	CDynamicCamera::GetInstance()->Update(fTimeDelta);


	//UI���� ������Ʈ ���� ���Ѿ� ���ϴ� ȿ���� ���� ����
	CPlayerHeartBeatUI::GetInstance()->Update(fTimeDelta);

	CSoundMgr::GetInstance()->Update(fTimeDelta);		//�ֱ������� ���� ��ġ�� ������Ʈ �Ѵ�.
	
	CEquipmentUI::GetInstance()->Update(fTimeDelta);
	
	CSkillFailFont::GetInstance()->Update(fTimeDelta);

	CPlayerBeShotUI::GetInstance()->Update(fTimeDelta);

	Engine::CScene::Update(fTimeDelta);

	CCursor::GetInstance()->Update(fTimeDelta);

	CFadeInOut::GetInstance()->Update(fTimeDelta);//���� �������� ������Ʈ�ؾ��Ѵ�.

	if(Engine::GetDIKeyState(DIK_9) & 0x80)
	{
		CLoading::m_eLoadingType = CLoading::LOADING_STAGE;
		if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_LOGO))))
		{
			MSG_BOX("Change_Scene(SCENE_LOGO) Failed");
			return E_FAIL;
		}
	}

	if( !m_bLoad && Engine::GetDIMouseState(Engine::CInput::DIM_RBUTTON) & 0x80)
	{
		m_bLoad = true;
		printf("GameLogic0\n");
		Ready_GameLogic_0();
		return 0;
	}
	return 0;
}

void CMiddleScene::Render(void)
{

}

CMiddleScene* CMiddleScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiddleScene*	pScene = new CMiddleScene(pGraphicDev);

	if(FAILED(pScene->Init_Scene()))
	{
		MSG_BOX("CMiddleScene Initialized Failed");
		::Safe_Release(pScene);
	}
	return pScene;
}

HRESULT CMiddleScene::Ready_Lighting(void)
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

#pragma region ������ �غ�
	HANDLE   hFile = CreateFile(L"../Bin/Resources/Data/Map/PointLight_BloodLord_2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

HRESULT CMiddleScene::Ready_Environment(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	m_mapLayer.insert(MAPLAYER::value_type(L"Environment", pLayer));

	Engine::CGameObject*		pGameObject = NULL;

	//SkyBox ������
	pGameObject = CSkyBox::Create(m_pGraphicDev, 0, RESOURCE_STAGEMIDDLE);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Sky", pGameObject);

	// Player HP/ SP / Stamina
	pGameObject = CPlayerState::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"PlayerState", pGameObject);

	// Player_Skill_List
	pGameObject = CPlayerSkill::Create(m_pGraphicDev, 1);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"PlayerSkill", pGameObject);
	CPlayer::GetInstance()->m_pPlayerSkill = (CPlayerSkill*)pGameObject;

	// Player_Combo
	pGameObject = CComboUI::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"ComboUI", pGameObject);

	CDamageFont::m_pLayer = pLayer;

	return S_OK;
}

HRESULT CMiddleScene::Ready_GameLogic_0( void )
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	m_mapLayer.insert(MAPLAYER::value_type(L"GameLogic0", pLayer));
	Engine::CGameObject*		pGameObject = NULL;	

	//===========================================================================
	pGameObject = CBloodJelly::Create(m_pGraphicDev, _vec3(180.f, 0.f, 180.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"BloodJelly", pGameObject);
	pGameObject = CBloodJelly::Create(m_pGraphicDev, _vec3(230.f, 0.f, 200.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"BloodJelly", pGameObject);
	pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(170.f, 0.f, 170.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"RoyalGuard", pGameObject);
	pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(210.f, 0.f, 210.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"RoyalGuard", pGameObject);
	//===========================================================================

	//===========================================================================
	pGameObject = CBloodJelly::Create(m_pGraphicDev, _vec3(170.f, 0.f, 160.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"BloodJelly", pGameObject);
	pGameObject = CBloodJelly::Create(m_pGraphicDev, _vec3(220.f, 0.f, 180.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"BloodJelly", pGameObject);
	pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(160.f, 0.f, 150.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"RoyalGuard", pGameObject);
	pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(200.f, 0.f, 190.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"RoyalGuard", pGameObject);
	//===========================================================================

	//===========================================================================
	pGameObject = CBloodJelly::Create(m_pGraphicDev, _vec3(190.f, 0.f, 120.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"BloodJelly", pGameObject);
	pGameObject = CBloodJelly::Create(m_pGraphicDev, _vec3(240.f, 0.f, 150.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"BloodJelly", pGameObject);
	pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(180.f, 0.f, 110.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"RoyalGuard", pGameObject);
	pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(220.f, 0.f, 150.f));
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"RoyalGuard", pGameObject);
	//===========================================================================

	return S_OK;
}

HRESULT CMiddleScene::Ready_GameLogic(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	m_mapLayer.insert(MAPLAYER::value_type(L"GameLogic", pLayer));

	Engine::CGameObject*		pGameObject = NULL;	
	Engine::CGameObject*		pMoonChopper = NULL;	

	//Terrain Create
	pGameObject = CTerrain::Create(m_pGraphicDev, RESOURCE_STAGEMIDDLE);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"Terrain",pGameObject);

#pragma region ������Ʈ �ε�
	//StaticObject
	{
		HANDLE	hFile = CreateFile(L"../Bin/Resources/Data/Map/StaticMesh_BloodLord_2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD		dwByte;

		while(true)
		{
			Engine::OBJINFO	CreateObjectInfo;

			ReadFile(hFile, &CreateObjectInfo, sizeof(Engine::OBJINFO), &dwByte, NULL);

			if(dwByte == 0)
				break;


			if(0 == CreateObjectInfo.iOption) //Default StaticObject ����
			{
				pGameObject = CStaticObject::Create(m_pGraphicDev, CreateObjectInfo.pName, CStaticObject::SOT_Default);
				if(NULL == pGameObject)
					return E_FAIL;
			}
			else if(1 == CreateObjectInfo.iOption) // Destroy StaticObject ����
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
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"PlayerSwordTrail", pGameObject);

	//Player Sword Trail
	pGameObject = CPlayerEyeTrail::Create(m_pGraphicDev, 0);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"CPlayerEyeTrail", pGameObject);

	// Monster - Moonchopper Create
 	pMoonChopper = CMoonchopper::Create(m_pGraphicDev);
 	if(NULL == pMoonChopper) 		return E_FAIL;
 	pLayer->Ready_Object(L"Moonchopper", pMoonChopper);

	pGameObject = CGlitter::Create(m_pGraphicDev);
	if(NULL == pGameObject ) return E_FAIL;
 	pLayer->Ready_Object(L"Glitter", pGameObject);


	for(int i = 0; i < 2; ++i)
	{
		pGameObject = CMCTrail::Create(m_pGraphicDev, i/*��ġ �ε���*/);
		if(NULL == pGameObject)		return E_FAIL;
		((CMCTrail*)pGameObject)->Set_MoonChop(pMoonChopper);
		pLayer->Ready_Object2(L"MCTrail", pGameObject);
	}	


	return S_OK;
}

HRESULT CMiddleScene::Ready_Effect(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);

	CEffectMgr::GetInstance()->Initialize(m_pGraphicDev, pLayer);

	m_mapLayer.insert(MAPLAYER::value_type(L"Effect", pLayer));

	return S_OK;
}

void CMiddleScene::Load_EnvEffect()
{
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(157.f, 0.f, 72.f), true);						// ������ ������ 		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(154.f, 0.f, 74.f), true);						// ������ �߰��� 		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(154.f, 2.f, 74.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.2f);						// �ҿ���
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(150.f, 0.f, 80.4f), true);						// ������ ū�� 		
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(150.f, 1.f, 80.4f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// ������ ū�� 		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(112.f, 2.7f, 92.f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(112.f, 2.7f, 92.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f, 1.5f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(114.f, 8.19f, 97.1f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(114.f, 8.19f, 97.1f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f, 1.2f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(111.f, 3.f, 98.3f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(111.f, 3.f, 98.3f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.7f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(107.f, 2.5f, 102.f), true);						// ���� ū��			
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(107.f, 2.5f, 102.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f, 2.1f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(108.f, 6.2f, 106.f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(108.f, 6.2f, 106.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f, 0.4f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(108.f, 0.f, 116.f), true);						// ���� ū��		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(108.f, 1.5f, 116.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.2f);						// ���� ū��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(117.f, 0.f, 90.3f), true);						// ���� ū��
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(117.f, 0.f, 90.3f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.5f);						// ���� ū��
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(118.f, 9.03f, 98.5f), true);						// ���� ū��
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(118.f, 9.03f, 98.5f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.3f);						// ���� ū��
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(123.f, 41.3f, 77.7f), true);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(123.f, 41.3f, 77.7f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.1f);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(117.f, 43.f, 73.6f), true);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(117.f, 43.f, 73.6f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.5f);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(115.f, 41.8f, 75.9f), true);						// ����				
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(115.f, 41.8f, 75.9f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,4.2f);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(111.f, 43.7f, 73.9f), true);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(111.f, 43.7f, 73.9f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.2f);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(125.f, 2.69f, 214.f), true);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(125.f, 2.69f, 214.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.3f);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(125.f, 15.6f, 219.f), true);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(125.f, 15.6f, 219.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.2f);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(126.f, 10.9f, 220.f), true);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(126.f, 10.9f, 220.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.7f);						// ����				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(115.f, 0.f, 228.f), true);						// ���� �� �� ū�� 	
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(115.f, 3.f, 228.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.4f);						// ���� �� �� ū�� 	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(118.f, 0.f, 231.f), true);						// ���� �� �� ū�� 	
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(118.f, -2.f, 231.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// ���� �� �� ū�� 	
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(121.f, 0.f, 233.f), true);						// ���� �� �� �߰���	
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(121.f, 0.f, 233.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.4f);						// ���� �� �� �߰���	

	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(180.343f, 6.42f, 254.26f), true);					// ȭ�Ժ� 				
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(203.512f, 6.41f, 253.451f), true);					// ȭ�Ժ� 		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(118.16f, 25.7f, 188.54f), true);					// �������		

	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(139.982f, 53.1f, 241.066f), true);			// �� õ�� �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(192.33f, 53.1f, 255.59f), true);			// �� õ�� �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_BestSmall", _vec3(246.655f, 53.02f, 240.089f), true);			// �� õ�� �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(261.f, 0.f, 216.f), true);						// ���� ������			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(263.f, 0.f, 213.f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(263.f, 0.f, 213.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.0f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(263.f, 4.9f, 216.f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(263.f, 4.9f, 216.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.5f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(258.f, 0.f, 199.f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(258.f, 0.f, 199.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.1f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(257.f, 5.4f, 196.f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(257.f, 5.4f, 196.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.2f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(260.f, 6.42f, 198.f), true);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(260.f, 6.42f, 198.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// ���� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Mid2", _vec3(266.f, 2.5f, 189.f), true);						// ��Ÿ�� �߰���		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(263.f, 0.f, 189.f), true);						// ��Ÿ�� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(263.f, 0.f, 189.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.3f);						// ��Ÿ�� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(261.f, 4.14f, 188.f), true);						// ��Ÿ�� ū��			
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(261.f, 4.14f, 188.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// ��Ÿ�� ū��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Camp", _vec3(258.f, 3.37f, 188.f), true);						// ��Ÿ�� ������		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(266.f, 7.18f, 173.f), true);						// ��Ÿ���� ����		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(266.f, 7.18f, 173.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.4f);						// ��Ÿ���� ����		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(265.f, 0.f, 175.f), true);						// ��Ÿ���� ����		
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(265.f, -3.f, 175.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.0f);						// ��Ÿ���� ����		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(271.f, 2.f, 167.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(271.f, 2.f, 167.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.8f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(268.f, 1.f, 167.f), true);						// ��Ÿ���� ��			
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(268.f, 1.f, 167.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.3f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(271.f, 0.f, 167.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(271.f, -1.f, 167.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.5f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(266.f, 0.f, 166.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(266.f, 3.f, 166.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.1f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(268.f, 3.88f, 166.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(268.f, 3.88f, 166.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.2f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(270.f, 10.4f, 166.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(270.f, 10.4f, 166.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.2f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(259.f, 16.6f, 169.f), true);						// ��Ÿ���� ��			
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(259.f, 16.6f, 169.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.3f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(267.f, 19.1f, 168.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(267.f, 19.1f, 168.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(267.f, 29.4f, 115.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(267.f, 29.4f, 115.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.1f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(268.f, 28.4f, 109.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(268.f, 28.4f, 109.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.4f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(265.f, 35.8f, 105.f), true);						// ��Ÿ���� ��			
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(265.f, 35.8f, 105.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.2f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(267.f, 31.1f, 98.6f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(267.f, 31.1f, 98.6f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.8f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(260.f, 38.2f, 102.f), true);						// ��Ÿ���� ��			
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(260.f, 38.2f, 102.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.1f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(257.f, 33.7f, 103.f), true);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(257.f, 33.7f, 103.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.7f);						// ��Ÿ���� ��			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(257.f, 37.4f, 98.6f), true);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(257.f, 37.4f, 98.6f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.8f);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(245.f, 0.f, 66.f), true);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(245.f, -1.f, 66.f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.3f);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(244.f, 0.f, 60.9f), true);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(244.f, 2.f, 60.9f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.5f);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(257.f, 37.4f, 98.6f), true);						// ��Ÿ������ ��		
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(257.f, 37.4f, 98.6f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.8f);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(245.f, 30.7f, 72.5f), true);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(245.f, 30.7f, 72.5f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.4f);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(245.f, 30.7f, 68.5f), true);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(245.f, 30.7f, 68.5f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,2.3f);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(246.f, 36.8f, 66.4f), true);						// ��Ÿ������ ��		
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(246.f, 36.8f, 66.4f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.1f);						// ��Ÿ������ ��		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(220.f, 0.f, 59.4f), true);						// ���� ���� 			
	//CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(220.f, 3.f, 59.4f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,1.6f);						// ���� ���� 			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(214.f, 0.f, 63.9f), true);						// ���� ���� 			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(214.f, 1.f, 63.9f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,3.2f);						// ���� ���� 			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(212.f, 6.79f, 65.8f), true);						// ���� ���� 			
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(212.f, 6.79f, 65.8f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.2f);						// ���� ���� 			
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Big2", _vec3(211.f, 19.4f, 62.1f), true);						// ���� ���� 
	CEffectMgr::GetInstance()->Play_Effect(L"Dust_Fire", _vec3(211.f, 19.4f, 62.1f), true, true, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, false, 0.f,0.f);						// ���� ���� 

	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.073f, 2.39f, 241.709f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.073f, 2.39f, 241.709f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.788f, 1.00f, 234.415f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.306f, 2.51f, 230.848f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(173.985f, 2.38f, 222.552f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.618f, 1.00f, 215.277f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.221f, 2.50f, 211.691f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.057f, 2.38f, 202.396f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.737f, 1.00f, 195.118f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.243f, 2.51f, 191.537f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.012f, 2.39f, 182.63f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.719f, 1.00f, 175.371f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(174.213f, 2.51f, 171.775f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.192f, 2.38f, 241.722f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.82f, 1.f, 234.483f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.396f, 2.50f, 230.858f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.164f, 2.38f, 222.237f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.823f, 1.01f, 214.952f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.382f, 2.51f, 211.385f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.206f, 2.38f, 202.658f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.857f, 1.01f, 195.375f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.399f, 2.50f, 191.792f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.262f, 2.38f, 182.655f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.902f, 1.01f, 175.372f), true);				// �к�		
	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Small2", _vec3(209.471f, 2.51f, 171.802f), true);				// �к�		

	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(151.f, 0.0f, 75.f), 0.7f); 
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(113.f, 0.0f, 101.f), 0.7f);	
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(128.f, 0.0f, 215.f), 0.7f);	
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(262.f, 0.0f, 216.f), 0.7f);	
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(257.f, 0.0f, 198.f), 0.7f);	
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(266.f, 0.0f, 170.f), 0.7f);	
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(257.f, 37.0f, 98.f), 0.7f);	
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(246.f, 0.0f, 62.f), 0.7f);
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(180.f, 0.0f, 255.f), 0.7f);	
	CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, _vec3(203.f, 0.0f, 253.f), 0.7f);

	//CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin", _vec3(230.471f, 1.f, 171.802f), true, true/*, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f),0.f,false,0.f, NULL, false, 0.f*/);			//
	//CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin2", _vec3(230.471f, 1.f, 171.802f), true, false, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f),0.f,false,0.f, NULL, true);			//
	//CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing", _vec3(230.471f, 1.f, 171.802f), true, true);			//

	//CEffectMgr::GetInstance()->Play_Effect(L"DustBig", _vec3(230.471f, 1.f, 171.802f), true, true);			//
}

_ulong CMiddleScene::Release(void)
{
	_ulong dwRefCnt = Engine::CScene::Release();

	if(0 == dwRefCnt)
	{
		CSoundMgr::GetInstance()->StopBGM();
		CSoundMgr::GetInstance()->EnvironmentStopEFF();

		printf("CMiddleScene Delete\n");
		//������ �����Ҵ��ߴ� �̸��� ����
		size_t iSize = vecObjName.size();
		for(size_t i = 0; i < iSize; ++i)
			Engine::Safe_Delete(vecObjName[i]);
		vecObjName.clear();
		vector<TCHAR*> vecTemp;
		vecObjName.swap(vecTemp);

		Engine::Release_Light();
		Engine::Reset_Resource(RESOURCE_STAGEMIDDLE);

		delete this;
	}
	return dwRefCnt;
}
