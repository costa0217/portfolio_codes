#include "stdafx.h"
#include "SoundMgr.h"

#include "Export_Function.h"
#include "Player.h"
#include "DynamicCamera.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{

}

CSoundMgr::~CSoundMgr()
{

}

void CSoundMgr::Initialize(void)
{
	//FMOD System Create
	Result = System_Create(&m_pfmod);
	ErrorCheck(Result);

	//Current FMOD version Check
	_uint uiFMODVersion;
	Result = m_pfmod->getVersion(&uiFMODVersion);
	ErrorCheck(Result);

	if(uiFMODVersion < FMOD_VERSION)
	{
		MSG_BOX("FMOD_VERSION is Low Failed");
	}

	/*Result = m_pfmod->set3DSettings(1.f, 1.f, 1.f);
	ErrorCheck(Result);*/

	Result = m_pfmod->init(200, FMOD_INIT_3D_RIGHTHANDED, 0);
	ErrorCheck(Result);

	//마스터 채널 생성
	m_pfmod->createChannelGroup("MasterChannel", &m_pMasterChannel);
	m_pfmod->createChannelGroup("EnvironmentChannel", &m_pEnvironmentChannel);
	m_pfmod->createChannelGroup("PlayerChannel", &m_pPlayerChannel);

	//에코 생성
	m_pdspecho = 0;
	Result = m_pfmod->createDSPByType(FMOD_DSP_TYPE_ECHO, &m_pdspecho);
	ErrorCheck(Result);

	Result = m_pPlayerChannel->addDSP(0, m_pdspecho);
	ErrorCheck(Result);

	m_pMasterChannel->set3DConeSettings(230.f, 360.f, 0.5f);
	m_pMasterChannel->set3DDopplerLevel(5.f);

	Result = m_pdspecho->setBypass(false);
	ErrorCheck(Result);

	//m_pdspecho->setParameterFloat(FMOD_DSP_ECHO_DELAY, 500.0f);	//몇초에 한번
	m_pdspecho->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 50.0f);	//얼마나 돌아오게?
	m_pdspecho->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL, 10.0f);	//원본 소리 크기
	m_pdspecho->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, -10.0f);	//에코 소리 크기
}

void CSoundMgr::SoundLoading(void)
{
	//createSound : 효과음(여러개 동시에 재생 가능)
	//createStream : 배경음악(하나만 지속적으로 재생)


	//BGM
	Result = m_pfmod->createStream("../Bin/Resources/Sound/BGM/bgm_mboss_vampire.wav", FMOD_2D | FMOD_LOOP_NORMAL, 0, &m_pBGMSound[SOUND::BGM_BloodLord]);
	ErrorCheck(Result);

	Result = m_pfmod->createStream("../Bin/Resources/Sound/BGM/bgm_morvan_inn.mp3", FMOD_2D | FMOD_LOOP_NORMAL, 0, &m_pBGMSound[SOUND::BGM_StartScene]);
	ErrorCheck(Result);

	Result = m_pfmod->createStream("../Bin/Resources/Sound/BGM/bgm_s3_meer.wav", FMOD_2D | FMOD_LOOP_NORMAL, 0, &m_pBGMSound[SOUND::BGM_MiddleStage]);
	ErrorCheck(Result);

	//EFF
	//Player
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_04]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_05.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_05]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_06.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_06]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_07.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_07]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_long_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_long_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_long_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_long_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_long_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_long_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_long_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_long_04]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_blunt_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_blunt_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_blunt_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_blunt_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_blunt_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_blunt_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_blunt_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_blunt_04]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_execution_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_execution_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_execution_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_execution_02]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_hell_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_hell_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_hell_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_hell_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_hell_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_hell_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_hell_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_hell_04]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_hell_finish_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_hell_finish_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_hell_finish_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_hell_finish_02]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_parrying_cast_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_parrying_cast_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_parrying_cast_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_parrying_cast_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_parrying_cast_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_parrying_cast_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_parrying_cast_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_parrying_cast_04]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_parrying_cast_05.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_parrying_cast_05]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_piercing_last_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_piercing_last_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_piercing_last_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_piercing_last_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_piercing_last_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_piercing_last_03]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong1_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong1_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong1_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong1_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong1_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong1_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong1_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong1_04]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong2_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong2_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong2_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong2_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong2_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong2_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_skill_swingstrong2_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_skill_swingstrong2_04]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_04]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_05.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_05]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_06.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_06]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_07.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_07]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_08.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_08]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_09.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_09]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_attack_strong_10.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_attack_strong_10]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_medium_1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_medium_1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_medium_2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_medium_2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_medium_3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_medium_3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_medium_4.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_medium_4]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_medium_5.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_medium_5]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_medium_6.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_medium_6]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_medium_7.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_medium_7]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_04]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_05.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_05]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_06.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_06]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_07.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_07]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_strong_08.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_strong_08]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_weak_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_weak_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_weak_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_weak_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_weak_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_weak_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_weak_04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_weak_04]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_weak_05.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_weak_05]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/hurk_hurt_weak_06.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::hurk_hurt_weak_06]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/step_lv3.wav", FMOD_3D_LINEARROLLOFF, 0, &m_pEFFSound[SOUND::step_lv3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/step_lv4.wav", FMOD_3D_LINEARROLLOFF, 0, &m_pEFFSound[SOUND::step_lv4]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_01.wav", FMOD_3D_LINEARROLLOFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_AttackBreak_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_AttackBreak_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Blow_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Blow_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Execution_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Execution_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Friction_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Friction_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Hell_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Hell_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_KnockBack_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_KnockBack_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Parrying_Blunt_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Parrying_Blunt_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Parrying_Metal_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Parrying_Metal_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Piercing_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Piercing_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_SmashCombo_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_SmashCombo_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Strong_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Strong_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/Hurk_Swing_GreatSword_Strong_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hurk_Swing_GreatSword_Strong_02]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/material_dirt_step1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::material_dirt_step1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/material_dirt_step2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::material_dirt_step2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/material_dirt_step3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::material_dirt_step3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/material_dirt_step4.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::material_dirt_step4]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Player/heart_beat.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::heart_beat]);
	ErrorCheck(Result);

	//BloodLord
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_attack01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_attack01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_attack02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_attack02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_attack03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_attack03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_bone01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_bone01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_bone02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_bone02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_bone03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_bone03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_bone04.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_bone04]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_breath01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_breath01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_breath02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_breath02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_breath03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_breath03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_dominate.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_dominate]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_dying.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_dying]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_dying2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_dying2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_firebreath.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_firebreath]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_low_health.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_low_health]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_roar.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_roar]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_spawn.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_spawn]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_threat.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_threat]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_warp_begin.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_warp_begin]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/lord_warp_end.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::lord_warp_end]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/BloodLord/explosion_lv4.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::explosion_lv4]);
	ErrorCheck(Result);

	//Vampire
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/action_landing_higher.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::action_landing_higher]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/Bear_Step.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Bear_Step]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/effect_fireball_appear.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::effect_fireball_appear]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/effect_lightning_charge.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::effect_lightning_charge]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/effect_lightning_explosion.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::effect_lightning_explosion]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/effect_lightning_shock.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::effect_lightning_shock]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_attack_drag.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_attack_drag]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_breath1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_breath1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_breath2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_breath2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_breath3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_breath3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_dying.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_dying]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_fireblade_begin.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_fireblade_begin]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_fireblade_during.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_fireblade_during]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_fireblade_end.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_fireblade_end]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_fireblade_original.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_fireblade_original]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_hurt1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_hurt1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_hurt2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_hurt2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_hurt3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_hurt3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_laugh1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_laugh1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_laugh2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_laugh2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_laugh3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_laugh3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_pierceaura_begin.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_pierceaura_begin]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_pierceaura_end.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_pierceaura_end]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_rapier_pierce.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_rapier_pierce]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_rapier_vanguard.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_rapier_vanguard]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_shout_long1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_shout_long1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_shout_long2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_shout_long2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_shout_long3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_shout_long3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_shout1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_shout1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_shout2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_shout2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_shout3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_shout3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_standup1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_standup1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_standup2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_standup2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_step1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_step1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_step2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_step2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_step3.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_step3]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/vampire_step4.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::vampire_step4]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/Swing_Metal.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Swing_Metal]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Vampire/Swing_MetalStrong.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Swing_MetalStrong]);
	ErrorCheck(Result);



	//Hit
	//Player
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Metal_Slash.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Metal_Slash]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Metal_Slash_adult.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Metal_Slash_adult]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Metal_Stab.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Metal_Stab]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Metal_Stab_adult.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Metal_Stab_adult]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Metal_StrongSlash.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Metal_StrongSlash]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Metal_StrongSlash_adult.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Metal_StrongSlash_adult]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Flesh_Blunt.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Flesh_Blunt]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Player/Hit_Flesh_Blunt_adult.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Flesh_Blunt_adult]);
	ErrorCheck(Result);

	//Monster
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Monster/Hit_Flesh_Slash.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Flesh_Slash]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Monster/Hit_Flesh_Slash_adult.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Flesh_Slash_adult]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Monster/Hit_Flesh_Stab.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Flesh_Stab]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Monster/Hit_Flesh_Stab_adult.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_Flesh_Stab_adult]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Monster/Hit_HardFlesh_Slash.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_HardFlesh_Slash]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Monster/Hit_HardFlesh_Slash_adult.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_HardFlesh_Slash_adult]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Hit/Monster/Hit_HardFlesh_Slash2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Hit_HardFlesh_Slash2]);
	ErrorCheck(Result);

	//오브젝트, 이펙트
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Common/magic_fire_shock_during_1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL, 0, &m_pEFFSound[SOUND::BurningFire]);
	ErrorCheck(Result);

	Result = m_pfmod->createSound("../Bin/Resources/Sound/Common/octopus_drill_wood_01.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::octopus_drill_wood_01]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Common/octopus_drill_wood_02.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::octopus_drill_wood_02]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Common/octopus_drill_wood_03.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::octopus_drill_wood_03]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Common/Click.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::Click]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Common/material_rock_break.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::material_rock_break]);
	ErrorCheck(Result);

	// Jelly
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_attack_explosion.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_attack_explosion]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_attack1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_attack1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_damage_strong1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_damage_strong1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_damage_strong2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_damage_strong2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_down.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_down]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_idle.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_idle]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_pierce.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_pierce]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_walk1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_walk1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_walk2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_walk2]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_weak1.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_weak1]);
	ErrorCheck(Result);
	Result = m_pfmod->createSound("../Bin/Resources/Sound/Jelly/jelly_weak2.wav", FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF, 0, &m_pEFFSound[SOUND::jelly_weak2]);
	ErrorCheck(Result);

	for(int i = 0; i < SOUND::EFF_SoundEnd; ++i)
	{
		if(i == SOUND::BurningFire)
			m_pEFFSound[i]->set3DMinMaxDistance(0.0f, 120.f);
		else if(i >= SOUND::lord_attack01 && i <= SOUND::lord_warp_end)
			m_pEFFSound[i]->set3DMinMaxDistance(0.0f, 150.f);
		else
			m_pEFFSound[i]->set3DMinMaxDistance(0.0f, 200.f);
	}
}

void CSoundMgr::PlayBGM(SOUND::BGM eType, _float fVolume /*=1.f*/)
{
	/*_bool bPlay;
	m_pBGMChannel->isPlaying(&bPlay);

	if(bPlay)
		m_pBGMChannel->stop();*/

	m_pfmod->playSound(m_pBGMSound[eType], 0, false, &m_pBGMChannel);
	m_pBGMChannel->setVolume(fVolume);
}

void CSoundMgr::StopBGM(void)
{
	_bool bPlay;
	m_pBGMChannel->isPlaying(&bPlay);

	if(bPlay)
		m_pBGMChannel->stop();
}

void CSoundMgr::PlayVolumeBGM(_float fVolume /*= 1.f*/)
{
	_bool bPlay;
	m_pBGMChannel->isPlaying(&bPlay);

	if(bPlay)
		m_pBGMChannel->setVolume(fVolume);
}

void CSoundMgr::PlayEffect(SOUND::EFF eType, _vec3 vBirthPos, _float fVolume /*= 1.f*/)
{
	if(eType == SOUND::BurningFire)
		m_pfmod->playSound(m_pEFFSound[eType], m_pEnvironmentChannel, false, &m_pEFFChannel);
	else if(eType >= SOUND::hurk_attack_01 && eType <= SOUND::heart_beat)
		m_pfmod->playSound(m_pEFFSound[eType], m_pPlayerChannel, false, &m_pEFFChannel);
	else
		m_pfmod->playSound(m_pEFFSound[eType], m_pMasterChannel, false, &m_pEFFChannel);

	FMOD_VECTOR Pos;
	if(vBirthPos == _vec3(0.f, 0.f, 0.f))
	{
		Pos.x = CPlayer::GetInstance()->Get_TransCom()->m_vPosition.x;
		Pos.y = CPlayer::GetInstance()->Get_TransCom()->m_vPosition.y;
		Pos.z = CPlayer::GetInstance()->Get_TransCom()->m_vPosition.z;
	}
	else
	{
		Pos.x = vBirthPos.x;
		Pos.y = vBirthPos.y;
		Pos.z = vBirthPos.z;
	}

	FMOD_VECTOR vel = {  0.0f, 1.0f, 0.0f };
	m_pEFFChannel->set3DAttributes(&Pos, NULL);
	m_pEFFChannel->setVolume(fVolume);
	Update(1.f);
}

void CSoundMgr::EnvironmentStopEFF(void)
{
	_bool bPlay;
	m_pEnvironmentChannel->isPlaying(&bPlay);

	if(bPlay)
		m_pEnvironmentChannel->stop();
}

void CSoundMgr::Update(const _float& fTimeDelta)
{
	_vec3 vPlayerPos(CPlayer::GetInstance()->Get_TransCom()->m_vPosition.x
		, CPlayer::GetInstance()->Get_TransCom()->m_vPosition.y
		, CPlayer::GetInstance()->Get_TransCom()->m_vPosition.z);

	_vec3 vCameraPos(CDynamicCamera::GetInstance()->GetEye().x
		, CDynamicCamera::GetInstance()->GetEye().y
		, CDynamicCamera::GetInstance()->GetEye().z);

	_vec3 vForwardDir = vCameraPos - vPlayerPos;
	vForwardDir.y = 0.f;
	D3DXVec3Normalize(&vForwardDir, &vForwardDir);

	FMOD_VECTOR listenerForward	= { vForwardDir.x, vForwardDir.y, vForwardDir.z };
	FMOD_VECTOR listenerPos		= { vCameraPos.x, vCameraPos.y, vCameraPos.z };
	FMOD_VECTOR listenerUp		= { 0.f, 1.f, 0.f };

	m_pMasterChannel->set3DConeOrientation(&listenerForward);
	m_pPlayerChannel->set3DConeOrientation(&listenerForward);
	m_pEnvironmentChannel->set3DConeOrientation(&listenerForward);

	m_pfmod->set3DListenerAttributes(0, &listenerPos, NULL, &listenerForward, &listenerUp);
	m_pfmod->update();
}

void CSoundMgr::ErrorCheck(FMOD_RESULT _r)
{
	if(_r != FMOD_OK)
	{
		MSG_BOX("SoundMgr Loading Failed");
	}
}

void CSoundMgr::Release(void)
{
	for(int i = 0; i < SOUND::BGM_SoundEnd; ++i)
	{
		m_pBGMSound[i]->release();
	}

	for(int i = 0; i < SOUND::EFF_SoundEnd; ++i)
	{
		m_pEFFSound[i]->release();
	}

	m_pPlayerChannel->removeDSP(m_pdspecho);

	m_pPlayerChannel->release();
	m_pMasterChannel->release();
	m_pEnvironmentChannel->release();

	m_pdspecho->release();
	m_pfmod->close();
	m_pfmod->release();
}