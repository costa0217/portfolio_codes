#include "StdAfx.h"
#include "MainGame.h"

#include "SceneMgr.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "RenderMgr.h"

#include "SDevice.h"

CMainGame::CMainGame(void)
: m_pDevice(CDevice::GetInstance())
, m_fTime(0.f)
, m_iCount(0)
{
}

CMainGame::~CMainGame(void)
{
	Release();
}
HRESULT	CMainGame::Initialize(void)
{
	if(FAILED(m_pDevice->InitDevice()))
	{
		ERR_MSG(L"Client Device Create Failed");
		return E_FAIL;
	}
	//ImageRoad();		//이미지 불러오기
	SoundRoad();

	CSceneMgr::GetInstance()->Initialize(SC_TITLE);
	CTimeMgr::GetInstance()->InitTime();

	ZeroMemory(m_szFPS,sizeof(m_szFPS));

	return S_OK;
}
void CMainGame::Progress(void)
{
	CTimeMgr::GetInstance()->SetTime();
	CSceneMgr::GetInstance()->Progress();
	CKeyMgr::GetInstance()->KeyCheck();

	D3DXMatrixTranslation(&m_matTrans, 500.f, 50.f, 0.f);
}
void CMainGame::Render(void)
{
	/*++m_iCount;

	m_fTime += CTimeMgr::GetInstance()->GetTime();

	if(m_fTime > 1.f)
	{
		m_fTime = 0.f;
		wsprintf(m_szFPS, L"FPS : %d", m_iCount);
		m_iCount = 0;
	}*/

	m_pDevice->GetDevice()->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	m_pDevice->Render_Begin();

	CSceneMgr::GetInstance()->Render();
	/*m_pDevice->GetSprite()->SetTransform(&m_matTrans);
	m_pDevice->GetFont()->DrawTextW(m_pDevice->GetSprite(), m_szFPS, lstrlen(m_szFPS), 
		NULL, NULL, D3DCOLOR_ARGB(255, 255, 0, 0));*/

	m_pDevice->Render_End();
	m_pDevice->GetDevice()->Present(NULL, NULL, NULL, NULL);

}
void CMainGame::Release(void)
{
	
	CRenderMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CSDevice::GetInstance()->DestroyInstance();
	m_pDevice->DestroyInstance();
}
void CMainGame::ImageRoad()
{
	
}
void CMainGame::SoundRoad()
{
	CSDevice::GetInstance()->Init();
	CSDevice::GetInstance()->LoadWave(L"../Sound/BGM/Title.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BGM/Field.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BGM/Stage1Wait.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BGM/Stage1Wave.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BGM/Stage2Wait.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BGM/Stage2Wave.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/BGM/BossBgm.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_GUIMouseOverTowerIcon.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_GUIButtonCommon.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_NextWaveReady.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_WaveIncoming.wav");


	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_TowerBuilding.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_TowerSell.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_TowerUpgrade.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_RallyPointPlaced.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Archer_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Archer_Taunt1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Archer_Taunt2.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Barrack_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Barrack_Taunt1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Barrack_Taunt2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Barrack_Move.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Mage_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Mage_Taunt1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Mage_Taunt2.wav");

	
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Artillery_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Artillery_Taunt1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Artillery_Taunt2.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Paladin_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Paladin_Move.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Paladin_Taunt2.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Barbarian_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Barbarian_Move.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Barbarian_Taunt1.wav");

	
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Ranger_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Ranger_Taunt1.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Muskateer_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Muskateer_Snipe.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Arcane_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Arcane_Taunt1.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sorcerer_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sorcerer_Taunt2.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/BFG_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/BFG_Taunt2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Tesla_Ready.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Tesla_Taunt1.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_ArrowHit2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_ArrowHit3.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_ArrowRelease3.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_Bomb1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_Tesla_attack_1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_RocketLaunt.wav");
	

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_Sniper.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_SoldiersFighting.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_MageShot.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_RayArcane.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_Sorcerer.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_SpellTowerHold_Dissipate.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_FireballHit.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Reinforcements_Event1.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Reinforcements_Event2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Reinforcements_Event3.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Reinforcements_Event4.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_Shrapnel.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_Teleport.wav");	

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_Thorn.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_HumanDead3.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyGoblinDead.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyBigDead.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyTrollDead.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyExplode1.wav"); // 거미 가고일 제트맨
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyPuffDead.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_HitIce3.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/KR_sfx_thor_lanzamartillo_op2.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyJtEat.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyJtDeath.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_EnemyJtExplode.wav");
	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/lordblackburn_specialstomp.wav");

	CSDevice::GetInstance()->LoadWave(L"../Sound/Effect/Sound_QuestCompleted.wav");

	

}

