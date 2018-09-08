#ifndef CullingMgr_h__
#define CullingMgr_h__

#include "Defines.h"


class CCullingMgr
{
public:
	DECLARE_SINGLETON(CCullingMgr)

	CCullingMgr();
	~CCullingMgr();

public:
	bool CullingCamera(const D3DXVECTOR3* pPos, float fRadius);	//카메라의 절두체와 객체의 위치와 반지름을 이용하여 컬링을 한다.

public:
	void Release(void);

private:
	D3DXVECTOR3						m_Vtx[8];
	D3DXPLANE						m_Plane[6];
};

#endif // CullingMgr_h__