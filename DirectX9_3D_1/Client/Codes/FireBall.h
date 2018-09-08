/*!
 * \file FireBall.h
 * \date 2015/12/16 17:00
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef FireBall_h__
#define FireBall_h__

#include "Effect.h"

class CFireBall : public CEffect
{
protected:
	explicit CFireBall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireBall(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos);

private:
	virtual HRESULT Initialize(_vec3 vCreatePos);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};
#endif // FireBall_h__