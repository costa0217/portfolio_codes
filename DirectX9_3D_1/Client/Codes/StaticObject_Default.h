/*!
 * \file StaticObject_Default.h
 * \date 2015/11/25 16:31
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: StaticObject Default Type class.
 *
 * \note
*/

#ifndef StaticObject_Default_h__
#define StaticObject_Default_h__

#include "StaticObject.h"
class CStaticObject_Default : public CStaticObject
{
private:
	explicit CStaticObject_Default(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	~CStaticObject_Default(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CStaticObject_Default* Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pObjectName);

private:
	_float		m_fTimeDelta;
	_float		m_fAlpha;
	_vec3		m_vViewPos;
private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);

	virtual void Set_ConstantTable(void);

public:
	virtual _ulong Release(void);
};
#endif // StaticObject_Default_h__