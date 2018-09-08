/*!
 * \file ControlPointMgr.h
 * \date 2015/12/20 16:44
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief ControlPointInfo을 관리하는 class.
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ControlPointMgr_h__
#define ControlPointMgr_h__

#include "Defines.h"

class CControlPointMgr
{
private:
	explicit CControlPointMgr(CameraModeType eCMT, StandardType eST);
	~CControlPointMgr(void);

public://getter, setter
	vector<ControlPointInfo>* GetpControlPointVector(void) { return &m_ControlPointVector; }
	ControlPointInfo GetControlPointInfo(_int iSelectNumber) { return m_ControlPointVector[iSelectNumber]; }

	CameraModeType GetCameraMode(void) { return m_eCameraMode; }
	StandardType GetStandardType(void) { return m_eStandardPos; }

public:
	static CControlPointMgr* Create(CameraModeType eCMT, StandardType eST);

private:
	vector<ControlPointInfo>			m_ControlPointVector;
	typedef vector<ControlPointInfo>	CPList;

	CameraModeType	m_eCameraMode;
	StandardType	m_eStandardPos;

public:
	_ulong Release(void);
};
#endif // ControlPointMgr_h__