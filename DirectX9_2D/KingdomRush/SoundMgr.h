#pragma once
#include "Include.h"

class CObj;
class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	System*		System;
	Sound*		Sound;
	Channel*	Channel;

public:
	void	CreateSoundDevice();
	void	LoadSound();
	void	PlaySound();
	void	ERRCHECK(FMOD_RESULT result);
	void	DestroySound();

private:
	CSoundMgr(void);
	~CSoundMgr(void);
};
