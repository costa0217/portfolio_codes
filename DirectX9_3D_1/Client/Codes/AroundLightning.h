/*!
 * \file AroundLightning.h
 * \date 2016/01/06 17:19
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Glitter Æø¹ß ½ºÅ³
 *
 * TODO: long description
 *
 * \note
*/
#ifndef AroundLightning_h__
#define AroundLightning_h__

#include "Effect.h"

class CAroundLightning : public CEffect
{
protected:
	explicit CAroundLightning(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAroundLightning(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet = false);

private:
	_bool	m_bBullet;
private:
	virtual HRESULT Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};

#endif // AroundLightning_h__