/*!
 * \file ForeHandLightning.h
 * \date 2016/01/06 16:53
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Glitter 양손공격 스킬 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef ForeHandLightning_h__
#define ForeHandLightning_h__

#include "Effect.h"

class CForeHandLightning : public CEffect
{
protected:
	explicit CForeHandLightning(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CForeHandLightning(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, TCHAR* pEffectName);

private:
	virtual HRESULT Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, TCHAR* pEffectName);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};

#endif // ForeHandLightning_h__