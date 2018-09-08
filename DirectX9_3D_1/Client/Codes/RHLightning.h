/*!
 * \file RHLightning.h
 * \date 2016/01/06 19:07
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Glitter 오른손 스킬
 *
 * TODO: long description
 *
 * \note
*/
#ifndef RHLightning_h__
#define RHLightning_h__

#include "Effect.h"

class CRHLightning : public CEffect
{
protected:
	explicit CRHLightning(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRHLightning(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet = false);
private:
	_vec3	m_vDir;
	_bool	m_bBullet; // 날리는 이펙트인지 아닌지
private:
	virtual HRESULT Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};
#endif // RHLightning_h__