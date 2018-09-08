/*!
 * \file StaticCamera.h
 * \date 2015/07/10 12:39
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
#ifndef StaticCamera_h__
#define StaticCamera_h__

namespace Engine
{
	class CTimeMgr;
	class CInfoSubject;

	class CTransform;
}

#include "Camera.h"

class CStaticCamera
	: public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStaticCamera(void);

public:
	void SetCameraTarget(const Engine::CTransform* pTargetInfo);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);

public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pDevice
		, const Engine::CTransform* pTargetInfo);

private:
	void KeyCheck(void);
	void TargetRenewal(void);
public:
	void SetTargetInfo(const Engine::CTransform* pTargetInfo){m_pTargetInfo = pTargetInfo;}
	void SetTargetDistance(float fTargetDistance){m_fTargetDistance = fTargetDistance;}
	void SetAngleRight(float fAngleRight){m_fAngleRight = fAngleRight;}
	void SetAngleUp(float fAngleUp){m_fAngleUp = fAngleUp;}
	void SetbBombMove(bool bBombMove){m_bBombMove = bBombMove;}
	void SetbBombCreate(bool bBombCreate){m_bBombCreate = bBombCreate;}
	bool GetbBombCreate(){return m_bBombCreate;}
	bool BombMove();

	void SetbEyeFix(bool bEyeFix){m_bEyeFix = bEyeFix;}
	void SetbKeyCheck(bool bKeyCheck){m_bKeyCheck = bKeyCheck;}
	void SetbGameStart(bool bGameStart){m_bGameStart = bGameStart;}

private:
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;

private:
	const Engine::CTransform*		m_pTargetInfo;

private:
	float		m_fTargetDistance;
	float		m_fAngleRight;
	float		m_fAngleUp;
	float		m_fCamSpeed;
	bool		m_bBombMove;
	bool        m_bBombCreate;
	bool		m_bBombSecondMove;

	bool		m_bEyeFix;
	bool        m_bKeyCheck;
	

	bool		m_bGameStart;
};

#endif // StaticCamera_h__