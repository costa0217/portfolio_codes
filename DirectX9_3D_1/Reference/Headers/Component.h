/*!
 * \file Component.h
 * \date 2015/11/07 1:04
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CComponet 를 상속받는 class들의 최상위 class.
 *
 * \note
*/

#ifndef Component_h__
#define Component_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public Engine::CBase
{
protected:
	explicit CComponent(void);
	virtual ~CComponent(void);
	
public:
	virtual _int Update(const _float& fTimeDelta) {return 0;}

public:
	virtual _ulong Release(void);
};

END

#endif // Component_h__