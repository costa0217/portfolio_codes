#include "stdafx.h"
#include "Title.h"

#include "Include.h"
#include "Engine_Include.h"
#include "Export_Function.h"

#include "SceneSelector.h"

#include "RcColor.h"
#include "Layer.h"
#include "GameObject.h"
#include "StaticCamera.h"
#include "Transform.h"
#include "CollisionMgr.h"

#include "ObjDataMgr.h"
#include "Loading.h"

unsigned int __stdcall BufferLoadThread(void* pArg)
{
	CTitle*			pTitle = (CTitle*)pArg;

	EnterCriticalSection(&pTitle->GetCrt());

	LPDIRECT3DDEVICE9	m_pDevice = Engine::Get_GraphicDev()->GetDevice();

	HRESULT		hr = NULL;

	/* 기본버퍼 */
	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_RCCOL, L"Buffer_TriColor");
	FAILED_CHECK(hr)

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC // 0804
		, Engine::CVIBuffer::BUFFER_RCCOL, L"Buffer_RcCol");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex2");
	FAILED_CHECK(hr);

	


	/* 텍스쳐 */

	//로고 
	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_LogoBack", L"../bin/Resources/LogoBack/LogoBack_%d.png", 21);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Exit", L"../bin/Resources/Button/Exit/Exit%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Option", L"../bin/Resources/Button/Option/Option%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Tutorial", L"../bin/Resources/Button/Tutorial/Tutorial%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Play", L"../bin/Resources/Button/Play/Play%d.png", 2);

	//스테이지
	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Player", L"../bin/Resources/Player%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"Texture_Cube", L"../bin/Resources/Monster%d.dds", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"Texture_ManCube", L"../bin/Resources/Soldier%d.dds", 3);
	FAILED_CHECK(hr);	

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Dust", L"../bin/Resources/Effect/Dust/Dust%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Flare", L"../bin/Resources/Effect/Flare/Flare%d.png", 4);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Smoke", L"../bin/Resources/Effect/Smoke/Smoke%d.png", 6);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Bullet", L"../bin/Resources/Bullet/Bullet%d.png", 2);
	FAILED_CHECK(hr);


	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Water", L"../bin/Resources/Water/Water%d.png", 9);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_HPBar", L"../bin/Resources/UI/HPBar%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_BulletBar", L"../bin/Resources/UI/BulletBar%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_TimeUI", L"../bin/Resources/UI/Time%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_MissionUI", L"../bin/Resources/UI/Mission%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_LimitUI", L"../bin/Resources/UI/Limit%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Wire", L"../bin/Resources/Wire/Wire%d.png", 3);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_AlphaBack", L"../bin/Resources/Pause/Alpha/AlphaBack%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_GoMain", L"../bin/Resources/Pause/GoMain/GoMain%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Restart", L"../bin/Resources/Pause/Restart/Restart%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Back", L"../bin/Resources/Pause/Back/Back%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Rain", L"../bin/Resources/Rain/Rain%d.png", 6);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Explosion", L"../bin/Resources/Explosion/Explosion%d.png", 10);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Mist", L"../bin/Resources/Effect/Mist/Mist%d.png", 6);
	FAILED_CHECK(hr);
	//////
	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_HeliExplo", L"../bin/Resources/Effect/Ex_A/Exa%d.png", 29);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_MissileExplo", L"../bin/Resources/Effect/Ex_B/Exb%d.png", 16);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_MonExplo", L"../bin/Resources/Effect/Ex_C/Exc%d.png", 16);
	FAILED_CHECK(hr);
	/////

	/************************************************************************/
	/*								  Terrain Texture                       */
	/************************************************************************/

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_TerrainFront", L"../bin/Resources/Terrain/MapFrontFinalLine%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_TerrainMid", L"../bin/Resources/Terrain/MapMidFinalFix%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_TerrainBack", L"../bin/Resources/Terrain/MapBackFinal%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_TerrainCliff_Left", L"../bin/Resources/Terrain/VerticalWallLine_Left%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_TerrainCliff_Right", L"../bin/Resources/Terrain/VerticalWallLine_Right%d.png", 1);
	FAILED_CHECK(hr);

	//Sky
	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Sky", L"../bin/Resources/Terrain/SkyDot%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Tree", L"../bin/Resources/Terrain/BigTree%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_TreeFog", L"../bin/Resources/Terrain/BigTreeFog%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_BigTree", L"../bin/Resources/Terrain/BigBigBigTree%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_SwapBigTree", L"../bin/Resources/Terrain/BigBigBigBangTree%d.png", 1);
	FAILED_CHECK(hr);

	/************************************************************************/
	/*							 TerrainTex						            */
	/************************************************************************/

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TERRAINTEX, L"Buffer_TerrainTex", L"../Data/Map/228_33_8FrontHeight.dat");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TERRAINTEX, L"Buffer_TerrainTex_Mid", L"../Data/Map/114_33_16MidHeight8.dat");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TERRAINTEX, L"Buffer_TerrainTex_Back", L"../Data/Map/57_17_32BackHeight5.dat");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TERRAINTEX, L"Buffer_TerrainTex_Cliff", L"../Data/Map/17_129_32cliff.dat");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TERRAINTEX, L"Buffer_TerrainTex_Sky", L"../Data/Map/0402600Cliff.dat");
	FAILED_CHECK(hr);

	//hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
	//	, Engine::TEXTURE_NORMAL, L"Texture_TerrainFront2", L"../bin/Resources/Terrain/MapFront%d.png", 1);
	//FAILED_CHECK(hr);

	//hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
	//	, Engine::TEXTURE_NORMAL, L"Texture_TerrainMid2", L"../bin/Resources/Terrain/MapMid%d.png", 1);
	//FAILED_CHECK(hr);

	//hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
	//	, Engine::TEXTURE_NORMAL, L"Texture_TerrainBack2", L"../bin/Resources/Terrain/MapBack%d.png", 1);
	//FAILED_CHECK(hr);

	//hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
	//	, Engine::TEXTURE_NORMAL, L"Texture_Sky2", L"../bin/Resources/Terrain/Sky%d.png", 1);
	//FAILED_CHECK(hr);


	/************************************************************************/

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_CUBETEX, L"Buffer_CubeTex");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_WIRETEX, L"Buffer_WireTex");
	FAILED_CHECK(hr);

	
	///////////
	//DDS
	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"Texture_Fragment", L"../bin/Resources/DDS/%d.dds", 15);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"Texture_ItemBullet", L"../bin/Resources/Item/ItemBullet%d.dds", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"Texture_ItemHp", L"../bin/Resources/Item/ItemHp%d.dds", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"SoldierOne", L"../bin/Resources/Soldier/SoldierOne%d.dds", 4);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"SoldierTwo", L"../bin/Resources/Soldier/SoldierTwo%d.dds", 4);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"SoldierThree", L"../bin/Resources/Soldier/SoldierThree%d.dds", 4);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"SoldierFour", L"../bin/Resources/Soldier/SoldierFour%d.dds", 4);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_CUBE, L"SoldierFive", L"../bin/Resources/Soldier/SoldierFive%d.dds", 4);
	FAILED_CHECK(hr);
	////

	//★★★★★★예슬님추가부분★★★★★★★

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_HeliStatus", L"../bin/Resources/Choice/HeliStatus/HeliStatus%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Kai", L"../bin/Resources/Choice/KaiButton/Kai%d.png", 3);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Chinook", L"../bin/Resources/Choice/ChinookButton/Chinook%d.png", 3);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_WeaponStatus", L"../bin/Resources/Choice/WeaponStatus/WeaponStatus%d.png", 3);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Gun", L"../bin/Resources/Choice/GunButton/Gun%d.png", 3);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_GatlingGun", L"../bin/Resources/Choice/GatlingButton/GatlingGun%d.png", 3);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Launcher", L"../bin/Resources/Choice/LauncherButton/Launcher%d.png", 3);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_StartButton", L"../bin/Resources/Choice/StartButton/Start%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Clear", L"../bin/Resources/Clear/Clear%d.png", 2);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Cargo", L"../bin/Resources/UI/Cargo%d.png", 1);
	FAILED_CHECK(hr);

	//★★★★★★★★★★★★★★★★★★★★


	//데이터 버퍼
	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Heli_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_HELICOL
		, L"Buffer_HeliCol");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/propeller_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_PROPELL
		, L"Buffer_Propeller");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/minipeller_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_MINIPROPELL
		, L"Buffer_MiniPropeller");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Hook_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_HOOK
		, L"Buffer_Hook");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Retona_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_RETONA
		, L"Buffer_Retona");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Dotge_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_DODGE
		, L"Buffer_Dodge");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Tank_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TANK
		, L"Buffer_TankBody");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/TankPaw_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TANKPAW
		, L"Buffer_TankPaw");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Paw_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TANKPAWSIN
		, L"Buffer_TankPawSin");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Bunker_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_BUNKER
		, L"Buffer_Bunker");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Bullet1_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_BULLET
		, L"Buffer_Bullet");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/MultiBullet_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_MULTIBULLET
		, L"Buffer_MultiBullet");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Misile_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_MISILE
		, L"Buffer_Misile");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Gun_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_GUN
		, L"Buffer_Gun");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/GatlingGun_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_GATLINGGUN
		, L"Buffer_GatlingGun");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Launcher_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_LAUNCHER
		, L"Buffer_Launcher");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/AirGunBody_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_AIRGUNBODY
		, L"Buffer_AirGunBody");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/AirGunHead_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_AIRGUNHEAD
		, L"Buffer_AirGunHead");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/AirGunPaw_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_AIRGUNPAW
		, L"Buffer_AirGunPaw");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Bomb_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_BOMB
		, L"Buffer_Bomb");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Chinook_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_CHINOOK
		, L"Buffer_Chinook");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Fence_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_FENCE
		, L"Buffer_Fence");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/Tent_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TENT
		, L"Buffer_Tent");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/WireFence_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_WIREFENCE
		, L"Buffer_WireFence");

	//
	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/BackCloud_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_BLACKCLOUD
		, L"Buffer_BlackCloud");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/FrontCloud_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_FRONTCLOUD
		, L"Buffer_FrontCloud");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/MidCloud_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_MIDCLOUD
		, L"Buffer_MidCloud");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/SmallTree_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_SMALLTREE
		, L"Buffer_SmallTree");

	CObjDataMgr::GetInstance()->LoadObjectData(m_pDevice, L"../Data/Object/TallTree_min.dat"
		, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_TALLTREE
		, L"Buffer_TallTree");

	

	
	LeaveCriticalSection(&pTitle->GetCrt());
	//_endthreadex(0);

	return 0;
}


CTitle::CTitle(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
{

}

CTitle::~CTitle(void)
{
	Release();
}

HRESULT CTitle::InitScene(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	HRESULT hr = NULL;
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	hr = Engine::Get_ResourcesMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex3");
	//FAILED_CHECK(hr);

	hr = Engine::Get_ResourcesMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC
		, Engine::TEXTURE_NORMAL, L"Texture_Loading", L"../bin/Resources/Loading/%d.png", 4);
	FAILED_CHECK(hr);	

	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, BufferLoadThread, this, 0, NULL);

	FAILED_CHECK(Add_GameLogic_Layer());
	FAILED_CHECK(Add_Camera_Layer());

	Engine::Get_GraphicDev()->LoadWave(L"../sound/Logo.wav");
	Engine::Get_GraphicDev()->LoadWave(L"../sound/StageStart.wav");
	Engine::Get_GraphicDev()->LoadWave(L"../sound/Button.wav");
	Engine::Get_GraphicDev()->LoadWave(L"../sound/Gun.wav");
	Engine::Get_GraphicDev()->LoadWave(L"../sound/GameEnd.wav");

	Engine::Get_GraphicDev()->SoundPlay(SOUND_TITLE, 0); 


	return S_OK;
}

void CTitle::Update(void)
{
	Engine::CScene::Update();

	int iCnt = Engine::CResourcesMgr::GetInstance()->GetCnt();
	
	if(iCnt == 89 || GetAsyncKeyState('G'))
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);

		Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_LOGO));
		
		return ;
	}
}

void CTitle::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	Engine::CScene::Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CTitle* CTitle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTitle* pScene = new CTitle(pDevice);
	if(FAILED(pScene->InitScene()))
		Engine::Safe_Delete(pScene);

	return pScene;
}

HRESULT CTitle::Add_GameLogic_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CLoading::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Loading 생성 실패");
	pLayer->AddObject(L"Loading", pGameObject);

	
	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CTitle::Add_Camera_Layer(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pDevice);
	Engine::CGameObject*	pGameObject = NULL;

	MAPLAYER::iterator		iter = m_mapLayer.find(LAYER_GAMELOGIC);
	if(iter == m_mapLayer.end())
		return E_FAIL;

	const Engine::CComponent*		pTransform = iter->second->GetComponent(L"Loading", L"Transform");

	pGameObject = CStaticCamera::Create(m_pDevice, dynamic_cast<const Engine::CTransform*>(pTransform));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"StaticCamera 생성 실패");
	pLayer->AddObject(L"StaticCamera", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_CAMERA, pLayer));
	return S_OK;
}

void CTitle::Release(void)
{
	//m_pResourceMgr->ResetDynamic();
}

CRITICAL_SECTION CTitle::GetCrt(void)
{
	return m_Crt;
}