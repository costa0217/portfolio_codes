/*!
 * \file MeteoEffect.h
 * \date 2015/12/29 21 : 02
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

#ifndef MeteoEffect_h__
#define MeteoEffect_h__

#include "Effect.h"

class CMeteoEffect : public CEffect
{
protected:
	explicit CMeteoEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeteoEffect(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos);
	static _float	m_fSize;
	static void		SetSize(_float fSize) { m_fSize = fSize; }

private:
	_bool	m_bPlay;
	_bool	m_bPlaySound;
	_float  m_fPlayTime;
private:
	virtual HRESULT Initialize(_vec3 vCreatePos);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};
#endif // MeteoEffect_h__