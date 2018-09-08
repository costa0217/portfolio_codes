#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include <process.h>

#include "SDevice.h"

CLoading::CLoading(LOADINGTYPE eLoadingType)
: m_LoadType(eLoadingType)
, m_pNaviMesh(Engine::Get_NaviMesh())
, m_bComplete(false)
, m_pGraphicDev(NULL)
, m_dwCntX(0)
, m_dwCntZ(0)
, m_dwItv(1)
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

	CSDevice::GetInstance()->SoundPlay(SD_LOADING, 1);		// 타이틀 비지엠
}

void CLoading::StageLoading(void)
{
	HRESULT		hr = NULL;
	lstrcpy(m_szLoadMessage, L"메쉬 로딩중...");

	// 보스칼
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_BSword1", L"../Bin/Resource/Meshes/StaticMesh/BossSword/", L"Boss_Sword1.x"))){MSG_BOX("Ready_MeshBS1 Ready Failed");return;	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_BSword2", L"../Bin/Resource/Meshes/StaticMesh/BossSword/", L"Boss_Sword2.x"))){MSG_BOX("Ready_MeshBS2 Ready Failed");return;	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_BSword3", L"../Bin/Resource/Meshes/StaticMesh/BossSword/", L"Boss_Sword3.x"))){MSG_BOX("Ready_MeshBS3 Ready Failed");return;	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_BSword4", L"../Bin/Resource/Meshes/StaticMesh/BossSword/", L"Boss_Sword4.x"))){MSG_BOX("Ready_MeshBS4 Ready Failed");return;	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_BSword5", L"../Bin/Resource/Meshes/StaticMesh/BossSword/", L"Boss_Sword5.x"))){MSG_BOX("Ready_MeshBS5 Ready Failed");return;	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_IceAge", L"../Bin/Resource/Meshes/StaticMesh/Effect/", L"IceAge.x"))){MSG_BOX("IceAge Ready Failed");return;	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_FireBall", L"../Bin/Resource/Meshes/StaticMesh/Effect/", L"FireBall.x"))){MSG_BOX("FireBall Ready Failed");return;	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Crystal", L"../Bin/Resource/Meshes/StaticMesh/Effect/Crystal/", L"Crystal.x"))){MSG_BOX("FireBall Ready Failed");return;	}


	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Stone", L"../Bin/Resource/Meshes/StaticMesh/TombStone/", L"TombStone.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Player", L"../Bin/Resource/Meshes/DynamicMesh/Player/", L"Player_Elf.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Sword", L"../Bin/Resource/Meshes/DynamicMesh/Weapon/Sword/", L"Weapon.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Shield", L"../Bin/Resource/Meshes/DynamicMesh/Weapon/Shield/", L"Shield.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_SkyDome", L"../Bin/Resource/Meshes/StaticMesh/SkyDome/", L"SkyDome.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Tree1", L"../Bin/Resource/Meshes/StaticMesh/Tree/", L"Tree1.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Tree2", L"../Bin/Resource/Meshes/StaticMesh/Tree/", L"Tree2.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Stair", L"../Bin/Resource/Meshes/StaticMesh/Stair/", L"Stair.x")))
	{
		MSG_BOX("Ready_Mesh  Stair Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Inka", L"../Bin/Resource/Meshes/StaticMesh/Inka/", L"Inka.x")))
	{
		MSG_BOX("Ready_Mesh  Inka Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Weed", L"../Bin/Resource/Meshes/StaticMesh/Weed/", L"Weed.x")))
	{
		MSG_BOX("Ready_Mesh  Weed Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"PineTree", L"../Bin/Resource/Meshes/StaticMesh/PineTree/", L"PineTree.x")))
	{
		MSG_BOX("Ready_Mesh  PineTree Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"ParmTree", L"../Bin/Resource/Meshes/StaticMesh/ParmTree/", L"ParmTree.x")))
	{
		MSG_BOX("Ready_Mesh  ParmTree Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Cage", L"../Bin/Resource/Meshes/StaticMesh/Cage/", L"Cage.x")))
	{
		MSG_BOX("Ready_Mesh  Cage Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"BananaTree", L"../Bin/Resource/Meshes/StaticMesh/BananaTree/", L"BananaTree.x")))
	{
		MSG_BOX("Ready_Mesh  BananaTree Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"WarpGate", L"../Bin/Resource/Meshes/StaticMesh/WarpGate/", L"WarpGate.x")))
	{
		MSG_BOX("Ready_Mesh  WarpGate Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Statu", L"../Bin/Resource/Meshes/StaticMesh/Statu/", L"Statu.x")))
	{
		MSG_BOX("Ready_Mesh  Statu Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Pot1", L"../Bin/Resource/Meshes/StaticMesh/Pot/", L"Pot1.x")))
	{
		MSG_BOX("Ready_Mesh  Pot1 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Pot2", L"../Bin/Resource/Meshes/StaticMesh/Pot/", L"Pot2.x")))
	{
		MSG_BOX("Ready_Mesh  Pot2 Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"BlackSun", L"../Bin/Resource/Meshes/StaticMesh/BlackSun/", L"BlackSun.x")))
	{
		MSG_BOX("Ready_Mesh  BlackSun Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Boss", L"../Bin/Resource/Meshes/DynamicMesh/Boss/", L"Boss.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Ground", L"../Bin/Resource/Meshes/StaticMesh/GroundHit/", L"GroundHit.x")))
	{
		MSG_BOX("Ready_Mesh_Ground Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Smoke", L"../Bin/Resource/Meshes/StaticMesh/Effect/SmokeSphere/", L"Smoke.x")))
	{
		MSG_BOX("Ready Mesh_Smoke Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Wand", L"../Bin/Resource/Meshes/StaticMesh/Effect/Wang/", L"Wang.x")))
	{
		MSG_BOX("Ready Mesh_Wand Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Swirl", L"../Bin/Resource/Meshes/StaticMesh/Effect/Wang/", L"Swirl.x")))
	{
		MSG_BOX("Ready Mesh_Swirl Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_FireTrail", L"../Bin/Resource/Meshes/StaticMesh/Effect/FireTrail/", L"Fire_Trail.x")))
	{
		MSG_BOX("Ready Mesh_FireTrail Ready Failed");
		return;
	}
	


	lstrcpy(m_szLoadMessage, L"텍스쳐 로딩중...");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Terrain", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Terrain/Tile/Tile_D%d.tga", 4)))
	{
		MSG_BOX("Texture_Terrain Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_TerrainNormal", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Terrain/Tile/Tile_N%d.tga", 4)))
	{
		MSG_BOX("Texture_Terrain Normal Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Alpha", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Terrain/AlphaTexture%d.jpg", 1)))
	{
		MSG_BOX("Texture_Alpha Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Sea", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Environment/Sea/Sea%d.jpg", 1)))
	{
		MSG_BOX("Texture_Sea Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Trail", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Trail/Trail%d.tga", 18)))
	{
		MSG_BOX("Texture_Trail Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Impact_Up", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/IceUp/FX_Impact_Up%d.tga", 1)))
	{
		MSG_BOX("Texture_Impact_Up Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_bigdirt", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/IceUp/FX_bigdirt%d.tga", 1)))
	{
		MSG_BOX("Texture_bigdirt Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_SwordCircle", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/SwordCircle/magicZone_KKJ%d.tga", 1)))
	{
		MSG_BOX("Texture_SwordCircle Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_BossFace", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/UI/BossFace%d.tga", 1)))
	{
		MSG_BOX("Texture_BossFace Ready Failed");
		return;
	}


	//UI
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Class", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/UI/ClassUI%d.tga", 1)))
	{
		MSG_BOX("Texture_Class Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Healthy", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/UI/HPBar%d.png", 3)))
	{
		MSG_BOX("Texture_Healthy Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Blood_Single", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/Blood/Blood_Single%d.tga", 1)))
	{
		MSG_BOX("Texture_Blood_Single Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Blood_Black", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/Blood/Blood_Black%d.tga", 3)))
	{
		MSG_BOX("Texture_Blood_Black Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_MagicCircle", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/MagicCircle/MagicCircle%d.tga", 5)))
	{
		MSG_BOX("Texture_MagicCircle Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Shock", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/Impact/Impact%d.tga", 4)))
	{
		MSG_BOX("Texture_Shock Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Impact", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/Impact/Impact_p%d.tga", 4)))
	{
		MSG_BOX("Texture_Impact Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Bloody", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/Effect/Bloody/LowHP%d.tga", 1)))
	{
		MSG_BOX("Texture_Bloody Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_SkillWnd", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/UI/Skill%d.png", 4)))
	{
		MSG_BOX("Texture_SkillWnd Ready Failed");
		return;
	}	
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_SkillBar", Engine::CTexture::TYPE_NORMAL
		, L"../bin/Resource/Texture/StageScene/UI/UIBar%d.png", 1)))
	{
		MSG_BOX("Texture_SkillBar Ready Failed");
		return;
	}	
	


	lstrcpy(m_szLoadMessage, L"버퍼 생성중...");

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_TRAIL
		, L"Buffer_Trail")))	
	{
		MSG_BOX("Buffer_TrailTex Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_TRAIL
		, L"Buffer_Trail2")))	
	{
		MSG_BOX("Buffer_TrailTex Ready Failed");
		return;
	}
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_TRAIL
		, L"Buffer_Trail3")))	
	{
		MSG_BOX("Buffer_TrailTex Ready Failed");
		return;
	}

	LoadingSaveMapData();
	LoadingSaveNaviData();

	


	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_TERRAIN
		, L"Buffer_Terrain", (_ushort)m_dwCntX, (_ushort)m_dwCntZ, (_ushort)m_dwItv, &m_vecPos)))	
	{
		MSG_BOX("Buffer_Terrain Ready Failed");
		return;
	}

	

	lstrcpy(m_szLoadMessage, L"로딩 완료...");
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
	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
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

		CSDevice::GetInstance()->SoundStop(SD_LOADING);
		delete this;
	}
	return dwRefCnt;	
}

void CLoading::LoadingSaveMapData(void)
{
	HANDLE		hFile = CreateFile(L"../Data/MapTest.dat", GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	ReadFile(hFile,  &m_dwCntX, sizeof(_ulong), &dwByte, NULL);
	ReadFile(hFile,  &m_dwCntZ, sizeof(_ulong), &dwByte, NULL);
	ReadFile(hFile,  &m_dwItv, sizeof(_ulong), &dwByte, NULL);

	VTXCNTX = m_dwCntX;
	VTXCNTZ = m_dwCntZ;
	VTXITV = m_dwItv;

	_vec3	vTerrainPos(0.f, 0.f, 0.f);

	m_vecPos.reserve(m_dwCntX * m_dwCntZ);


	for(_ulong i = 0; i < m_dwCntX * m_dwCntZ; ++i)
	{
		ReadFile(hFile,  &vTerrainPos, sizeof(_vec3), &dwByte, NULL);
		m_vecPos.push_back(vTerrainPos);

		if(dwByte == 0)
			break;
	}
	CloseHandle(hFile);
}

void CLoading::LoadingSaveNaviData(void)
{

	m_pNaviMesh->Reserve_CellContainerSize(m_pGraphicDev, 400);
	HANDLE		hFile = CreateFile(L"../Data/NaviTest.dat", GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD				dwByte;

	int					iCnt = 0;

	while(true)
	{
		_vec3	vPoint[3];
		ZeroMemory(vPoint, sizeof(_vec3) * 3);

		ReadFile(hFile,  &vPoint[iCnt], sizeof(_vec3), &dwByte, NULL);
		++iCnt;
		ReadFile(hFile,  &vPoint[iCnt], sizeof(_vec3), &dwByte, NULL);
		++iCnt;
		ReadFile(hFile,  &vPoint[iCnt], sizeof(_vec3), &dwByte, NULL);
		iCnt = 0;

		Engine::Get_NaviMesh()->Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2]);

		if(dwByte == 0)
			break;
	}
	CloseHandle(hFile);

	m_pNaviMesh->Link_Cell();
}
