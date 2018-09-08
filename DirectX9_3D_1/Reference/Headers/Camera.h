/*!
 * \file Camera.h
 * \date 2015/11/07 1:03
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CCamer class.
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
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);
public:
	virtual _int Update(const _float& fTimeDelta);
public:
	void Invalidate_ViewMatrix(void);
	void Invalidate_ProjMatrix(void);

	D3DXMATRIX GetView(void) { return m_matView; }
	D3DXMATRIX GetProj(void) { return m_matProj; }

protected:
	D3DXMATRIX		m_matView;
	D3DXVECTOR3		m_vEye, m_vAt, m_vUp;
protected:
	D3DXMATRIX		m_matProj;
	float			m_fFovY, m_fAspect, m_fNear, m_fFar;
public:
	virtual _ulong Release(void);
};

END

#endif // Camera_h__