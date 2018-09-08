
//For.Getter------------------------------------------------------------------------------------------------------------
inline Engine::CComponent*	Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt/* = 0*/)
{
	return Engine::CManagement::GetInstance()->Get_Component(pComponentTag, pLayerTag, pObjectTag, iCnt);
}

inline LPD3DXEFFECT Engine::Get_EffectHandle(const TCHAR* pShaderTag)
{
	return Engine::CShaderMgr::GetInstance()->Get_EffectHandle(pShaderTag);
}

inline const D3DLIGHT9* Engine::Get_LightInfo(const _ulong& dwLightIdx)
{
	return Engine::CLightMgr::GetInstance()->Get_LightInfo(dwLightIdx);
}
inline CNaviMesh* Get_NaviMesh(void)
{
	return CNaviMesh::GetInstance();
}

inline CCameraNaviMesh* Get_CameraNaviMesh(void)
{
	return CCameraNaviMesh::GetInstance();
}
inline LPDIRECT3DTEXTURE9 Get_TargetTexture(const TCHAR* pTargetTag)
{
	return Engine::CRenderTargetMgr::GetInstance()->Get_TargetTexture(pTargetTag);
}
//For.Setter------------------------------------------------------------------------------------------------------------
inline void Add_RenderGroup(Engine::CRenderer::RENDERTYPE eType, Engine::CGameObject* pGameObject, float fViewZ)
{
	Engine::CManagement::GetInstance()->Add_RenderGroup(eType, pGameObject, fViewZ);
}
inline void Set_MRT(const TCHAR* pMRTTag)
{
	Engine::CRenderTargetMgr::GetInstance()->Set_MRT(pMRTTag);
}
inline void Reset_MRT(const TCHAR* pMRTTag)
{
	Engine::CRenderTargetMgr::GetInstance()->Reset_MRT(pMRTTag);
}
inline void Set_LightInfo(const _ulong& dwLightIdx, D3DLIGHT9* pLightInfo)
{
	Engine::CLightMgr::GetInstance()->Set_LightInfo(dwLightIdx, pLightInfo);
}
//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Engine::Ready_ShaderFromFiles(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath)
{
	return Engine::CShaderMgr::GetInstance()->Ready_ShaderFromFiles(pGraphicDev, pShaderTag, pFilePath);
}

inline HRESULT Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return Engine::CManagement::GetInstance()->Ready_Management(pGraphicDev);
}

inline _int Update_Management(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update(fTimeDelta);
}

inline void Render_Management(const _float& fTimeDelta)
{
	CManagement::GetInstance()->Render(fTimeDelta);
}

template <typename T> HRESULT Change_Scene(T& Functor)
{
	return CManagement::GetInstance()->SceneChange(Functor);
}

inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, dwLightIdx);
}

inline void Render_DebugBuffer(const TCHAR* pTargetTag)
{
	Engine::CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pTargetTag);
}
inline void Render_Light(LPD3DXEFFECT pEffect)
{
	Engine::CLightMgr::GetInstance()->Render(pEffect);
}
inline void Delete_Light(_int iIndex)
{
	Engine::CLightMgr::GetInstance()->Delete_Light(iIndex);
}
//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_Light(void)
{
	CLightMgr::GetInstance()->Delete_AllLight();
	//CLightMgr::GetInstance()->DestroyInstance();
}

inline void Release_Utility(void)
{
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CShaderMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CCollision::GetInstance()->DestroyInstance();
}