#pragma once
#include "../../Assets/import/FMOD/include/fmod.hpp"

/*
add, use ex)
CSoundManager::Add_sound("../../Assets/Sound/Electroman-adventures.mp3", "test_bgm", FMOD_LOOP_NORMAL | FMOD_2D);
CSoundManager::Add_3Dsound("../../Assets/Sound/bullet sound.mp3", "test_3d", FMOD_LOOP_OFF | FMOD_3D_LINEARROLLOFF);
if (fProgress > 1) {
	fProgress = 0.f;
	XMFLOAT3 pPoints[] = { XMFLOAT3(256,0,256), XMFLOAT3(0,0,0), XMFLOAT3(512,0,0), XMFLOAT3(0,0,512), XMFLOAT3(512,0,512) };
	CSoundManager::Play_3Dsound("test_3d", 5, pPoints, 1.f, 10.f, 150.f);
}
*/
/*
flag ex)
FMOD_DEFAULT | FMOD_LOOP_OFF
FMOD_LOOP_NORMAL | FMOD_2D
FMOD_DEFAULT | FMOD_LOOP_OFF
*/
using namespace FMOD;

struct stSoundInfo {
	Sound* m_pSound;
	Channel* m_pChannel;
};
class CSoundManager {
public:
	static void Begin();
	static void End();

	static void Add_sound(string sPath, string sName, UINT flag);
	static void Play_bgm(string sName);
	static void Play_effect(string sName);
	static void Stop_bgm(string sName);

	static void Add_3Dsound(string sPath, string sName, UINT flag);
	static void Play_3Dsound(string sName, int nPoints, XMFLOAT3* pPoints, float fVolume = 5.f, float fMin = 10.f, float fMax = 1000.f);
	static void Update();

	//static void SetListenerNum(int nListener) { m_nListener = nListener; }
private:
	static System* m_pSystem;
	static map<string, stSoundInfo*> m_mpSoundInfo;
	static map<string, stSoundInfo*> m_mp3DSoundInfo;

	//static int m_nListener;
public:
	CSoundManager();
	~CSoundManager();

};