/*!
 * \file Export_System.h
 * \date 2015/07/01 13:33
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

BEGIN(Engine)

inline CGraphicDev* Get_GraphicDev(void);
inline CTimeMgr* Get_TimeMgr(void);
inline CKeyMgr* Get_KeyMgr(void);

#include "Export_System.inl"

END

#endif // Export_System_h__
