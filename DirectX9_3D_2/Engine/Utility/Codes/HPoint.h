#ifndef HPoint_h__
#define HPoint_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CHPoint : public Engine::CComponent
{
private:
	explicit CHPoint(_int iHP);
	virtual ~CHPoint(void);


public:
	HRESULT Initialize();

public:
	static CHPoint* Create(_int iHP);

public:
	_int			m_iHP;

public:
	virtual _ulong Release(void);

};

END

#endif // HPoint_h__
