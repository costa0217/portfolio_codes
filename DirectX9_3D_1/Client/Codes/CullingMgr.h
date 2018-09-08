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
	bool CullingCamera(const D3DXVECTOR3* pPos, float fRadius);	//ī�޶��� ����ü�� ��ü�� ��ġ�� �������� �̿��Ͽ� �ø��� �Ѵ�.

public:
	void Release(void);

private:
	D3DXVECTOR3						m_Vtx[8];
	D3DXPLANE						m_Plane[6];
};

#endif // CullingMgr_h__