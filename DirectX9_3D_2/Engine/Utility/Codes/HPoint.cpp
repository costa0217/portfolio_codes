#include "HPoint.h"

USING(Engine)

Engine::CHPoint::CHPoint(_int iHP)
: m_iHP(iHP)
{

}

Engine::CHPoint::~CHPoint(void)
{

}
HRESULT Engine::CHPoint::Initialize()
{

	return S_OK;
}



CHPoint* Engine::CHPoint::Create(_int iHP)
{
	CHPoint*		pHPoint = new CHPoint(iHP);

	if(FAILED(pHPoint->Initialize()))
	{
		MSG_BOX("pHPoint Create Failed");
		Engine::Safe_Release(pHPoint);
	}	
	return pHPoint;
}

_ulong Engine::CHPoint::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

