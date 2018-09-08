#include "stdafx.h"
#include "StageScene.h"
#include "Terrain.h"
#include "TombStone.h"
#include "Player.h"
#include "Sword.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "Export_Function.h"
#include "SkyDome.h"
#include "Shield.h"
#include "Sea.h"
#include "Trail.h"

#include "StaticObj.h"
#include "Boss.h"
#include "UIImage.h"
#include "HealthyBar.h"
#include "HealthPoint.h"
#include "BossSword.h"
#include "FireBall.h"
#include "IceAge.h"
#include "IceThone.h"
#include "Blood.h"
#include "BossTrail.h"
#include "BossTrail2.h"
#include "GroundHit.h"
#include "MonsterHP.h"
#include "MonsterBar.h"
#include "SkillWnd.h"

#include "SDevice.h"
#include "SkillBar.h"
#include "BossFace.h"

CStageScene::CStageScene(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_fTime(0.f)
, m_dwCount(0)
, m_pNaviMesh(Engine::Get_NaviMesh())
, m_pFrustum(Engine::Get_Frustum())
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
}

CStageScene::~CStageScene(void)
{

}

HRESULT CStageScene::Init_Scene(void)
{
	if(FAILED(Ready_Lighting()))
		return E_FAIL;

	if(FAILED(Ready_Environment()))
		return E_FAIL;

	if(FAILED(Ready_GameLogic()))
		return E_FAIL;


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pFrustum->InitFrustum(m_pGraphicDev);

	CSDevice::GetInstance()->SoundPlay(SD_BACKGROUND, 1);

	return S_OK;
}

_int CStageScene::Update(const _float& fTimeDelta)
{
	m_pFrustum->Transform_Frustum_ToWorld(m_pGraphicDev);
	Engine::CScene::Update(fTimeDelta);

	++m_dwCount;
	m_fTime += fTimeDelta;

	if(m_fTime >= 1.f)
	{	
		wsprintf(m_szFps, L"FPS : %d", m_dwCount);
		m_dwCount = 0;
		m_fTime = 0.f;
	}

	return 0;
}

void CStageScene::Render(void)
{
	
	Engine::Render_Font(L"Font_Default", _vec3(10.f, 10.f, 0.f), m_szFps
		, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#ifdef _DEBUG
	
	m_pNaviMesh->Render_NaviMesh();
#endif
}

CStageScene* CStageScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageScene*	pScene = new CStageScene(pGraphicDev);

	if(FAILED(pScene->Init_Scene()))
	{
		MSG_BOX("pScene Initialized Failed");
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
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Direction = _vec3(0.f, -0.4f, 1.f); // In WorldSpace

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CStageScene::Ready_Environment(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CSkyDome::Create(m_pGraphicDev);
	if(NULL == pGameObject) 	return E_FAIL;
	pLayer->Ready_Object(L"SkyDome", pGameObject);

	pGameObject = CSea::Create(m_pGraphicDev);
	if(NULL == pGameObject) 	return E_FAIL;
	pLayer->Ready_Object(L"Sea", pGameObject);

	
	m_mapLayer.insert(MAPLAYER::value_type(L"Environment", pLayer));	

	return S_OK;
}

HRESULT CStageScene::Ready_GameLogic(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);

	Engine::CGameObject*		pGameObject	= NULL;
	Engine::CGameObject*		pPlayer		= NULL;
	Engine::CGameObject*		pBoss		= NULL;

	

	// For.Terrain-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pGameObject = CTerrain::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Terrain", pGameObject);

	// For.Player-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pPlayer = CPlayer::Create(m_pGraphicDev);
	if(NULL == pPlayer)
		return E_FAIL;
	pLayer->Ready_Object(L"Player", pPlayer);

	//// For.Sword-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pGameObject = CSword::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Sword", pGameObject);
	//검광메쉬를 위한 끝자락
	((CPlayer*)pPlayer)->Set_SwordEndPos(((CSword*)pGameObject)->Get_SwordEndPos());

	pGameObject = CShield::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Shield", pGameObject);


	//// For.Trail-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	pGameObject = CTrail::Create(m_pGraphicDev);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"0Trail", pGameObject);
	((CTrail*)pGameObject)->Set_Player(pPlayer);	



	////// For.Boss-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CBossSword::Create(m_pGraphicDev, L"Mesh_BSword1", 0);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"BossSword", pGameObject);
	((CBossSword*)pGameObject)->Set_Player(pPlayer);

	pGameObject = CBossSword::Create(m_pGraphicDev, L"Mesh_BSword2", 1);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"BossSword", pGameObject);
	((CBossSword*)pGameObject)->Set_Player(pPlayer);

	pGameObject = CBossSword::Create(m_pGraphicDev, L"Mesh_BSword3", 2);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"BossSword", pGameObject);
	((CBossSword*)pGameObject)->Set_Player(pPlayer);

	pGameObject = CBossSword::Create(m_pGraphicDev, L"Mesh_BSword4", 3);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"BossSword", pGameObject);
	((CBossSword*)pGameObject)->Set_Player(pPlayer);

	pGameObject = CBossSword::Create(m_pGraphicDev, L"Mesh_BSword5", 4);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"BossSword", pGameObject);
	((CBossSword*)pGameObject)->Set_Player(pPlayer);

	pGameObject = CFireBall::Create(m_pGraphicDev);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"FireBall", pGameObject);
	((CFireBall*)pGameObject)->Set_Player(pPlayer);

	
	pGameObject = CIceAge::Create(m_pGraphicDev, 1);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"IceAge", pGameObject);
	((CIceAge*)pGameObject)->Set_Player(pPlayer);

	pGameObject = CIceAge::Create(m_pGraphicDev, 2);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"IceAge", pGameObject);
	((CIceAge*)pGameObject)->Set_Player(pPlayer);

	for(int i = 0; i < 20; ++i)
	{
		pGameObject = CIceThone::Create(m_pGraphicDev, i);
		if(NULL == pGameObject)	return E_FAIL;
		pLayer->Ready_Object(L"IceThone", pGameObject);
		((CIceThone*)pGameObject)->Set_Player(pPlayer);
	}

	pBoss = CBoss::Create(m_pGraphicDev);
	if(NULL == pBoss)
		return E_FAIL;
	pLayer->Ready_Object(L"Boss", pBoss);
	((CBoss*)pBoss)->Set_Player(pPlayer);
	((CBoss*)pBoss)->Set_FireBall(pLayer->Find_ObjectList(L"FireBall")->front());

	((CBoss*)pBoss)->Set_IceAge(pLayer->Find_ObjectList(L"IceAge"));
	((CBoss*)pBoss)->Set_IceThone(pLayer->Find_ObjectList(L"IceThone"));
	((CBoss*)pBoss)->Set_SwordList(pLayer->Find_ObjectList(L"BossSword"));

	((CBoss*)pBoss)->Set_Layer(pLayer);
	((CPlayer*)pPlayer)->Set_Boss(pBoss);
	((CPlayer*)pPlayer)->Set_Layer(pLayer);
	

	pGameObject = CBossTrail::Create(m_pGraphicDev);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"0Trail", pGameObject);
	((CBossTrail*)pGameObject)->Set_Boss(pBoss);	

	pGameObject = CBossTrail2::Create(m_pGraphicDev);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"0Trail", pGameObject);
	((CBossTrail2*)pGameObject)->Set_Boss(pBoss);


	pGameObject = CGroundHit::Create(m_pGraphicDev);
	if(NULL == pGameObject)	return E_FAIL;
	pLayer->Ready_Object(L"GroundHit", pGameObject);
	((CPlayer*)pPlayer)->Set_GroundHit(pGameObject);


	// For.ui-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pGameObject = CUIImage::Create(m_pGraphicDev);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);

	pGameObject = CHealthyBar::Create(m_pGraphicDev);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);

	pGameObject = CHealthPoint::Create(m_pGraphicDev);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);

	pGameObject = CMonsterBar::Create(m_pGraphicDev);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);
	((CMonsterBar*)pGameObject)->SetBoss(pBoss);

	pGameObject = CMonsterHP::Create(m_pGraphicDev);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);
	((CMonsterHP*)pGameObject)->SetBoss(pBoss);

	pGameObject = CSkillBar::Create(m_pGraphicDev);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);

	pGameObject = CBossFace::Create(m_pGraphicDev);
	if(NULL == pGameObject)		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);
	((CBossFace*)pGameObject)->SetBoss(pBoss);

	for(int i = 0; i < 4; ++i)
	{
		pGameObject = CSkillWnd::Create(m_pGraphicDev, i);
		if(NULL == pGameObject)		return E_FAIL;
		pLayer->Ready_Object(L"UI", pGameObject);
	}
	


	// For.StaticCam-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pGameObject = CStaticCamera::Create(m_pGraphicDev, pPlayer);
	if(NULL == pGameObject)	return E_FAIL;
	((CStaticCamera*)pGameObject)->SetBoss(pBoss);
	pLayer->Ready_Object(L"StaticCamera", pGameObject);

	// For.DynamicCam-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 5.f, -5.f), &_vec3(0.f, 0.f, 0.f));
	if(NULL == pGameObject)		return E_FAIL;
	((CDynamicCamera*)pGameObject)->SetTarget(pBoss, pPlayer);
	pLayer->Ready_Object(L"Camera", pGameObject);


	// For.StaticMesh------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Load_StaticMesh(pLayer);

	// For.TombStone-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*for (_int i = 0; i < 20; ++i)
	{
		pGameObject = CTombStone::Create(m_pGraphicDev);
		if(NULL == pGameObject)
			return E_FAIL;
		pLayer->Ready_Object(L"TombStone", pGameObject);
	}*/


	m_mapLayer.insert(MAPLAYER::value_type(L"GameLogic", pLayer));

	return S_OK;
}

_ulong CStageScene::Release(void)
{
	_ulong dwRefCnt = Engine::CScene::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;	
}

void CStageScene::Load_StaticMesh(Engine::CLayer* pLayer)
{
	HANDLE		hFile = CreateFile(L"../Data/ObjTest.dat", GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD				dwByte;

	vector<Engine::OBJSAVE*>		vecObjLoad;
	int					iCnt = 0;

	while(true)
	{
		Engine::OBJSAVE*	pObjSave = new Engine::OBJSAVE;
		ReadFile(hFile,  pObjSave, sizeof(Engine::OBJSAVE), &dwByte, NULL);

		if(dwByte == 0)
		{
			Engine::Safe_Delete(pObjSave);
			break;
		}

		Engine::CGameObject* pMeshObj = CStaticObj::Create(Engine::Get_GraphicDev(), pObjSave->szName);

		((CStaticObj*)pMeshObj)->SetPos(pObjSave->vPos);
		((CStaticObj*)pMeshObj)->SetScale(pObjSave->vScale);
		((CStaticObj*)pMeshObj)->SetRotation(D3DXVECTOR3(pObjSave->fAngle[0], pObjSave->fAngle[1], pObjSave->fAngle[2]));

		//m_pObjList->push_back(pMeshObj);	
		pLayer->Ready_Object(L"StaticObj", pMeshObj);

		Engine::Safe_Delete(pObjSave);
	}
	CloseHandle(hFile);

}
