#include "StdAfx.h"
#include "Player.h"

CPlayer::CPlayer(void)
:m_pStageBar(NULL)
{
}

CPlayer::~CPlayer(void)
{
	Release();
}
HRESULT CPlayer::Initialize(void)
{
	m_iLife		= 20;
	m_iWave		= 0;
	m_iGold		= 10000;
	m_iMaxWave	= 0;
	return S_OK;
}
int CPlayer::Progress(void)
{
	return 0;
}
void CPlayer::Render(void)
{
}
void CPlayer::Release(void)
{
}
