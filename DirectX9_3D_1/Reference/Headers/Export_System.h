/*!
 * \file Export_System.h
 * \date 2015/11/06 22:16
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Client에서 필요한 System의 속해 있는 클래스들의 함수들을 모아 놓은 헤더파일.
 *
 * \note
*/

#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimeMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "Input.h"
#include "PhysxSDK.h"
BEGIN(Engine)

//For.Getter------------------------------------------------------------------------------------------------------------
inline LPDIRECT3DDEVICE9 Get_GraphicDev(void); //그래픽디바이스 장치를 얻어온다.
inline _float Get_TimeDelta(const TCHAR* pTimerTag); //Tag를 통해 CTimeMgr에서 해당하는 CTime class에서 시간을 얻어온다.
inline _bool Get_Activate(const TCHAR* pFrameTag, const _float& fTimeDelta); //Tag를 통해 CFrameMgr에서 해당하는 CFrame에서 프레임에 따른 시간을 검사하여 bool변수를 리턴한다.
inline _byte GetDIKeyState(BYTE KeyFlag);	//키보드의 Input을 검사한다.
inline _byte GetDIMouseState(CInput::MOUSECLICK KeyFlag); //마우스의 Input을 검사한다.
inline _long GetDIMouseMove(CInput::MOUSEMOVE KeyFlag);	//마우스의 움직임을 검사한다.
inline NxPhysicsSDK* Get_PhysxSDK(void);
inline NxScene*	  Get_PhysxScene();
//For.Setter------------------------------------------------------------------------------------------------------------
inline void Set_TimeDelta(const TCHAR* pTimerTag); //Tag를 통해 해당하는 CTime class에 CPU의 진동을 이용하여 시간을 설정정한다.
inline void Set_InputState(void); //키보드, 마우스의 인풋을 설정한다.

//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Ready_GraphicDev(Engine::CGraphicDev::WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY);
inline HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
inline HRESULT Ready_Timer(const TCHAR* pTimerTag);
inline HRESULT Ready_Frame(const TCHAR* pFrameTag, const _float& fFps);
inline HRESULT Ready_Font(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
inline void Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color);
inline HRESULT Ready_PhysxSDK();
inline NxActor* CreateBox(_vec3* pPos, _vec3* pSize);
inline NxActor* CreateSphere(_vec3* pPos, _float fHeight, _float fRadius);
inline NxActor* CreateGroundPlane();
inline void UpdateSimulate(_float fTime);
//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_System(void);

#include "Export_System.inl"

END

#endif //Export_System_h__