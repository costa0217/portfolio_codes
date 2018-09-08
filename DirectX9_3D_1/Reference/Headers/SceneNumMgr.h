#ifndef SceneNumMgr_h__
#define SceneNumMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CSceneNumMgr
{
public:
	DECLARE_SINGLETON(CSceneNumMgr)
private:
	explicit CSceneNumMgr(void);
	~CSceneNumMgr(void);

public:
	void		Set_CurrentScene(_int iSceneNum){m_iCurrentSceneNum = iSceneNum;}
	_int		Get_CurrentScene(void){ return m_iCurrentSceneNum; }
private:
	int			m_iCurrentSceneNum;

public:
	void Release(void);
};
END

#endif // SceneNumMgr_h__