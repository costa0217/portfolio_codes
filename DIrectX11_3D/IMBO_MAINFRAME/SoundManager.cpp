#include "stdafx.h"
#include "SoundManager.h"

#pragma comment(lib, "../../Assets/import/FMOD/lib/fmod64_vc.lib")

System* CSoundManager::m_pSystem = nullptr;
map<string, stSoundInfo*> CSoundManager::m_mpSoundInfo;
map<string, stSoundInfo*> CSoundManager::m_mp3DSoundInfo;

void CSoundManager::Begin()
{
	m_pSystem->set3DNumListeners(1);//3d listener num

	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(
		FMOD_MAX_CHANNEL_WIDTH
		, FMOD_INIT_NORMAL
		, nullptr
	);

	Add_sound("../../Assets/Sound/game/bgm/bgm_aldenard.mp3", "bgm_aldenard", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_boss1_appear.wav", "bgm_boss1_appear", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_boss2_balcony.wav", "bgm_boss2_balcony", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_boss2_battle.mp3", "bgm_boss2_battle", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_boss2_cam.mp3", "bgm_boss2_cam", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_firsttown_battle.wav", "bgm_firsttown_battle", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_fly_loop1.mp3", "bgm_fly_loop1", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_fly_loop2.wav", "bgm_fly_loop2", FMOD_LOOP_NORMAL | FMOD_2D);
	//Add_sound("../../Assets/Sound/game/bgm/bgm_gate_loop.wav", "bgm_gate_loop", FMOD_LOOP_NORMAL | FMOD_2D);
	Add_sound("../../Assets/Sound/game/bgm/bgm_victory.wav", "bgm_victory", FMOD_LOOP_NORMAL | FMOD_2D);

	Add_3Dsound("../../Assets/Sound/game/bgm/bgm_gate_loop.wav", "bgm_gate_loop", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/bgm/bird_03.wav", "bird_03", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/bgm/bird_04.wav", "bird_04", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	//Add_3Dsound("../../Assets/Sound/bgm/cat_01", "cat_01", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	//Add_3Dsound("../../Assets/Sound/bgm/cat_02", "cat_02", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/bgm/env_bird_one.wav", "env_bird_one", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/bgm/env_bird_two.wav", "env_bird_two", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	//Add_3Dsound("../../Assets/Sound/bgm/env_bird_three", "env_bird_three", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	//Add_3Dsound("../../Assets/Sound/bgm/regina_camera_1", "regina_camera_1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	//Add_3Dsound("../../Assets/Sound/bgm/regina_camera_2", "regina_camera_2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	//Add_3Dsound("../../Assets/Sound/bgm/regina_camera_3", "regina_camera_3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//boss1
	Add_3Dsound("../../Assets/Sound/game/boss1/boss1_die.wav", "boss1_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss1/boss1_hurt.wav", "boss1_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss1/boss1_skill1.wav", "boss1_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss1/boss1_skill2.wav", "boss1_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss1/boss1_skill3.wav", "boss1_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss1/boss1_skill4.wav", "boss1_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	
	//boss2
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_die.wav", "boss2_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_hurt.wav", "boss2_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill1.wav", "boss2_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill2.wav", "boss2_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill3.wav", "boss2_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill4.wav", "boss2_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill_sound1.wav", "boss2_skill_sound1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill_sound2.wav", "boss2_skill_sound2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill_sound3.wav", "boss2_skill_sound3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/boss2/boss2_skill_sound4.wav", "boss2_skill_sound4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//ranger
	Add_3Dsound("../../Assets/Sound/game/c0_ranger/ranger_attack.wav", "ranger_attack", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c0_ranger/ranger_die.wav", "ranger_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c0_ranger/ranger_hurt.wav", "ranger_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c0_ranger/ranger_skill1.wav", "ranger_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c0_ranger/ranger_skill2.wav", "ranger_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c0_ranger/ranger_skill3.wav", "ranger_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c0_ranger/ranger_skill4.wav", "ranger_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//knight
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_die.wav", "knight_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_hurt.wav", "knight_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_skill1.wav", "knight_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_skill2.wav", "knight_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_skill3.wav", "knight_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_skill4.wav", "knight_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_attack1.wav", "knight_attack1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_attack2.wav", "knight_attack2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c1_knight/knight_attack3.wav", "knight_attack3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//dementor
	Add_3Dsound("../../Assets/Sound/game/c2_dementor/dementor_attack.wav", "dementor_attack", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c2_dementor/dementor_die.wav", "dementor_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c2_dementor/dementor_hurt.wav", "dementor_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c2_dementor/dementor_skill1.wav", "dementor_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c2_dementor/dementor_skill2.wav", "dementor_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c2_dementor/dementor_skill3.wav", "dementor_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c2_dementor/dementor_skill4.wav", "dementor_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//sister
	Add_3Dsound("../../Assets/Sound/game/c3_sister/sister_attack.wav", "sister_attack", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c3_sister/sister_die.wav", "sister_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c3_sister/sister_hurt.wav", "sister_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c3_sister/sister_skill1.wav", "sister_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c3_sister/sister_skill2.wav", "sister_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c3_sister/sister_skill3.wav", "sister_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c3_sister/sister_skill4.wav", "sister_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//wizard
	Add_3Dsound("../../Assets/Sound/game/c4_wizard/wizard_attack.wav", "wizard_attack", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c4_wizard/wizard_die.wav", "wizard_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c4_wizard/wizard_hurt.wav", "wizard_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c4_wizard/wizard_skill1.wav", "wizard_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c4_wizard/wizard_skill2.wav", "wizard_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c4_wizard/wizard_skill3.wav", "wizard_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c4_wizard/wizard_skill4.wav", "wizard_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//bard
	Add_3Dsound("../../Assets/Sound/game/c5_bard/bard_attack.wav", "bard_attack", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c5_bard/bard_die.wav", "bard_die", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c5_bard/bard_hurt.wav", "bard_hurt", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c5_bard/bard_skill1.wav", "bard_skill1", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c5_bard/bard_skill2.wav", "bard_skill2", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c5_bard/bard_skill3.wav", "bard_skill3", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
	Add_3Dsound("../../Assets/Sound/game/c5_bard/bard_skill4.wav", "bard_skill4", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

	//other masic eff
	Add_3Dsound("../../Assets/Sound/game/other_eff/magic_healing_accept.wav", "magic_healing_accept", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);

}

void CSoundManager::End()
{
	for (auto& p : m_mpSoundInfo) {
		p.second->m_pSound->release();
	}

	m_pSystem->release();
	m_pSystem->close();
}

void CSoundManager::Add_sound(string sPath, string sName, UINT flag)
{
	if (m_mpSoundInfo.end() != m_mpSoundInfo.find(sName)) {//있다면 지우고
		m_mpSoundInfo.erase(sName);//지우고
	}
	m_mpSoundInfo[sName] = new stSoundInfo;
	m_pSystem->createSound(
		sPath.c_str()
		, flag
		, nullptr
		, &m_mpSoundInfo[sName]->m_pSound
	);
}

void CSoundManager::Play_bgm(string sName)
{
	m_pSystem->playSound(m_mpSoundInfo[sName]->m_pSound
		, nullptr, false, &m_mpSoundInfo[sName]->m_pChannel);
}

void CSoundManager::Play_effect(string sName)
{
	m_pSystem->update();
	m_pSystem->playSound(m_mpSoundInfo[sName]->m_pSound
		, nullptr, false, nullptr);
}

void CSoundManager::Stop_bgm(string sName)
{
	bool bPlaying;
	m_mpSoundInfo[sName]->m_pChannel->isPlaying(&bPlaying);
	if (bPlaying)
		m_mpSoundInfo[sName]->m_pChannel->stop();
}

void CSoundManager::Add_3Dsound(string sPath, string sName, UINT flag)
{
	if (m_mp3DSoundInfo.end() != m_mp3DSoundInfo.find(sName)) {//있다면 지우고
		m_mp3DSoundInfo.erase(sName);//지우고
	}
	flag = flag | FMOD_3D;//flag에 무조건 3D flag추가
	m_mp3DSoundInfo[sName] = new stSoundInfo;
	m_pSystem->createSound(
		sPath.c_str()
		, flag
		, nullptr
		, &m_mp3DSoundInfo[sName]->m_pSound
	);
}

void CSoundManager::Play_3Dsound(string sName, int nPoints, XMFLOAT3* pPoints, float fVolume, float fMin, float fMax)
{
	FMOD_VECTOR pFmodVecPoints[20];//한번에 20개씩 재생가능
	for (int i = 0; i < nPoints; ++i) {
		pFmodVecPoints[i].x = pPoints[i].x;
		pFmodVecPoints[i].y = pPoints[i].y;
		pFmodVecPoints[i].z = pPoints[i].z;
	}
	//m_mp3DSoundInfo[sName]->m_pSound->set3DCustomRolloff(pFmodVecPoints, nPoints);
	m_mp3DSoundInfo[sName]->m_pSound->set3DMinMaxDistance(fMin, fMax);

	for (int i = 0; i < nPoints; ++i) {
		m_pSystem->update();
		m_pSystem->playSound(m_mp3DSoundInfo[sName]->m_pSound
			, nullptr, false, &m_mp3DSoundInfo[sName]->m_pChannel);
		m_mp3DSoundInfo[sName]->m_pChannel->setVolume(fVolume);
		m_mp3DSoundInfo[sName]->m_pChannel->set3DMinMaxDistance(fMin, fMax);
		m_mp3DSoundInfo[sName]->m_pChannel->set3DAttributes(&pFmodVecPoints[i], nullptr);
	}

}

void CSoundManager::Update() {
	FMOD_VECTOR poss[1];
	FMOD_VECTOR forwards[1];
	FMOD_VECTOR ups[1];

	auto pCamera = UPDATER->GetCamera();
	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, pCamera->GetPosition());
	poss[0].x = xmf3Pos.x;
	poss[0].y = xmf3Pos.y;
	poss[0].z = xmf3Pos.z;

	XMFLOAT3 xmf3Look;
	XMStoreFloat3(&xmf3Look, pCamera->GetLook());
	forwards[0].x = xmf3Look.x;
	forwards[0].y = xmf3Look.y;
	forwards[0].z = xmf3Look.z;

	XMFLOAT3 xmf3Up;
	XMStoreFloat3(&xmf3Up, pCamera->GetUp());
	ups[0].x = xmf3Up.x;
	ups[0].y = xmf3Up.y;
	ups[0].z = xmf3Up.z;

	m_pSystem->set3DListenerAttributes(0, poss, nullptr, forwards, ups);
	m_pSystem->update();
}


CSoundManager::CSoundManager()
{

	//	for (auto& p : m_mpSoundInfo) {
	//		p.second->m_pSound = nullptr;
	//	}
}


CSoundManager::~CSoundManager()
{

}