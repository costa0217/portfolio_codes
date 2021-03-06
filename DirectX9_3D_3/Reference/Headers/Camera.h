/*!
 * \file Camera.h
 * \date 2015/07/10 12:29
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

#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera
	: public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCamera(void);

public:
	void SetViewSpaceMatrix(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp);
	void SetProjectionMatrix(const float& fFovY, const float& fAspece
		, const float& fNear, const float& fFar);
	void SetOrthogonalMatrix(const float& fFovY, const float& fAspece
		, const float& fNear, const float& fFar);

protected:
	D3DXMATRIX		m_matView, m_matProj, m_matOrtho;
	D3DXVECTOR3		m_vEye, m_vAt, m_vUp;
};

END

#endif // Camera_h__