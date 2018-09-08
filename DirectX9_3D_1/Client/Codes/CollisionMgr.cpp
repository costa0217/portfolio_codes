#include "stdafx.h"
#include "CollisionMgr.h"

#include "Export_Function.h"
#include "Effect.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{

}

CCollisionMgr::~CCollisionMgr()
{

}

_int CCollisionMgr::Update(const _float& fTimeDelta)
{	
	list<CEffect*>::iterator iter		= m_pEffectList.begin();
	list<CEffect*>::iterator iterEnd	= m_pEffectList.end();
		
	_int iResult = 0;

	for(; iter != iterEnd; )
	{
		iResult = (*iter)->Update(fTimeDelta);

		if(iResult & 0x80000000)
		{
			//iResult ���� �����϶� ���´�. Ư�� ������Ʈ�� �����ϱ� ���� ����
			::Safe_Release(*iter);
			iter = m_pEffectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//list<CMonster*>::iterator iter2		= m_pMonsterList.begin();
	//list<CMonster*>::iterator iterEnd2	= m_pMonsterList.end();

	//_int iResult2 = 0;

	//for(; iter2 != iterEnd2; )
	//{
	//	iResult2 = (*iter2)->Update(fTimeDelta);

	//	if(iResult2 & 0x80000000)
	//	{
	//		//iResult ���� �����϶� ���´�. Ư�� ������Ʈ�� �����ϱ� ���� ����
	//		::Safe_Release(*iter2);
	//		iter2 = m_pMonsterList.erase(iter2);
	//	}
	//	else
	//	{
	//		++iter2;
	//	}
	//}
	return 0;
}

void CCollisionMgr::Render(void)
{
	list<CEffect*>::iterator iter		= m_pEffectList.begin();
	list<CEffect*>::iterator iterEnd	= m_pEffectList.end();

	for(; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}
}

void CCollisionMgr::Release(void)
{
	delete this;
}