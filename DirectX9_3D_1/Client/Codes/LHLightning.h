/*!
 * \file LHLightning.h
 * \date 2016/01/05 17:39
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Glitter�޼ս�ų 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef LHLightning_h__
#define LHLightning_h__

#include "Effect.h"

class CLHLightning : public CEffect
{
protected:
	explicit CLHLightning(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLHLightning(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet = false);
private:
	_vec3	m_vDir;
	_bool	m_bBullet; // ������ ����Ʈ���� �ƴ���
private:
	virtual HRESULT Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet);
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};

#endif // LHLightning_h__