/*!
 * \file StaticObject_Default.h
 * \date 2015/12/14 20:31
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: StaticObject Destroy Type class.
 *
 * \note
*/

#ifndef StaticObject_Destroy_h__
#define StaticObject_Destroy_h__

#include "StaticObject.h"
class CStaticObject_Destroy : public CStaticObject
{
private:
	explicit CStaticObject_Destroy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	~CStaticObject_Destroy(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CStaticObject_Destroy* Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pObjectName);

private:
	_float	m_fObjectHp;
private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);

	virtual void Set_ConstantTable(void);

	void PlayerAttackBox_And_MyBodyBox(const _float& fTimeDelta);

	void Create_DestroyBrick();

public:
	virtual _ulong Release(void);
};
#endif // StaticObject_Destroy_h__