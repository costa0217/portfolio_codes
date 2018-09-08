#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"
#include <fstream>
namespace Engine
{
	class CResourceMgr;
	class CGraphicDev;
}

class CLoading : public Engine::CBase
{
public:
	enum LOADINGTYPE {LOADING_DYE, LOADING_STAGE, LOADING_MIDDLE_STAGE, LOADING_END};
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
	void DyeSceneLoading(void);
	void StageLoading(void);
	void MiddleStageLoading(void);
public:
	static CLoading* Create(LOADINGTYPE eLoadingType);
	static CLoading::LOADINGTYPE m_eLoadingType;
	static _int	m_iLoadingNumber;
private:
	static UINT WINAPI LoadingFunction(void* pArg);
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LOADINGTYPE				m_LoadType;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;

private:
	TCHAR			m_szLoadMessage[128];
	bool			m_bComplete;
public:
	virtual _ulong Release(void);
};

#endif // Loading_h__