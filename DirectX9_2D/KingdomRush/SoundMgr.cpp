#include "StdAfx.h"
#include "SoundMgr.h"

CSoundMgr::CSoundMgr(void)
: System(NULL)
, Sound(NULL)
, Channel(NULL)
{
}

CSoundMgr::~CSoundMgr(void)
{
}
void	CSoundMgr::CreateSoundDevice()
{
	result	= FMOD::System_Create(&System); // create the main system object;
	ERRCHEC(result);
}
void	CSoundMgr::LoadSound()
{
}
void	CSoundMgr::PlaySound()
{
}
void	CSoundMgr::ERRCHECK(FMOD_RESULT result)
{
}
void	CSoundMgr::DestroySound()
{
}

