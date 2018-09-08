/*!
 * \file Export_System.h
 * \date 2015/11/06 22:16
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Client���� �ʿ��� System�� ���� �ִ� Ŭ�������� �Լ����� ��� ���� �������.
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
inline LPDIRECT3DDEVICE9 Get_GraphicDev(void); //�׷��ȵ���̽� ��ġ�� ���´�.
inline _float Get_TimeDelta(const TCHAR* pTimerTag); //Tag�� ���� CTimeMgr���� �ش��ϴ� CTime class���� �ð��� ���´�.
inline _bool Get_Activate(const TCHAR* pFrameTag, const _float& fTimeDelta); //Tag�� ���� CFrameMgr���� �ش��ϴ� CFrame���� �����ӿ� ���� �ð��� �˻��Ͽ� bool������ �����Ѵ�.
inline _byte GetDIKeyState(BYTE KeyFlag);	//Ű������ Input�� �˻��Ѵ�.
inline _byte GetDIMouseState(CInput::MOUSECLICK KeyFlag); //���콺�� Input�� �˻��Ѵ�.
inline _long GetDIMouseMove(CInput::MOUSEMOVE KeyFlag);	//���콺�� �������� �˻��Ѵ�.
inline NxPhysicsSDK* Get_PhysxSDK(void);
inline NxScene*	  Get_PhysxScene();
//For.Setter------------------------------------------------------------------------------------------------------------
inline void Set_TimeDelta(const TCHAR* pTimerTag); //Tag�� ���� �ش��ϴ� CTime class�� CPU�� ������ �̿��Ͽ� �ð��� �������Ѵ�.
inline void Set_InputState(void); //Ű����, ���콺�� ��ǲ�� �����Ѵ�.

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