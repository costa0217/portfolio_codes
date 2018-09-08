/*!
 * \file BreathEffect.h
 * \date 2015/12/29 21 : 04
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

#ifndef BreathEffect_h__
#define BreathEffect_h__

#include "Effect.h"

class CBreathEffect : public CEffect
{
protected:
	explicit CBreathEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBreathEffect(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos);
	static _vec3	m_vDir;
	static void SetDir(_vec3 vDir);
private:
	_float m_fSize;

private:
	virtual HRESULT Initialize(_vec3 vCreatePos);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};
#endif // BreathEffect_h__