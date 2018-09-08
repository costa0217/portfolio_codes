/*!
 * \file Export_Utility.h
 * \date 2015/11/07 1:26
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Ŭ���̾�Ʈ���� �ʿ��� Utiltiy�� ���� �ִ� Ŭ�������� �Լ��� ��� ���� �������
 *
 * \note
*/

#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "LightMgr.h"
#include "ShaderMgr.h"
#include "Transform.h"
#include "Material.h"
#include "Collision.h"
#include "NaviMesh.h"
#include "CameraNaviMesh.h"
#include "RenderTargetMgr.h"
#include "SceneNumMgr.h"

BEGIN(Engine)

//For.Getter------------------------------------------------------------------------------------------------------------
Engine::CComponent*	Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt = 0); //tag���� ���� Component�� ���´�.
inline LPD3DXEFFECT Get_EffectHandle(const TCHAR* pShaderTag);	//Tag�� ���� ���ϴ� ���̴� ������ LPD3DXEFFECT ���� ���´�.
inline const D3DLIGHT9* Get_LightInfo(const _ulong& dwLightIdx); //�ε��� ��ȣ�� ���� ���� CLightMgr�� �����ϴ� ������ ��ȣ�� �ش��ϴ� ���� ������ ���´�.
inline CNaviMesh* Get_NaviMesh(void);
inline CCameraNaviMesh* Get_CameraNaviMesh(void);
inline LPDIRECT3DTEXTURE9 Get_TargetTexture(const TCHAR* pTargetTag);
//For.Setter------------------------------------------------------------------------------------------------------------
inline void Add_RenderGroup(Engine::CRenderer::RENDERTYPE eType, Engine::CGameObject* pGameObject, float fViewZ = 0.f);
inline void Set_MRT(const TCHAR* pMRTTag);
inline void Reset_MRT(const TCHAR* pMRTTag);
inline void Set_LightInfo(const _ulong& dwLightIdx, D3DLIGHT9* pLightInfo);
//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Ready_ShaderFromFiles(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath);
inline HRESULT Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev);
inline _int Update_Management(const _float& fTimeDelta);
inline void Render_Management(const _float& fTimeDelta);
inline void Render_DebugBuffer(const TCHAR* pTargetTag);
inline void Render_Light(LPD3DXEFFECT pEffect);
template <typename T> HRESULT Change_Scene(T& Functor);
inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);

inline void Delete_Light(_int iIndex);
//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_Light(void);
inline void Release_Utility(void);

#include "Export_Utility.inl"
END

#endif // Export_Utility_h__