//For.Getter------------------------------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 Engine::Get_GraphicDev(void)
{
	return Engine::CGraphicDev::GetInstance()->Get_GraphicDev();
}

_float Engine::Get_TimeDelta(const TCHAR* pTimerTag)
{
	return Engine::CTimeMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}

_bool Engine::Get_Activate(const TCHAR* pFrameTag, const _float& fTimeDelta)
{
	return Engine::CFrameMgr::GetInstance()->Get_Activate(pFrameTag, fTimeDelta);
}

_byte Engine::GetDIKeyState(BYTE KeyFlag)
{
	return Engine::CInput::GetInstance()->GetDIKeyState(KeyFlag);
}

_byte Engine::GetDIMouseState(Engine::CInput::MOUSECLICK KeyFlag)
{
	return Engine::CInput::GetInstance()->GetDIMouseState(KeyFlag);
}

_long Engine::GetDIMouseMove(Engine::CInput::MOUSEMOVE KeyFlag)
{
	return Engine::CInput::GetInstance()->GetDIMouseMove(KeyFlag);
}
inline NxPhysicsSDK* Get_PhysxSDK(void)
{
	return Engine::CPhysxSDK::GetInstance()->Get_PhysxSDK();
}
inline NxScene*	  Get_PhysxScene()
{
	return Engine::CPhysxSDK::GetInstance()->Get_PhysxScene();
}
//For.Setter------------------------------------------------------------------------------------------------------------
void Engine::Set_TimeDelta(const TCHAR* pTimerTag)
{
	Engine::CTimeMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}

void Engine::Set_InputState(void)
{
	Engine::CInput::GetInstance()->Set_InputState();
}

//For.Generic------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_GraphicDev(Engine::CGraphicDev::WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY)
{
	return Engine::CGraphicDev::GetInstance()->Ready_GraphicDev(Mode, hWnd, wSizeX, wSizeY);
}

HRESULT Engine::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	return Engine::CInput::GetInstance()->Ready_InputDevice(hInst, hWnd);
}

HRESULT Engine::Ready_Timer(const TCHAR* pTimerTag)
{
	return Engine::CTimeMgr::GetInstance()->Ready_Timer(pTimerTag);
}

HRESULT Engine::Ready_Frame(const TCHAR* pFrameTag, const _float& fFps)
{
	return Engine::CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fFps);
}

inline HRESULT Engine::Ready_Font(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return Engine::CFontMgr::GetInstance()->Ready_Font(pDevice, pFontKey, iWidth, iHeight, iWeight);
}

inline void Engine::Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontKey, vPos, pString, Color);
}
inline HRESULT Ready_PhysxSDK()
{
	return Engine::CPhysxSDK::GetInstance()->Ready_PhysxSDK();
}
inline NxActor* CreateBox(_vec3* pPos, _vec3* pSize)
{
	return Engine::CPhysxSDK::GetInstance()->CreateBox(pPos, pSize);
}
inline NxActor* CreateSphere(_vec3* pPos, _float fHeight, _float fRadius)
{
	return Engine::CPhysxSDK::GetInstance()->CreateSphere(pPos, fHeight, fRadius);
}
inline NxActor* CreateGroundPlane()
{
	return Engine::CPhysxSDK::GetInstance()->CreateGroundPlane();
}
inline void UpdateSimulate(_float fTime)
{
	Engine::CPhysxSDK::GetInstance()->UpdateSimulate(fTime);
}
//For.Release------------------------------------------------------------------------------------------------------------
inline void Engine::Release_System(void)
{
	Engine::CInput::GetInstance()->DestroyInstance();
	Engine::CFontMgr::GetInstance()->DestroyInstance();
	Engine::CFrameMgr::GetInstance()->DestroyInstance();
	Engine::CTimeMgr::GetInstance()->DestroyInstance();	
	Engine::CGraphicDev::GetInstance()->DestroyInstance();
	Engine::CPhysxSDK::GetInstance()->DestroyInstance();
}