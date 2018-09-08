/*!
 * \file DynamicCamera.h
 * \date 2015/08/17 21:30
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

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Defines.h"
#include "Camera.h"

namespace Engine
{
	class CTimeMgr;
	class CInput;

	class CGameObject;
	class CTransform;
}

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CDynamicCamera(void);

public:
	void	SetTarget(Engine::CGameObject* pBoss, Engine::CGameObject* pPlayer) {m_pBoss = pBoss; m_pPlayer = pPlayer;}

private:
	Engine::CGameObject*		m_pBoss;
	Engine::CGameObject*		m_pPlayer;

	Engine::CTransform*			m_pPlayerTransCom;
	Engine::CTransform*			m_pBossTransCom;

public:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
	void KeyCheck(const _float& fTimeDelta);
public:
	virtual _int Update(const _float& fTimeDelta);
public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
private:
	void FixMouse(void);
private:
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInput*			m_pInput;
private:
	float					m_fCamSpeed;
	bool					m_bMouseFix;
	bool					m_bClick;

	_float					m_fAccTime;
public:
	virtual _ulong Release(void);
};

#endif // DynamicCamera_h__