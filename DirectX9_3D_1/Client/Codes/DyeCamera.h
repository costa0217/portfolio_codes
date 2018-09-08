/*!
 * \file DyeCamera.h
 * \date 2016/01/02 19:51
 *
 * \author yoon.a.y
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef DyeCamera_h__
#define DyeCamera_h__

#include "Defines.h"
#include "Camera.h"

namespace Engine
{
	class CTransform;
}

class CDyeCamera : public Engine::CCamera
{
private:
	explicit CDyeCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDyeCamera(void);

public:
	virtual _int Update(const _float& fTimeDelta);

private:
	enum ANGLE { ANGLE_X, ANGLE_Y, ANGLE_END };

private:
	_matrix m_matWorld;				//카메라의 눈의 행렬

	_bool	m_bFixCamera;
	_bool	m_bFixEnable;

	Engine::CTransform*		m_pCameraTargetTransCom;

public:
	static CDyeCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	void CameraMove(void);

#pragma endregion

private:
	virtual HRESULT Initialize(void);
	void FixMouse(void);

public:
	virtual _ulong Release(void);
};

#endif // DynamicCamera_h__