#include "StdAfx.h"
#include "MainGame.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "RenderMgr.h"
#include "ObjMgr.h"
#include "Device.h"

CMainGame::CMainGame(void)
{
}

CMainGame::~CMainGame(void)
{
	Release();
}

void CMainGame::Initialize(void)
{





	m_hdc = GetDC(g_hWnd);
	srand((unsigned)time(NULL));

	CDevice::GetInstance()->Init();
	

	CDevice::GetInstance()->LoadWave(L"../Sound/Start.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/Click.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/Bgm.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/Bullet.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/BulletEnd.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/213_Isaac_Hurt_Grunt2.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/Close.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/Open.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/274_bum shoot off.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/FireEnd.wav");

	CDevice::GetInstance()->LoadWave(L"../Sound/40_Scared_Whimper_1.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/66_Powerup1.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/87_Monster_Grunt_4_B.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/115_miniboss.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/120_Meaty_Deaths_2.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/176_LockBreak2.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/212_isaacbosswin.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/219_Hellboss_Groundpound_0.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/256_Death_Burst_Small_1.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/259_Death_Burst_Large_0.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/261_Cute_Grunt_1.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/284_boss1_explosions2.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/297_bloodshoot0.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/BossDoor.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/BossStart.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/fly.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/ItemEat.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/Monster1.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/Monster3.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/307_beep.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/59_Rock_crumble 0.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/124_meatheadshoot1.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/314_1up.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/335_Whoosh_high1.L.wav");

	CDevice::GetInstance()->LoadWave(L"../Sound/97_Mom_Vox_Isaac_2.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/113_Mom_Vox_Death_1.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/38_secret room find.wav");
	CDevice::GetInstance()->LoadWave(L"../Sound/291_Boss_Gurgle_Roar.wav");

	CDevice::GetInstance()->LoadWave(L"../Sound/BossWing.wav");

	CSceneMgr::GetInst()->SetScene(SC_MENU);

}
void CMainGame::Progress(void)
{
	CKeyMgr::GetInst()->KeyCheck();
	CSceneMgr::GetInst()->Progress();
}
void CMainGame::Render(void)
{
	CSceneMgr::GetInst()->Render(m_hdc);
}
void CMainGame::Release(void)
{
	ReleaseDC(g_hWnd, m_hdc);

	CDevice::GetInstance()->DestroyInst();
	CRenderMgr::GetInst()->DestroyInst();
	CSceneMgr::GetInst()->DestroyInst();
	CKeyMgr::GetInst()->DestroyInst();
	CObjMgr::GetInst()->DestroyInst();
}
