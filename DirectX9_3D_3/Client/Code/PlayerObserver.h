/*!
 * \file PlayerObserver.h
 * \date 2015/07/30 13:53
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef PlayerObserver_h__
#define PlayerObserver_h__

#include "Observer.h"
#include "Include.h"

namespace Engine
{
	class CInfoSubject;
}

class CPlayerObserver
	: public Engine::CObserver
{
private:
	CPlayerObserver(void);

public:
	virtual ~CPlayerObserver(void);

public:
	D3DXVECTOR3* GetPos(void);
	D3DXVECTOR3* GetDir(void);
	float GetHp(void);
	float GetBullet(void);
	OBJ_STATE GetState(void);
	D3DXMATRIX* GetWorld(void);

	int	GetMAXBoardNum();
	int GetBoardNum();

public:
	virtual void Update(int message);

public:
	static CPlayerObserver* Create(void);

private:
	Engine::CInfoSubject*		m_pInfoSubject;

private:
	D3DXVECTOR3		m_Pos;
	D3DXVECTOR3		m_Dir;
	float			m_fHp;
	float			m_fBullet;
	OBJ_STATE		m_eState;
	D3DXMATRIX		m_World;

	int				m_iBoardNum;
	int				m_iMAXBoardNum;
};

#endif // PlayerObserver_h__