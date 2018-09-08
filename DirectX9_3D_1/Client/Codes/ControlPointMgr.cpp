#include "stdafx.h"
#include "ControlPointMgr.h"

CControlPointMgr::CControlPointMgr(CameraModeType eCMT, StandardType eST)
: m_eCameraMode(eCMT)
, m_eStandardPos(eST)
{

}

CControlPointMgr::~CControlPointMgr(void)
{

}

CControlPointMgr* CControlPointMgr::Create(CameraModeType eCMT, StandardType eST)
{
	CControlPointMgr* pControlPointMgr = new CControlPointMgr(eCMT, eST);
	return pControlPointMgr;
}

_ulong CControlPointMgr::Release(void)
{
	m_ControlPointVector.clear();
	delete this;
	return 0;
}
