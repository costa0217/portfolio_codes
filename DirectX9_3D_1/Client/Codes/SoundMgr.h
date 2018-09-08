#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Defines.h"

#include "fmod.hpp"

using namespace FMOD;

class CSoundMgr
{
public:
	DECLARE_SINGLETON(CSoundMgr)

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(void);
	void SoundLoading(void);
	void PlayBGM(SOUND::BGM eType, _float fVolume = 1.f);
	void PlayVolumeBGM(_float fVolume = 1.f);
	void StopBGM(void);
	void EnvironmentStopEFF(void);
	void PlayEffect(SOUND::EFF eType, _vec3 vBirthPos = _vec3(0.f, 0.f, 0.f), _float fVolume = 1.f);

	void Update(const _float& fTimeDelta);

public: //getter setter
	void SetEnvironmentChannel(_bool SetBool) { m_pEnvironmentChannel->setPaused(SetBool); }

private:
	System*			m_pfmod;
	Channel*		m_pBGMChannel;
	Channel*		m_pEFFChannel;
	ChannelGroup*	m_pMasterChannel;
	ChannelGroup*	m_pEnvironmentChannel;
	ChannelGroup*	m_pPlayerChannel;

	DSP*			m_pdspecho;
	
	Sound*			m_pBGMSound[SOUND::BGM_SoundEnd];
	Sound*			m_pEFFSound[SOUND::EFF_SoundEnd];

	FMOD_RESULT		Result;

private:
	void ErrorCheck(FMOD_RESULT _r);

public:
	void Release(void);
};
#endif // SoundMgr_h__
