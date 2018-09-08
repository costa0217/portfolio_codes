#ifndef FrameAni_h__
#define FrameAni_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFrameAni : public Engine::CComponent
{
private:
	explicit CFrameAni(void);
	virtual ~CFrameAni(void);

public:
	HRESULT Initialize(_int CntX, _int CntY, _float fFrameSpeed = 1.f);
	void Init_Frame();
public:
	virtual _int Update(const _float& fTimeDelta);

public:
	static CFrameAni* Create();

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

#endif // FrameAni_h__
