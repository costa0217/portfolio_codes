#ifndef StageScene_h__
#define StageScene_h__

#include "Defines.h"
#include "Scene.h"

namespace Engine
{
	class CFrustum;
	class CNaviMesh;
}

class CStageScene : public Engine::CScene
{
private:
	explicit CStageScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStageScene(void);
public:
	virtual HRESULT Init_Scene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CStageScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	HRESULT Ready_Lighting(void);
	HRESULT Ready_Environment(void);
	HRESULT Ready_GameLogic(void);

	void	Load_StaticMesh(Engine::CLayer* pLayer);

private:
	Engine::CNaviMesh*		m_pNaviMesh;

	Engine::CFrustum*		m_pFrustum;

public:
	float		m_fTime;
	DWORD		m_dwCount;
	TCHAR		m_szFps[128];


public:
	virtual _ulong Release(void);
};


#endif // StageScene_h__
