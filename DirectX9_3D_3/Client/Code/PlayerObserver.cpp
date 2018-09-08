#include "stdafx.h"
#include "PlayerObserver.h"

#include "Export_Function.h"
#include "Include.h"

CPlayerObserver::CPlayerObserver(void)
: m_pInfoSubject(Engine::Get_InfoSubject())
{

}

CPlayerObserver::~CPlayerObserver(void)
{

}

D3DXVECTOR3* CPlayerObserver::GetPos(void)
{
	return &m_Pos; 
}

D3DXVECTOR3* CPlayerObserver::GetDir(void)
{
	return &m_Dir; 
}

float CPlayerObserver::GetHp(void)
{
	return m_fHp; 
}

float CPlayerObserver::GetBullet(void)
{
	return m_fBullet; 
}

OBJ_STATE CPlayerObserver::GetState(void)
{
	return m_eState;
}

D3DXMATRIX* CPlayerObserver::GetWorld(void)
{
	return &m_World;
}

void CPlayerObserver::Update(int message)
{
	Engine::DATALIST*		pDatalist = m_pInfoSubject->GetDatalist(message);

	NULL_CHECK(pDatalist);

	switch(message)
	{
	case PLAYER_POS:
		m_Pos = *((D3DXVECTOR3*)pDatalist->front());
		break;

	case PLAYER_DIR:
		m_Dir = *((D3DXVECTOR3*)pDatalist->front());
		break;

	case PLAYER_HP:
		m_fHp = *((float*)pDatalist->front());
		break;

	case PLAYER_BULLET:
		m_fBullet = *((float*)pDatalist->front());
		break;

	case PLAYER_STATE:
		m_eState = *((OBJ_STATE*)pDatalist->front());
		break;

	case PLAYER_WORLD:
		m_World = *((D3DXMATRIX*)pDatalist->front());
		break;

	case PLAYER_BOARDNUM:
		m_iBoardNum = *((int*)pDatalist->front());
		break;

	case PLAYER_MAXBOARDNUM:
		m_iMAXBoardNum = *((int*)pDatalist->front());
		break;
	}
}

CPlayerObserver* CPlayerObserver::Create(void)
{
	return new CPlayerObserver;
}
int CPlayerObserver::GetBoardNum()
{
	return m_iBoardNum;
}

int CPlayerObserver::GetMAXBoardNum()
{
	return m_iMAXBoardNum;
}
