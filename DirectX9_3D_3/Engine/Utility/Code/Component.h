/*!
 * \file Component.h
 * \date 2015/07/06 13:17
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent
{
protected:
	explicit CComponent(void);

public:
	virtual ~CComponent(void) = 0;

public:
	virtual void Update(void) {};
};

END

#endif // Component_h__