/*!
 * \file Transform.h
 * \date 2015/07/09 12:50
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

#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform
	: public CComponent
{
private:
	explicit CTransform(const D3DXVECTOR3& vLook);

public:
	virtual ~CTransform(void);

public:
	virtual void Update(void);
public:
	void SetPos(D3DXVECTOR3 vPos){m_vPos = vPos;}
	void SetHP(float fHP){m_fHP = fHP;}

public:
	static CTransform* Create(const D3DXVECTOR3& vLook);

public:
	float			m_fAngleNo[ANGLE_END];
	float			m_fAngle[ANGLE_END];
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vScale;
	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matWorldAng;
	D3DXMATRIX		m_matWorldNoSca;


	float			m_fHP;

	OBJ_NAME		m_eName;
	OBJ_STATE		m_eState;

public:
	void SetScale(D3DXVECTOR3 vScale){m_vScale = vScale;}
};

END

#endif // Transform_h__