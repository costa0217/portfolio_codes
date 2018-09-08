#ifndef Frame_h__
#define Frame_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public Engine::CComponent
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);

public:
	HRESULT Initialize(_int CntX, _int CntY, _float fFrameSpeed = 1.f);

public:
	virtual _int Update(const _float& fTimeDelta);

public:
	static CFrame* Create();

public:
	_int			m_iCntX;
	_int			m_iCntY;

	_float			m_fFrameX;
	_int			m_iFrameHeight;

	_float			m_fFrameSpeed;

	_bool			m_bEnd;

public:
	virtual _ulong Release(void);

};

END

#endif // Frame_h__
