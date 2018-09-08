/*!
 * \file Pierceaura.h
 * \date 2016/01/08 17:30
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Moonchopper 요격스킬
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Pierceaura_h__
#define Pierceaura_h__

#include "Effect.h"

class CPierceaura : public CEffect
{
protected:
	explicit CPierceaura(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPierceaura(void);

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
#endif // Pierceaura_h__