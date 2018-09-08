/*!
 * \file Spin.h
 * \date 2016/01/09 18:08
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Moonchopper Spin
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Spin_h__
#define Spin_h__

#include "Effect.h"

class CSpin : public CEffect
{
protected:
	explicit CSpin(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpin(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet = false);

private:
	_int	m_iSpinNum;
private:
	virtual HRESULT Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};

#endif // Spin_h__