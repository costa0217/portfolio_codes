#include "InputMgr.h"

IMPLEMENT_SINGLETON(CInputMgr)



CInputMgr::CInputMgr()
	: m_fDeltaX(0.f)
	, m_fDeltaY(0.f)
	, m_fMousePosX(0.f)
	, m_fMousePosY(0.f)
{
}


CInputMgr::~CInputMgr()
{
}

void CInputMgr::UpdateInput(float mPosx, float mPosy)
{
	if (m_bCheck)
	{
		m_bCheck = false;
		m_fDeltaX = 0.f;
		m_fDeltaY = 0.f;

		m_fMousePosX = mPosx;
		m_fMousePosY = mPosy;
		return;
	}
	float epsilon = 0.5f;
	m_fDeltaX = m_fMousePosX - mPosx;
	m_fDeltaY = m_fMousePosY - mPosy;

	if (abs(m_fDeltaX) < epsilon) m_fDeltaX = 0;
	if (abs(m_fDeltaY) < epsilon) m_fDeltaY = 0;

	m_fMousePosX = mPosx;
	m_fMousePosY = mPosy;

}