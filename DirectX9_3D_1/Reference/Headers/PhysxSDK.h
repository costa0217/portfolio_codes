
#ifndef PhysxSDK_h__
#define PhysxSDK_h__

#include "NxPhysics.h"
#include "NxVec3.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CPhysxSDK
{
public:
	DECLARE_SINGLETON(CPhysxSDK)

private:
	explicit CPhysxSDK(void);

public:
	~CPhysxSDK(void);

public:
	// getter
	NxPhysicsSDK* Get_PhysxSDK(void) { return m_pPhysxSDK; }
	NxScene*	  Get_PhysxScene(){return m_pPhysxScene;}
public:
	HRESULT Ready_PhysxSDK();
	NxActor*	CreateBox(_vec3* pPos,_vec3* pSize);
	NxActor*	CreateSphere(_vec3* pPos, _float fHeight, _float fRadius);
	NxActor*	CreateGroundPlane();
	void UpdateSimulate(_float fTime);

private:
	NxPhysicsSDK*			m_pPhysxSDK;		
	NxScene*				m_pPhysxScene;	
	NxSceneDesc				m_SceneDesc;

private:
	void Release(void);

};

END

#endif // PhysxSDK_h__