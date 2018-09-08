/*!
 * \file Resources.h
 * \date 2015/11/06 22:42
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CComponent class를 상속받아 컴포넌트 패턴을 통해 Buffer, Mesh, Texture등 모든 리소스의 부모 class.
 *
 * \note
*/

#ifndef Resources_h__
#define Resources_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources
	: public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CResources(void);

public:
	virtual CResources* Clone_Resource(void) PURE;

protected:
	_ulong*				m_pwRefCnt;

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

public:
	virtual _ulong Release(void);
};

END

#endif // Resources_h__