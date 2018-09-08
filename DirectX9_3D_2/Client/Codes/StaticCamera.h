#ifndef StaticCamera_h__
#define StaticCamera_h__

namespace Engine
{
	class CTimeMgr;
	class CInput;

	class CTransform;
}

#include "Defines.h"
#include "Camera.h"

class CStaticCamera
	: public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pDevice, Engine::CGameObject* pPlayer);
	virtual ~CStaticCamera(void);

public:
	void	SetBoss(Engine::CGameObject* pBoss) {m_pBoss = pBoss;}

private:
	Engine::CGameObject*		m_pBoss;

public:
	void SetCameraTarget(Engine::CTransform* pTargetInfo);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);

public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pDevice, Engine::CGameObject* pPlayer
		/*, const Engine::CTransform* pTargetInfo*/);

private:
	void	KeyCheck(const _float& fTimeDelta);
	void	TargetRenewal(void);

	void	FixMouse(void);
	POINT	GetMouse(void);

	void	Zoom_InOut(void);

private:
	Engine::CTimeMgr*		m_pTimeMgr;

private:
	Engine::CTransform*		m_pTargetInfo;

	Engine::CGameObject*	m_pPlayer;

private:
	float		m_fTargetDistance;
	float		m_fAngle;
	float		m_fCamSpeed;

	
	_vec3		m_vCameraDirX;
	_vec3		m_vCameraDirY;
	_vec3		m_vResultCamDir;

	POINT		m_ClickMousePoint;
	_bool		m_bClickMouse;
	_float		m_fRotScreenTime;

	_bool		m_bDirChange;

	_float		m_fSaveDist;
	_vec3		m_vShakeAt;
	_float		m_fTimeDelta;
	_float		m_fZoomTime;
	_bool		m_bFstLoop;

	_bool		m_bKeyPress;


	_vec3		m_vSaveAt;
public:
	virtual _ulong Release(void);
};


#endif // StaticCamera_h__
