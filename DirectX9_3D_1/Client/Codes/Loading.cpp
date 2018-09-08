#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include <process.h>

#include "StageScene.h"
#include "MiddleScene.h"
#include "EffectMgr.h"

#include "Player.h"

vector<TCHAR*> CStageScene::vecObjName;
vector<TCHAR*> CMiddleScene::vecObjName;

CLoading::LOADINGTYPE CLoading::m_eLoadingType;
_int CLoading::m_iLoadingNumber = 0;
CLoading::CLoading(LOADINGTYPE eLoadingType)
: m_LoadType(eLoadingType)
, m_bComplete(false)
, m_pGraphicDev(NULL)
{
	ZeroMemory(m_szLoadMessage, sizeof(TCHAR) * 128);
}

CLoading::~CLoading(void)
{

}

CLoading::LOADINGTYPE CLoading::GetLoadType(void){	return m_LoadType; }
CRITICAL_SECTION* CLoading::GetCSKey(void){	return &m_CSKey; }
bool CLoading::GetComplete(void){ return m_bComplete; }
const TCHAR* CLoading::GetLoadingMessage(void){	return m_szLoadMessage;}
void CLoading::InitLoading(void)
{
	m_pGraphicDev = Engine::Get_GraphicDev();

	InitializeCriticalSection(&m_CSKey);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
}


void CLoading::DyeSceneLoading(void)
{
	//Scene Num
	//Engine::CSceneNumMgr::GetInstance()->Set_CurrentScene(0);	// Loading

	HRESULT		hr = NULL;
	lstrcpy(m_szLoadMessage, L"DyeSceneLoading...");

	m_iLoadingNumber = 0;
	//Static
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_RcTex")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return;
	}	
	m_iLoadingNumber = 1;
#pragma region 허크 로딩
	lstrcpy(m_szLoadMessage, L"Hurk Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Player", L"../Bin/Resources/Mesh/DynamicMesh/Hurk/", L"Hurk.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	m_iLoadingNumber = 2;
	lstrcpy(m_szLoadMessage, L"Skeleton Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Skeleton_Hair", L"../Bin/Resources/Mesh/DynamicMesh/Look/Skeleton/", L"Skeleton_Head.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Skeleton_Upper", L"../Bin/Resources/Mesh/DynamicMesh/Look/Skeleton/", L"Skeleton_Upper.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Skeleton_Lower", L"../Bin/Resources/Mesh/DynamicMesh/Look/Skeleton/", L"Skeleton_Lower.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Skeleton_Hand", L"../Bin/Resources/Mesh/DynamicMesh/Look/Skeleton/", L"Skeleton_Hand.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Skeleton_Foot", L"../Bin/Resources/Mesh/DynamicMesh/Look/Skeleton/", L"Skeleton_Foot.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	m_iLoadingNumber = 3;
	lstrcpy(m_szLoadMessage, L"Heremon Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Heremon_Hair", L"../Bin/Resources/Mesh/DynamicMesh/Look/Heremon/", L"Heremon_Head.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Heremon_Upper", L"../Bin/Resources/Mesh/DynamicMesh/Look/Heremon/", L"Heremon_Upper.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Heremon_Lower", L"../Bin/Resources/Mesh/DynamicMesh/Look/Heremon/", L"Heremon_Lower.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Heremon_Hand", L"../Bin/Resources/Mesh/DynamicMesh/Look/Heremon/", L"Heremon_Hand.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Heremon_Foot", L"../Bin/Resources/Mesh/DynamicMesh/Look/Heremon/", L"Heremon_Foot.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	m_iLoadingNumber = 4;
	lstrcpy(m_szLoadMessage, L"Pirate Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Pirate_Hair", L"../Bin/Resources/Mesh/DynamicMesh/Look/Pirate/", L"Pirate_Head.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Pirate_Upper", L"../Bin/Resources/Mesh/DynamicMesh/Look/Pirate/", L"Pirate_Upper.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Pirate_Lower", L"../Bin/Resources/Mesh/DynamicMesh/Look/Pirate/", L"Pirate_Lower.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Pirate_Hand", L"../Bin/Resources/Mesh/DynamicMesh/Look/Pirate/", L"Pirate_Hand.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_DYE, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Pirate_Foot", L"../Bin/Resources/Mesh/DynamicMesh/Look/Pirate/", L"Pirate_Foot.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
		return;
	}

	//플레이어 검메쉬
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_STATIC, L"Mesh_Sword", L"../Bin/Resources/Mesh/StaticMesh/Night_Sword/", L"Night_Sword.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Sword_1)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_STATIC, L"Mesh_Sword2", L"../Bin/Resources/Mesh/StaticMesh/Skeleton_Sword/", L"Skeleton_Sword.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Sword2)");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_STATIC, L"Mesh_Sword3", L"../Bin/Resources/Mesh/StaticMesh/Heremon_Sword/", L"Sword3.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Sword3)");
		return;
	}
#pragma endregion
	m_iLoadingNumber = 5;
#pragma region 지형 오브젝트 스태틱 메쉬
	lstrcpy(m_szLoadMessage, L"Static Mesh Loading...");
	//ㅊㅂㅇ======================================================
	wstring strFullPath1 = L"../Bin/Resources/Data/Map/StaticMeshList.txt";

	wifstream	LoadFile;
	LoadFile.open(strFullPath1.c_str(), ios::in);

	TCHAR	str[128];
	TCHAR	str2[128];

	ZeroMemory(str, sizeof(TCHAR)*128);
	ZeroMemory(str2, sizeof(TCHAR)*128);

	while(!LoadFile.eof())
	{
		LoadFile.getline(str, 128, '|');
		LoadFile.getline(str2, 128);
		TCHAR szTag[128] = L"Mesh_";
		lstrcat(szTag, str);
		lstrcat(str,L".x");

		TCHAR* strTemp = new TCHAR[128];
		ZeroMemory(strTemp, sizeof(TCHAR)*128);

		lstrcpy(strTemp, szTag);

		if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_STATIC, strTemp, L"../Bin/Resources/Mesh/StaticMesh/BloodLord/", str)))
		{
			MSG_BOX("Ready_Mesh Ready Failed");
			return;
		}

		CStageScene::vecObjName.push_back(strTemp);
	}
	LoadFile.close();
#pragma endregion
	m_iLoadingNumber = 6;
#pragma  region UI 버퍼 로딩
	lstrcpy(m_szLoadMessage, L"UI Buffer Loading...");
	// For.PlayerHP_Front 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_PlayerHPFront")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.PlayerHP_Back 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_PlayerHPBack")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.PlayerSP_Front 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_PlayerSPFront")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.PlayerSP_Back 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_PlayerSPBack")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.PlayerStamina_Front 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_PlayerStaminaFront")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.PlayerStamina_Back 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_PlayerStaminaBack")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_PlayerState 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_PlayerState")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_HeartBeatUI
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_HeartBeatUI")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_EquipmentUI
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_EquipmentUI")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_SkillFailFont")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_Judgement
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_Judgement")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_Charge_Slash
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_Charge_Slash")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_Execution
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_Execution")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_Berserker
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_Berserker")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_Hell
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_Hell")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_JudgementCool 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_JudgementCool")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_Charge_SlashCool 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_Charge_SlashCool")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_ExecutionCool
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_ExecutionCool")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_BerserkerCool 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_BerserkerCool")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_HellCool 
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_HellCool")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_SKILL_SP_FONT
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_SP_SF")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_SKILL_COOL_FONT
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_Cool_SF")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	//Monster
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_BossBar")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_BossBack")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_BossFrontBloodLord")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_BossFrontGlitter")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_BossFrontMoonchopper")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Boss")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_BossFace")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_DamageFont1 ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_DamageFont1")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_DamageFont2 ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_DamageFont2")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_DamageFont3 ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_DamageFont3")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_DamageFont4 ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_DamageFont4")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_DamageFont ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_OverKillFont")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.Buffer_ComboFont ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_ComboFont")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.ComboOneFront ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_ComboOneFront")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}
	// For.ComboOneBack ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_ComboOneBack")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.ComboTenFront ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_ComboTenFront")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	// For.ComboTenFront ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_ComboTenBack")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

	//ㅊㅂㅇ
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Cursor")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}

#pragma endregion
	m_iLoadingNumber = 7;
#pragma region UI 텍스쳐 로딩
	lstrcpy(m_szLoadMessage, L"UI Texture Loading...");
	//ㅊㅂㅇ
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL, L"Texture_Cursor", 
		L"../Bin/Resources/Texture/UIResource/Cursor/Cursor.png", 1)))
	{
		MSG_BOX("Texture_Cursor Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerHP", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/HP%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerHP Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerHPBack", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/HPBack%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerHPBack Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerHPBack2", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/HPBack2%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerHPBack2 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerSP", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/SP%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerSP Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerSPBack", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/SPBack%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerSPBack Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerSPBack2", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/SPBack2%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerSPBack2 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerStamina", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/Stamina%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerStamina_Red Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerStaminaBack", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/StaminaBack%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerStamina_Red Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerStaminaBack2", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/StaminaBack2%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerStamina_Red Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerHeartBeatUI", 
		L"../Bin/Resources/Texture/StageScene/UIResource/PlayerState/PlayerHeartBeatUI%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerHeartBeatUI Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SkillFailFont", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/SkillFailFont%d.png", 2)))
	{
		MSG_BOX("Texture_SkillFailFont Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_EquipmentUI", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/Equipment%d.png", 5)))
	{
		MSG_BOX("Texture_EquipmentUI Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerSkill", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/Player_Skill%d.png", 1)))
	{
		MSG_BOX("Texture_PlayerSkill Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_PlayerSkillCool", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/Player_SkillCool%d.png", 1)))
	{
		MSG_BOX("Texture_PlayerSkillCool Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SkillFont", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/Skill_Fail_Font%d.png", 1)))
	{
		MSG_BOX("Texture_SkillFont Ready Failed");
		return;
	}

	//Monster
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_BossHPBar", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Boss/BossHPBar%d.png", 1)))
	{
		MSG_BOX("Texture_BossHPBar Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_BossHpBack", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Boss/BossHpBack%d.png", 7)))
	{
		MSG_BOX("Texture_BossHpBack Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_BossHPFront", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Boss/BossHPFront%d.png", 7)))
	{
		MSG_BOX("Texture_BossHPFront Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_BossName", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Boss/BossName%d.png", 3)))
	{
		MSG_BOX("Texture_BossName Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_UIMul", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Boss/UIMul%d.png", 7)))
	{
		MSG_BOX("Texture_UIMul Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_BossFace", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Boss/Bosstarget%d.png", 2)))
	{
		MSG_BOX("Texture_BossFace Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DamageFont0", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/MabiDamage%d.png", 1)))
	{
		MSG_BOX("MabiDamage Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_Number_Combine0", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/Number_Combine%d.png", 1)))
	{
		MSG_BOX("Number_Combine Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_OverKill0", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/OverKill%d.png", 1)))
	{
		MSG_BOX("OverKill Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ComboFont0", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Player/ComboFont%d.png", 1)))
	{
		MSG_BOX("Texture_ComboFont Ready Failed");
		return;
	}
#pragma endregion
	m_iLoadingNumber = 8;
#pragma  region 이펙트 로딩
	lstrcpy(m_szLoadMessage, L"Effect Loading...");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blunt_yellow_00", 
		L"../Bin/Resources/Texture/Effect/blunt_yellow_0%d.tga", 1)))
	{
		MSG_BOX("Texture_blunt_yellow_00 Ready Failed");
		return;
	}
printf("0");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_upSmoke_yellow0", 
		L"../Bin/Resources/Texture/Effect/upSmoke_yellow%d.tga", 1)))
	{
		MSG_BOX("upSmoke_yellow0 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_AuraFlip_001_TEX_HKB0", 
		L"../Bin/Resources/Texture/Effect/Smoke/FX_AuraFlip_001_TEX_HKB%d.tga", 1)))
	{
		MSG_BOX("Texture_Smoke Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Disappear_Smoke_ani.dat", L"Disappear_Smoke_ani");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_ani_00", 
		L"../Bin/Resources/Texture/Effect/Blood/blood_ani_0%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Red_Small_ani Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Red_Small_ani.dat", L"Blood_Red_Small_ani");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_ani_20", 
		L"../Bin/Resources/Texture/Effect/Blood/blood_ani_2%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Red_Mid2_ani Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Red_Mid2_ani.dat", L"Blood_Red_Mid2_ani");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_Final_Fire020", 
		L"../Bin/Resources/Texture/Effect/Final_Fire02%d.tga", 1)))
	{
		MSG_BOX("Final_Fire020 Ready Failed");
		return;
	}

	printf("0.1");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_blood_007_TEX_CJH0", 
		L"../Bin/Resources/Texture/Effect/Blood/FX_blood_007_TEX_CJH%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Red_Big2_One Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Red_Big2_One.dat", L"Blood_Red_Big2_One");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_green_ani_00", 
		L"../Bin/Resources/Texture/Effect/Blood/blood_green_ani_0%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Green_Small_ani_One Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Green_Small_ani.dat", L"Texture_Blood_Green_Small_ani");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_green_ani_10", 
		L"../Bin/Resources/Texture/Effect/Blood/blood_green_ani_1%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Green_Mid1_ani Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Green_Mid1_ani.dat", L"Blood_Green_Mid1_ani");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_green_ani_20", 
		L"../Bin/Resources/Texture/Effect/Blood/blood_green_ani_2%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Green_Mid2_ani Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Green_Mid2_ani.dat", L"Blood_Green_Mid2_ani");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_dust_00", 
		L"../Bin/Resources/Texture/Effect/Dust/dust_0%d.tga", 1)))
	{
		MSG_BOX("Texture_Dust_Basic_One Ready Failed");
		return;
	}	
	//CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Test.dat", L"Fire_Test");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_dust_10", 
		L"../Bin/Resources/Texture/Effect/Dust/dust_1%d.tga", 1)))
	{
		MSG_BOX("Texture_Dust_Basic_One Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_dust_20", 
		L"../Bin/Resources/Texture/Effect/Dust/dust_2%d.tga", 1)))
	{
		MSG_BOX("Texture_Dust_Basic_One Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_dust_30", 
		L"../Bin/Resources/Texture/Effect/Dust/dust_3%d.tga", 1)))
	{
		MSG_BOX("Texture_Dust_Basic_One Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_dust_40", 
		L"../Bin/Resources/Texture/Effect/Dust/dust_4%d.tga", 1)))
	{
		MSG_BOX("Texture_Dust_Basic_One Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_dust_60", 
		L"../Bin/Resources/Texture/Effect/Dust/dust_6%d.tga", 1)))
	{
		MSG_BOX("Texture_Dust_Basic_One Ready Failed");
		return;
	}	
printf("0.2");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_dust_70", 
		L"../Bin/Resources/Texture/Effect/Dust/dust_7%d.tga", 1)))
	{
		MSG_BOX("Texture_Dust_Basic_One Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Dust_Basic_One.dat", L"Dust_Basic_One");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_letter0", 
		L"../Bin/Resources/Texture/Effect/etc/letter%d.tga", 1)))
	{
		MSG_BOX("Texture_Berserk_floor_One Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Berserk_floor_One.dat", L"Berserk_floor_One");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_Basic_Spark0", 
		L"../Bin/Resources/Texture/Effect/Spark/Basic_Spark%d.tga", 1)))
	{
		MSG_BOX("Texture_Player_Run_Sword_Spark_ani Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Player_Run_Sword_Spark_ani.dat", L"Player_Run_Sword_Spark_ani");	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_efx_sword_slashpoint0", 
		L"../Bin/Resources/Texture/Effect/Spark/efx_sword_slashpoint%d.tga", 1)))
	{
		MSG_BOX("Texture_efx_sword_slashpoint Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_efx_sword_slash_spin0", 
		L"../Bin/Resources/Texture/Effect/Spark/efx_sword_slash_spin%d.tga", 1)))
	{
		MSG_BOX("Texture_efx_sword_slash_spin0 Ready Failed");
		return;
	}

	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Player_Slash_Effect.dat", L"Player_Slash_Effect");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Player_Slash_Effect_Final.dat", L"Player_Slash_Effect_Final");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_detail_40", 
		L"../Bin/Resources/Texture/Effect/etc/detail_4%d.tga", 1)))
	{
		MSG_BOX("Texture_detail_4 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Dungeon_Door_One.dat", L"Dungeon_Door_One");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_Smoke_8x400", 
		L"../Bin/Resources/Texture/Effect/Smoke/FX_Smoke_8x40%d.tga", 1)))
	{
		MSG_BOX("Texture_FX_Smoke_8x400 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Smoke_Basic.dat", L"Smoke_Basic");
printf("0.3");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_flame_campfire_010", 
		L"../Bin/Resources/Texture/Effect/Fire/flame_campfire_01%d.tga", 1)))
	{
		MSG_BOX("flame_campfire_010 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Camp.dat", L"Fire_Camp");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_fireball_023_1_TEX_KJS0", 
		L"../Bin/Resources/Texture/Effect/Fire/FX_fireball_023_1_TEX_KJS%d.tga", 1)))
	{
		MSG_BOX("flame_campfire_010 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Camp2.dat", L"Fire_Camp2");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_Impact_Up_001_TEX_HKB0", 
		L"../Bin/Resources/Texture/Effect/FX_Impact_Up_001_TEX_HKB%d.tga", 1)))
	{
		MSG_BOX("FX_Impact_Up_001_TEX_HKB0 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_fireball_005_TEX_KJS0", 
		L"../Bin/Resources/Texture/Effect/Fire/FX_fireball_005_TEX_KJS%d.tga", 1)))
	{
		MSG_BOX("FX_fireball_005_TEX_KJS0 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_fireball_007_TEX_KJS0", 
		L"../Bin/Resources/Texture/Effect/Fire/FX_fireball_007_TEX_KJS%d.tga", 1)))
	{
		MSG_BOX("FX_fireball_007_TEX_KJS0 Ready Failed");
		return;
	}
printf("0.4");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_fireball_015_TEX_KJS0", 
		L"../Bin/Resources/Texture/Effect/Fire/FX_fireball_015_TEX_KJS%d.tga", 1)))
	{
		MSG_BOX("FX_fireball_007_TEX_KJS0 Ready Failed");
		return;
	}

	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Test_Light.dat", L"Test_Light");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Mid.dat", L"Fire_Mid");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Mid2.dat", L"Fire_Mid2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Big.dat", L"Fire_Big");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Big2.dat", L"Fire_Big2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_BestSmall.dat", L"Fire_BestSmall");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_stone_050", 
		L"../Bin/Resources/Texture/Effect/Fragment/stone_05%d.tga", 1)))
	{
		MSG_BOX("stone_050 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_stone_020", 
		L"../Bin/Resources/Texture/Effect/Fragment/stone_02%d.tga", 1)))
	{
		MSG_BOX("stone_020 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/GroundAtt_Smoke.dat", L"GroundAtt_Smoke");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fragment1.dat", L"Fragment1");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fragment2.dat", L"Fragment2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fragment3.dat", L"Fragment3");

	m_iLoadingNumber = 9;
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_bat0", 
		L"../Bin/Resources/Texture/Effect/etc/bat%d.tga", 1)))
	{
		MSG_BOX("bat0 Ready Failed");
		return;
	}
printf("0.5");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/bat.dat", L"bat");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Spark_Mid.dat", L"Spark_Mid");

	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blunt_Test_1.dat", L"Blunt1");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blunt_Test_2.dat", L"Blunt2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blunt_Test_3.dat", L"Blunt3");	

	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Sprint_Spark.dat", L"Sprint_Spark");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring_outer_wind02_R0", 
		L"../Bin/Resources/Texture/Effect/Ring/ring_outer_wind02_R%d.tga", 1)))
	{
		MSG_BOX("ring_outer_wind02_R0 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Berserk.dat", L"Berserk");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/UpSmoke.dat", L"UpSmoke");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Rager.dat", L"Rager");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/BigBlunt.dat", L"BigBlunt");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_wind_willy0", 
		L"../Bin/Resources/Texture/Effect/wind_willy%d.tga", 1)))
	{
		MSG_BOX("wind_willy Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/BigWind.dat", L"BigWind");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/MidBlunt.dat", L"MidBlunt");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blunt_Basic.dat", L"Blunt_Basic");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blunt_Final.dat", L"Blunt_Final");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blunt_Final2.dat", L"Blunt_Final2");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blunt_00", 
		L"../Bin/Resources/Texture/Effect/Spark/blunt_0%d.tga", 1)))
	{
		MSG_BOX("blunt_00 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/4th_Smoke.dat", L"4th_Smoke");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/4thSmash_final.dat", L"4thSmash_final");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Small_spark.dat", L"Small_spark");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Dash_Spark.dat", L"Dash_Spark");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring60", 
		L"../Bin/Resources/Texture/Effect/ring/ring6%d.tga", 1)))
	{
		MSG_BOX("ring60 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring_light_010", 
		L"../Bin/Resources/Texture/Effect/ring/ring_light_01%d.tga", 1)))
	{
		MSG_BOX("ring_light_010 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring_outer_wind010", 
		L"../Bin/Resources/Texture/Effect/ring/ring_outer_wind01%d.tga", 1)))
	{
		MSG_BOX("ring_outer_wind010 Ready Failed");
		return;
	}
	//
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_wind010", 
		L"../Bin/Resources/Texture/Effect/wind01%d.tga", 1)))
	{
		MSG_BOX("wind010 Ready Failed");
		return;
	}

	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Wind.dat", L"Wind");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Dash_Smoke.dat", L"Dash_Smoke");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_light0", 
		L"../Bin/Resources/Texture/Effect/Spark/light%d.tga", 1)))
	{
		MSG_BOX("light Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare_s0", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare_s%d.tga", 1)))
	{
		MSG_BOX("yellowflare_s0 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare_ss0", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare_ss%d.tga", 1)))
	{
		MSG_BOX("yellowflare_ss0 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare_white0", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare_white%d.tga", 1)))
	{
		MSG_BOX("yellowflare_white0 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare0", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare%d.tga", 1)))
	{
		MSG_BOX("yellowflare0 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare3_s0", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare3_s%d.tga", 1)))
	{
		MSG_BOX("yellowflare3_s0 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare4_A0", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare4_A%d.tga", 1)))
	{
		MSG_BOX("yellowflare4_A0 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare10", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare1%d.tga", 1)))
	{
		MSG_BOX("yellowflare10 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare20", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare2%d.tga", 1)))
	{
		MSG_BOX("yellowflare20 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare40", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare4%d.tga", 1)))
	{
		MSG_BOX("yellowflare40 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare50", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare5%d.tga", 1)))
	{
		MSG_BOX("yellowflare50 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare60", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare6%d.tga", 1)))
	{
		MSG_BOX("yellowflare60 Rea Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_yellowflare70", 
		L"../Bin/Resources/Texture/Effect/yellowflare/yellowflare7%d.tga", 1)))
	{
		MSG_BOX("yellowflare70 Rea Failed");
		return;
	}
	m_iLoadingNumber = 10;
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/testtest.dat", L"testtest");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Test_Blunt.dat", L"Test_Blunt");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Small2.dat", L"Fire_Small2");
//
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_circle_lightning010", 
		L"../Bin/Resources/Texture/Effect/Lightning/circle_lightning01%d.tga", 1)))	{		MSG_BOX("circle_lightning010 Ready Failed");		return;	}
////////////////////////////////////////////////////////////////////////////
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_efx_sword_sharp_line0", 
		L"../Bin/Resources/Texture/Effect/efx/efx_sword_sharp_line%d.tga", 1)))	{		MSG_BOX("efx_sword_sharp_line Ready Failed");		return;	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_bomb_BW00", 
		L"../Bin/Resources/Texture/Effect/Bomb/bomb_BW0%d.tga", 1)))	{		MSG_BOX("bomb_BW00 Ready Failed");		return;	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Attack2.dat", L"Attack2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Attack3.dat", L"Attack3");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/AttackStrongA.dat", L"AttackStrongA");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_efx_ripple_normal0", 
		L"../Bin/Resources/Texture/Effect/Distortion/efx_ripple_normal%d.tga", 1)))	{		MSG_BOX("efx_ripple_normal0 Ready Failed");		return;	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_efx_ripple1_normal0", 
		L"../Bin/Resources/Texture/Effect/Distortion/efx_ripple1_normal%d.tga", 1)))	{		MSG_BOX("efx_ripple1_normal0 Ready Failed");		return;	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_efx_ripple_dudv0", 
		L"../Bin/Resources/Texture/Effect/Distortion/efx_ripple_dudv%d.tga", 1)))	{		MSG_BOX("efx_ripple_dudv0 Ready Failed");		return;	}


	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/AttackStrong2Dis.dat", L"AttackStrong2Dis");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/AttackStrong3Dis.dat", L"AttackStrong3Dis");

////////////////////////////////////////////////////////////////////////////
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_LightningSubUV_001_TEX_CJS0", 
		L"../Bin/Resources/Texture/Effect/Lightning/FX_LightningSubUV_001_TEX_CJS%d.tga", 1)))	{		MSG_BOX("FX_LightningSubUV_001_TEX_CJS0 Ready Failed");		return;	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_light_all0", 
		L"../Bin/Resources/Texture/Effect/Lightning/light_all%d.tga", 1)))	{		MSG_BOX("light_all0 Ready Failed");		return;	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning_ani0", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning_ani%d.tga", 1)))	{		MSG_BOX("lightning_ani0 Ready Failed");		return;	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning_dark_ani0", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning_dark_ani%d.tga", 1)))
	{
		MSG_BOX("lightning_dark_ani Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning01_seq0", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning01_seq%d.tga", 1)))
	{
		MSG_BOX("lightning01_seq0 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning03_dark0", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning03_dark%d.tga", 1)))
	{
		MSG_BOX("lightning03_dark0 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning03_seq0", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning03_seq%d.tga", 1)))
	{
		MSG_BOX("lightning03_seq Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning04_dark0", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning04_dark%d.tga", 1)))
	{
		MSG_BOX("lightning04_dark0 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning07_A0", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning07_A%d.tga", 1)))
	{
		MSG_BOX("lightning07_A0 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning010", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning01%d.tga", 1)))
	{
		MSG_BOX("lightning010 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning020", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning02%d.tga", 1)))
	{
		MSG_BOX("lightning020 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning030", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning03%d.tga", 1)))
	{
		MSG_BOX("lightning030 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning040", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning04%d.tga", 1)))
	{
		MSG_BOX("lightning040 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning050", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning05%d.tga", 1)))
	{
		MSG_BOX("lightning050 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning060", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning06%d.tga", 1)))
	{
		MSG_BOX("lightning060 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning070", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning07%d.tga", 1)))
	{
		MSG_BOX("lightning070 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning080", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning08%d.tga", 1)))
	{
		MSG_BOX("lightning080 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning090", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning09%d.tga", 1)))
	{
		MSG_BOX("lightning090 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_lightning100", 
		L"../Bin/Resources/Texture/Effect/Lightning/lightning10%d.tga", 1)))
	{
		MSG_BOX("lightning100 Ready Failed");
		return;
	}
	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_wind_blade_small010", 
		L"../Bin/Resources/Texture/Effect/etc/wind_blade_small01%d.tga", 1)))
	{
		MSG_BOX("wind_blade_small01 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_cold_dust00", 
		L"../Bin/Resources/Texture/Effect/dust/cold_dust0%d.tga", 1)))
	{
		MSG_BOX("cold_dust00 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_trail_20", 
		L"../Bin/Resources/Texture/Effect/Blood/blood_trail_2%d.tga", 1)))
	{
		MSG_BOX("blood_trail_20 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_blood_005_TEX_CJH0", 
		L"../Bin/Resources/Texture/Effect/Blood/FX_blood_005_TEX_CJH%d.tga", 1)))
	{
		MSG_BOX("FX_blood_005_TEX_CJH0 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_FX_blood_006_TEX_CJH0", 
		L"../Bin/Resources/Texture/Effect/Blood/FX_blood_006_TEX_CJH%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Red_Big_One Ready Failed");
		return;
	}	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Red_Big_One.dat", L"Blood_Red_Big_One");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_core00", 
		L"../Bin/Resources/Texture/Effect/Blood/blood_core0%d.tga", 1)))
	{
		MSG_BOX("blood_core00 Ready Failed");
		return;
	}	
//
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_02_s0", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_02_s%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_02_s0 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_03_gray0", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_03_gray%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_03_gray0 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_03_s0", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_03_s%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_03_s0 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_010", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_01%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_010 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_020", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_02%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_02 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_030", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_03%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_030 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_040", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_04%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_040 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_small0", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_small%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_small0 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_evy_icelight_02_gray0", 
		L"../Bin/Resources/Texture/Effect/Ice/evy_icelight_02_gray%d.tga", 1)))
	{
		MSG_BOX("evy_icelight_02_gray0 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring_outer_wind030", 
		L"../Bin/Resources/Texture/Effect/ring_outer_wind03%d.tga", 1)))
	{
		MSG_BOX("ring_outer_wind030 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_shape_fire_100", 
		L"../Bin/Resources/Texture/Effect/shape_fire_10%d.tga", 1)))
	{
		MSG_BOX("shape_fire_100 Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_wind030", 
		L"../Bin/Resources/Texture/Effect/wind03%d.tga", 1)))
	{
		MSG_BOX("wind030 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_trail_00", 
		L"../Bin/Resources/Texture/Effect/blood_trail_0%d.tga", 1)))
	{
		MSG_BOX("blood_trail_00 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blood_trail_10", 
		L"../Bin/Resources/Texture/Effect/blood_trail_1%d.tga", 1)))
	{
		MSG_BOX("blood_trail_10 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_sneezn_red_010", 
		L"../Bin/Resources/Texture/Effect/sneezn_red_01%d.tga", 1)))
	{
		MSG_BOX("sneezn_red_010 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_sneezn_red_020", 
		L"../Bin/Resources/Texture/Effect/sneezn_red_02%d.tga", 1)))
	{
		MSG_BOX("sneezn_red_020 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_sneezn_red_030", 
		L"../Bin/Resources/Texture/Effect/sneezn_red_03%d.tga", 1)))
	{
		MSG_BOX("sneezn_red_030 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_sneezn_red_040", 
		L"../Bin/Resources/Texture/Effect/sneezn_red_04%d.tga", 1)))
	{
		MSG_BOX("sneezn_red_040 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_critical_ehen010", 
		L"../Bin/Resources/Texture/Effect/critical_ehen01%d.tga", 1)))
	{
		MSG_BOX("critical_ehen010 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_blade_01_smooth0", 
		L"../Bin/Resources/Texture/Effect/blade_01_smooth%d.tga", 1)))
	{
		MSG_BOX("blade_01_smooth0 Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_wind_blade_small0", 
		L"../Bin/Resources/Texture/Effect/wind_blade_small%d.tga", 1)))
	{
		MSG_BOX("wind_blade_small0 Ready Failed");
		return;
	}


	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring_outer_wind020", 
		L"../Bin/Resources/Texture/Effect/ring_outer_wind02%d.tga", 1)))
	{
		MSG_BOX("ring_outer_wind020 Ready Failed");
		return;
	}

	m_iLoadingNumber = 11;	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_smoke010", 
		L"../Bin/Resources/Texture/Effect/smoke01%d.tga", 1)))
	{
		MSG_BOX("smoke010 Ready Failed");
		return;
	}

	////////////////////////
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring0", 
		L"../Bin/Resources/Texture/Effect/ring%d.tga", 1)))
	{
		MSG_BOX("ring0 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_ring20", 
		L"../Bin/Resources/Texture/Effect/ring2%d.tga", 1)))
	{
		MSG_BOX("ring20 Ready Failed");
		return;
	}
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/AttackStrong2.dat", L"AttackStrong2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/AttackStrong2_2.dat", L"AttackStrong2_2");

	////////////////////////


	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Test_Lightning.dat", L"Test_Lightning");	// 라이트닝볼 기모을때
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Lightning_Tail.dat", L"Lightning_Tail");	// 라이트닝볼 발사 할때
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Lightning_Flore.dat", L"Lightning_Flore");	// 벼락 칠때 바닥
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Thunder_storm2.dat", L"Thunder_storm2");	// 벼락
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Body_Lightning2.dat", L"Body_Lightning2");	// 글리터 몸에 나는 전기 	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/wind_blade.dat", L"wind_blade");	// 문챠퍼 찌르기 2번후 휘리릭할때  	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Sting.dat", L"Sting");	// 문챠퍼 찌르기 2번할때 	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/LightningBolt_Attacked.dat", L"LightningBolt_Attacked");
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Tail_Tail.dat", L"Tail_Tail"); // 라이트닝볼 발사체의 꼬리

	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Missile.dat", L"Blood_Missile");		// 블러드로드 브래스 쏠때 미사일
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Breath_Start.dat", L"Blood_Breath_Start"); // 블러드로드 브래스 쏠때 첫 주둥아리 이펙트
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Breath_Doing.dat", L"Blood_Breath_Doing"); // 블러드로드 브래스 쏠때 처음끝나고 주둥아리
	
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Berserk_Dust1.dat", L"Berserk_Dust1"); // 버서크 쓸때 연기 이펙트 1
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Berserk_Dust2.dat", L"Berserk_Dust2"); // 버서크 쓸때 연기 이펙트 2
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Berserk_Dust3.dat", L"Berserk_Dust3"); // 버서크 쓸때 연기 이펙트 3
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Berserk_Dust4.dat", L"Berserk_Dust4"); // 버서크 쓸때 연기 이펙트 4
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/4th_Dust.dat", L"4th_Dust"); // 버서크 쓸때 연기 이펙트 4
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Jin.dat", L"Blood_Jin"); // 블러드 로드 피 폭발
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Jin2.dat", L"Blood_Jin2"); // 블러드 로드 피 폭발
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Dancing.dat", L"Blood_Dancing"); // 블러드로드 회오리 돌때
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/DustBig.dat", L"DustBig"); // 땅에 끌릴때 먼지 (1초동안 조금씩 Z축으로 움직임)
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Charge.dat", L"Blood_Charge"); // 블러드로드 돌격하면서 휘리릭할때 이펙트
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Jin_Ready.dat", L"Blood_Jin_Ready"); // 블러드로드 공중부양하기 직전 처음튀기는 피
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Blood_Jin_Ready2.dat", L"Blood_Jin_Ready2"); // 블러드로드 공중부양할때 아래로 흘리는 피
	//CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Fire_Test.dat", L"Fire_Test"); 
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/Dust_Fire.dat", L"Dust_Fire"); 
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/4th_Smash.dat", L"4th_Smash"); 
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/4th_Smash2.dat", L"4th_Smash2"); 
	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/LogoLight.dat", L"LogoLight");

	CEffectMgr::GetInstance()->Load_EffectData(L"../Bin/Data/Effect/4th_Smoke11.dat", L"4th_Smoke11");

#pragma endregion
	m_iLoadingNumber = 12;
#pragma region 트레일 버퍼, 텍스쳐
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_TRAILTEX, L"Buffer_PlayerTrailTex")))	
	{
		MSG_BOX("Buffer_PlayerTrailTex Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_MONTRAILTEX, L"Buffer_MonTrail")))	
	{
		MSG_BOX("Buffer_MonTrail Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_EYETRAILTEX, L"Buffer_EyeTrail")))	
	{
		MSG_BOX("Buffer_EyeTrail Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL, L"Texture_PlayerTrail", L"../Bin/Resources/Texture/Trail/Use/HurkTrail%d.tga", 7)))
	{
		MSG_BOX("Texture_RedTrail Ready Failed");
		return;
	}
#pragma endregion
	m_iLoadingNumber = 13;
	//Not Static
#pragma region 버퍼 로딩
	lstrcpy(m_szLoadMessage, L"Buffer Loading...");

	// For.Select Cloth
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_DYE, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_SelectCloth")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}
	// For.Select Dye Color
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_DYE, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_SelectDyeColor")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}
	// For.Select Dye Part
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_DYE, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_SelectPart")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}
	// For.Select Dye Part
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_DYE, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_DyeIcon")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return ;
	}
#pragma endregion
	m_iLoadingNumber = 14;
#pragma region 텍스쳐 로딩
	lstrcpy(m_szLoadMessage, L"Texture Loading...");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SelectCloth", 
		L"../Bin/Resources/Texture/DyeScene/clothtest%d.png", 1)))
	{
		MSG_BOX("Texture_SelectCloth Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SelectDyeColor", 
		L"../Bin/Resources/Texture/DyeScene/palette%d.png", 1)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SelectPart", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/UI/DyePartUI%d.png", 1)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	//// PartIcon
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconHead", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/DyePart_Head%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconUpper", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/DyePart_Upper%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconHand", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/DyePart_Hand%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconLower", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/DyePart_Lower%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconFoot", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/DyePart_Foot%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconSword", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/DyePart_Sword%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL ,
		L"Texture_PoseIcon", 
		L"../Bin/Resources/Texture/DyeScene/PoseIcon/pose%d.tga", 3)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	//// PartButton
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconPartA", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/UI/PartA_%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconPartB", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/UI/PartB_%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_DyeIconPartC", 
		L"../Bin/Resources/Texture/DyeScene/DyePart/UI/PartC_%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}

	//// EqIcon
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_EqIconSkeleton", 
		L"../Bin/Resources/Texture/DyeScene/EqIcon/skeleton/eq_skeleton_%d.tga", 5)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_EqIconHeremon", 
		L"../Bin/Resources/Texture/DyeScene/EqIcon/heremon/eq_heremon_%d.tga", 5)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_EqIconPirate", 
		L"../Bin/Resources/Texture/DyeScene/EqIcon/pirate/eq_pirate_%d.tga", 5)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_StartIcon", 
		L"../Bin/Resources/Texture/DyeScene/start_%d.tga", 2)))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SwordIconNight", 
		L"../Bin/Resources/Texture/DyeScene/EqIcon/sword/eq_sword_nighthawk0.tga")))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SwordIconSkeleton", 
		L"../Bin/Resources/Texture/DyeScene/EqIcon/sword/eq_sword_skeleton0.tga")))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_NORMAL  ,L"Texture_SwordIconHeremon", 
		L"../Bin/Resources/Texture/DyeScene/EqIcon/sword/eq_sword_heremon0.tga")))
	{
		MSG_BOX("Texture_SelectDyeColor Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL  ,L"Texture_Noise", 
		L"../Bin/Resources/Texture/Noise0.tga")))
	{
		MSG_BOX("Texture_Noise Ready Failed");
		return;
	}

	
#pragma endregion
	m_iLoadingNumber = 15;
#pragma region 스카이 박스
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_DYE, Engine::CVIBuffer::TYPE_CUBE
		, L"Buffer_Cube")))	
	{
		::MSG_BOX("Buffer_Cube Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_DYE, Engine::CTexture::TYPE_CUBE
		, L"Texture_Sky", L"../Bin/Resources/Texture/StageScene/SkyBox/SkyBox%d.dds", 2)))
	{
		::MSG_BOX("Texture_Sky Ready Failed");
		return;
	}
#pragma endregion
	m_iLoadingNumber = 16;
	lstrcpy(m_szLoadMessage, L"Let's Start...");
	m_bComplete = true;
}

void CLoading::MiddleStageLoading(void)
{
	HRESULT		hr = NULL;
	lstrcpy(m_szLoadMessage, L"Broken Mesh Loading...");
	m_iLoadingNumber = 0;
	CPlayer::GetInstance()->Ready_ClothMesh();

	m_iLoadingNumber = 1;
#pragma region 다이나믹 메쉬
	lstrcpy(m_szLoadMessage, L"Glitter Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGEMIDDLE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Glitter", L"../Bin/Resources/Mesh/DynamicMesh/Monster/Glitter/", L"Glitter.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Glitter)");
		return;
	}

	m_iLoadingNumber = 3;
	lstrcpy(m_szLoadMessage, L"Moonchopper Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGEMIDDLE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Moonchopper", L"../Bin/Resources/Mesh/DynamicMesh/Monster/Moonchopper/", L"Moonchopper.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Moonchopper)");
		return;
	}

	m_iLoadingNumber = 5;
	lstrcpy(m_szLoadMessage, L"Jelly Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Jelly", L"../Bin/Resources/Mesh/DynamicMesh/Monster/Jelly/", L"Jelly_bc.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Mesh_Jelly)");
		return;
	}

	m_iLoadingNumber = 7;
	lstrcpy(m_szLoadMessage, L"Royal Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Royal", L"../Bin/Resources/Mesh/DynamicMesh/Monster/Royal/", L"Royal.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(Mesh_Royal)");
		return;
	}
#pragma endregion
	m_iLoadingNumber = 9;
#pragma region BloodUI 버퍼, 텍스쳐 로딩
	lstrcpy(m_szLoadMessage, L"BeShot UI Resource Loading...");
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood0")))
	{
		MSG_BOX("Buffer_Blood0 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood1")))
	{
		MSG_BOX("Buffer_Blood1 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood2")))
	{
		MSG_BOX("Buffer_Blood2 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood3")))
	{
		MSG_BOX("Buffer_Blood3 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood4")))
	{
		MSG_BOX("Buffer_Blood4 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood5")))
	{
		MSG_BOX("Buffer_Blood5 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood6")))
	{
		MSG_BOX("Buffer_Blood6 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood7")))
	{
		MSG_BOX("Buffer_Blood7 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood8")))
	{
		MSG_BOX("Buffer_Blood8 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood9")))
	{
		MSG_BOX("Buffer_Blood9 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood10")))
	{
		MSG_BOX("Buffer_Blood10 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood11")))
	{
		MSG_BOX("Buffer_Blood11 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood12")))
	{
		MSG_BOX("Buffer_Blood12 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood13")))
	{
		MSG_BOX("Buffer_Blood13 Ready_Buffer Failed");
		return ;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX2, L"Buffer_Blood14")))
	{
		MSG_BOX("Buffer_Blood14 Ready_Buffer Failed");
		return ;
	}

	///////////////////////////

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, Engine::CTexture::TYPE_NORMAL, L"Texture_Blood", 
		L"../Bin/Resources/Texture/StageScene/UIResource/Blood/Blood%d.png", 15)))
	{
		MSG_BOX("Texture_Blood Ready Failed");
		return;
	}

#pragma endregion
	m_iLoadingNumber = 10;
#pragma region Terrain Buffer Loading
	lstrcpy(m_szLoadMessage, L"Terrain Buffer Loading...");
	HANDLE	hFile = CreateFile(L"../Bin/Resources/Data/Map/Terrain_BloodLord_2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	_int iCntX;
	_int iCntZ;
	_int iItv;

	ReadFile(hFile, &iCntX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iCntZ, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iItv, sizeof(int), &dwByte, NULL);

	TCHAR TerrainTextureName[128];
	ZeroMemory(TerrainTextureName, sizeof(TCHAR) * 128);
	ReadFile(hFile, &TerrainTextureName, sizeof(TCHAR) * 128, &dwByte, NULL);

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGEMIDDLE, Engine::CVIBuffer::TYPE_TERRAIN, L"Buffer_Terrain", iCntX, iCntZ, iItv)))
	{
		MSG_BOX("Buffer_Terrain Ready Failed");
		return;
	}

	Engine::VTXTEX* pVertex = new Engine::VTXTEX[iCntX * iCntZ];
	ReadFile(hFile, (void*)pVertex, sizeof(Engine::VTXTEX) * iCntX * iCntZ, &dwByte, NULL);

	CloseHandle(hFile);

	Engine::CVIBuffer* m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STAGEMIDDLE, L"Buffer_Terrain");
	m_pBufferCom->Set_VtxInfo(pVertex);

	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Delete_Array(pVertex);
#pragma endregion
	m_iLoadingNumber = 12;
#pragma region 네비메쉬 레디
	lstrcpy(m_szLoadMessage, L"NaviMesh Loading...");
	{
		Engine::Get_NaviMesh()->Ready_GraphicDev(m_pGraphicDev);
		HANDLE hFile = CreateFile(L"../Bin/Resources/Data/Map/Navi_BloodLord_2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD		dwByte;

		while(TRUE)
		{
			_vec3 vPointA(0.f, 0.f, 0.f);
			_vec3 vPointB(0.f, 0.f, 0.f);
			_vec3 vPointC(0.f, 0.f, 0.f);
			ReadFile(hFile, vPointA, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointB, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointC, sizeof(_vec3), &dwByte, NULL);

			if(dwByte == 0)
			{
				break;
			}
			Engine::Get_NaviMesh()->Add_Cell(&vPointA, &vPointB, &vPointC);
			Engine::Get_NaviMesh()->Link_Cell();
		}
		CloseHandle(hFile);
	}
	m_iLoadingNumber = 12;
	{
		Engine::Get_CameraNaviMesh()->Ready_GraphicDev(m_pGraphicDev);
		HANDLE hFile = CreateFile(L"../Bin/Resources/Data/Map/Navi_BloodLord_2(Camera).dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD		dwByte;

		while(TRUE)
		{
			_vec3 vPointA(0.f, 0.f, 0.f);
			_vec3 vPointB(0.f, 0.f, 0.f);
			_vec3 vPointC(0.f, 0.f, 0.f);
			ReadFile(hFile, vPointA, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointB, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointC, sizeof(_vec3), &dwByte, NULL);

			if(dwByte == 0)
			{
				break;
			}
			Engine::Get_CameraNaviMesh()->Add_Cell(&vPointA, &vPointB, &vPointC);
			Engine::Get_CameraNaviMesh()->Link_Cell();
		}
		CloseHandle(hFile);
	}
#pragma endregion
	m_iLoadingNumber = 14;
#pragma region 기타
	lstrcpy(m_szLoadMessage, L"Etc Loading...");
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGEMIDDLE, Engine::CVIBuffer::TYPE_CUBE
		, L"Buffer_Cube")))	
	{
		::MSG_BOX("Buffer_Cube Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGEMIDDLE, Engine::CTexture::TYPE_NORMAL, L"Texture_Terrain", L"../Bin/Resources/Texture/StageScene/Terrain/ground0D.jpg")))
	{
		MSG_BOX("Texture_Terrain Failed");	return;
	}
	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGEMIDDLE, Engine::CTexture::TYPE_CUBE
		, L"Texture_Sky", L"../Bin/Resources/Texture/StageScene/SkyBox/SkyBox%d.dds", 1)))
	{
		::MSG_BOX("Texture_Sky Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGEMIDDLE, Engine::CTexture::TYPE_NORMAL, L"Texture_TerrainNor", L"../Bin/Resources/Texture/StageScene/Terrain/Bloodload2_GroundN.jpg")))
	{
		MSG_BOX("Texture_Terrain Failed");	return;
	}

	
#pragma endregion
	m_iLoadingNumber = 16;
	lstrcpy(m_szLoadMessage, L"Loading Complete!!!");

	m_bComplete = true;
}

void CLoading::StageLoading(void)
{
	//Scene Num
	//Engine::CSceneNumMgr::GetInstance()->Set_CurrentScene(0);	// Loading

	HRESULT		hr = NULL;
	m_iLoadingNumber = 0;
#pragma region 다이나믹 메쉬
   	lstrcpy(m_szLoadMessage, L"BloodLord Mesh Loading...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_BloodLord", L"../Bin/Resources/Mesh/DynamicMesh/Monster/BloodLord/", L"BloodLord.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed(BloodLord)");
		return;
	}
#pragma endregion
	m_iLoadingNumber = 8;
#pragma region Terrain Buffer Loading
	lstrcpy(m_szLoadMessage, L"Terrain Buffer Loading...");
	HANDLE	hFile = CreateFile(L"../Bin/Resources/Data/Map/Terrain_BloodLord_3.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	_int iCntX;
	_int iCntZ;
	_int iItv;

	ReadFile(hFile, &iCntX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iCntZ, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iItv, sizeof(int), &dwByte, NULL);

	TCHAR TerrainTextureName[128];
	ZeroMemory(TerrainTextureName, sizeof(TCHAR) * 128);
	ReadFile(hFile, &TerrainTextureName, sizeof(TCHAR) * 128, &dwByte, NULL);

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_TERRAIN, L"Buffer_Terrain", iCntX, iCntZ, iItv)))
	{
		MSG_BOX("Buffer_Terrain Ready Failed");
		return;
	}


	Engine::VTXTEX* pVertex = new Engine::VTXTEX[iCntX * iCntZ];
	ReadFile(hFile, (void*)pVertex, sizeof(Engine::VTXTEX) * iCntX * iCntZ, &dwByte, NULL);

	CloseHandle(hFile);

	Engine::CVIBuffer* m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Terrain");
	m_pBufferCom->Set_VtxInfo(pVertex);
	
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Delete_Array(pVertex);
#pragma endregion
	m_iLoadingNumber = 11;
#pragma region 네비메쉬 레디
	lstrcpy(m_szLoadMessage, L"NaviMesh Loading...");
	{
		Engine::Get_NaviMesh()->Ready_GraphicDev(m_pGraphicDev);
		HANDLE hFile = CreateFile(L"../Bin/Resources/Data/Map/Navi_BloodLord_3.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD		dwByte;

		while(TRUE)
		{
			_vec3 vPointA(0.f, 0.f, 0.f);
			_vec3 vPointB(0.f, 0.f, 0.f);
			_vec3 vPointC(0.f, 0.f, 0.f);
			ReadFile(hFile, vPointA, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointB, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointC, sizeof(_vec3), &dwByte, NULL);

			if(dwByte == 0)
			{
				break;
			}
			Engine::Get_NaviMesh()->Add_Cell(&vPointA, &vPointB, &vPointC);
			Engine::Get_NaviMesh()->Link_Cell();
		}
		CloseHandle(hFile);
	}
	m_iLoadingNumber = 12;
	{
		Engine::Get_CameraNaviMesh()->Ready_GraphicDev(m_pGraphicDev);
		HANDLE hFile = CreateFile(L"../Bin/Resources/Data/Map/Navi_BloodLord_3(Camera).dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD		dwByte;

		while(TRUE)
		{
			_vec3 vPointA(0.f, 0.f, 0.f);
			_vec3 vPointB(0.f, 0.f, 0.f);
			_vec3 vPointC(0.f, 0.f, 0.f);
			ReadFile(hFile, vPointA, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointB, sizeof(_vec3), &dwByte, NULL);
			ReadFile(hFile, vPointC, sizeof(_vec3), &dwByte, NULL);

			if(dwByte == 0)
			{
				break;
			}
			Engine::Get_CameraNaviMesh()->Add_Cell(&vPointA, &vPointB, &vPointC);
			Engine::Get_CameraNaviMesh()->Link_Cell();
		}
		CloseHandle(hFile);
	}
#pragma endregion
	m_iLoadingNumber = 14;
#pragma region 기타
	lstrcpy(m_szLoadMessage, L"Etc Loading...");
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, Engine::CTexture::TYPE_NORMAL, L"Texture_Terrain", L"../Bin/Resources/Texture/StageScene/Terrain/ground1D.jpg")))
	{
		MSG_BOX("Texture_Terrain Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, Engine::CTexture::TYPE_NORMAL, L"Texture_TerrainNor", L"../Bin/Resources/Texture/StageScene/Terrain/ground1N.tga")))
	{
		MSG_BOX("Texture_TerrainNor Failed");
		return;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_CUBE
		, L"Buffer_Cube")))	
	{
		::MSG_BOX("Buffer_Cube Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, Engine::CTexture::TYPE_CUBE
		, L"Texture_Sky", L"../Bin/Resources/Texture/StageScene/SkyBox/SkyBox%d.dds", 1)))
	{
		::MSG_BOX("Texture_Sky Ready Failed");
		return;
	}

#pragma endregion
	m_iLoadingNumber = 16;
	lstrcpy(m_szLoadMessage, L"Loading Complete!!!");
	
	m_bComplete = true;
}

CLoading* CLoading::Create(LOADINGTYPE eLoadingType)
{
	CLoading*	pLoding = new CLoading(eLoadingType);

	pLoding->InitLoading();

	return pLoding;
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{
	CLoading*	pLoading = (CLoading*)pArg;

	EnterCriticalSection(pLoading->GetCSKey());

	switch(pLoading->GetLoadType())
	{
	case CLoading::LOADING_DYE:
		pLoading->DyeSceneLoading();
		break;

	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
		break;

	case CLoading::LOADING_MIDDLE_STAGE:
		pLoading->MiddleStageLoading();
		break;
	}

	LeaveCriticalSection(pLoading->GetCSKey());
	return 0;
}

_ulong CLoading::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		DeleteCriticalSection(&m_CSKey);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		delete this;
	}
	return dwRefCnt;	
}