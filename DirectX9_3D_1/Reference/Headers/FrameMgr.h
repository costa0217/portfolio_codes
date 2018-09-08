/*!
 * \file FrameMgr.h
 * \date 2015/11/06 21:56
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Map STL을 통해서 CFrame class를 관리하는 Singleton Manager clsss.
 *
 * \note
*/

#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CFrame;

class ENGINE_DLL CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)
private:
	explicit CFrameMgr(void);
	~CFrameMgr(void);

public: //getter
	_bool Get_Activate(const TCHAR* pFrameTag, const _float& fTimeDelta);

public:
	HRESULT	Ready_Frame(const TCHAR* pFrameTag, const _float& fFps);

private:
	boost::unordered_map<const TCHAR*, CFrame*>			m_mapFrame;
	typedef boost::unordered_map<const TCHAR*, CFrame*>	MAPFRAME;

private:
	CFrame* Find_Frame(const TCHAR* pFrameTag);

public:
	void Release(void);

};

END

#endif // FrameMgr_h__