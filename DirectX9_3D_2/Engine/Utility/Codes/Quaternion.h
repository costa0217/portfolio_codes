#ifndef Quaternion_h__
#define Quaternion_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CQuaternion : public Engine::CComponent
{
public:
	enum ANGLE {ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END};
private:
	explicit CQuaternion(void);
	virtual ~CQuaternion(void);
public:
	HRESULT Initialize(void);
public:
	virtual _int Update(const _float& fTimeDelta);
public:
	static CQuaternion* Create(void);
public:
	_matrix					m_matWorld;
	_vec3					m_vPosition;
	_vec3					m_vDir;
	_vec3					m_vScale;
	_float					m_fAngle[ANGLE_END];

	_float					m_fQuatAngle;
	_vec3					m_vCross;
public:
	virtual _ulong Release(void);
};

END


#endif // Quaternion_h__
