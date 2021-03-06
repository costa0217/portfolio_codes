/*!
 * \file Loading.h
 * \date 2015/08/19 12:23
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
#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"

namespace Engine
{
	class CResourceMgr;
	class CGraphicDev;
	class CNaviMesh;
}

class CLoading : public Engine::CBase
{
public:
	enum LOADINGTYPE {LOADING_STAGE, LOADING_END};
private:
	explicit CLoading(LOADINGTYPE eLoadingType);
	~CLoading(void);
public:
	LOADINGTYPE GetLoadType(void);
	CRITICAL_SECTION* GetCSKey(void);
	bool GetComplete(void);
	const TCHAR* GetLoadingMessage(void);

public:
	void InitLoading(void);
	void StageLoading(void);
	void StageLoading1(void);

private:
	void LoadingSaveMapData(void);
	void LoadingSaveNaviData(void);

public:
	static CLoading* Create(LOADINGTYPE eLoadingType);
private:
	static UINT WINAPI LoadingFunction(void* pArg);

private:
	Engine::CNaviMesh*		m_pNaviMesh;
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LOADINGTYPE				m_LoadType;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;

private:
	TCHAR			m_szLoadMessage[128];
	bool			m_bComplete;

private:
	_ulong			m_dwCntX;
	_ulong			m_dwCntZ;
	_ulong			m_dwItv;

	vector<_vec3>	m_vecPos;
public:
	virtual _ulong Release(void);
};

#endif // Loading_h__