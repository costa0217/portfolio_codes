/*!
 * \file Export_Utility.h
 * \date 2015/11/07 1:26
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 클라이언트에서 필요한 Utiltiy의 속해 있는 클래스들의 함수를 모아 놓은 헤더파일
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
Engine::CComponent*	Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt = 0); //tag들을 통해 Component를 얻어온다.
inline LPD3DXEFFECT Get_EffectHandle(const TCHAR* pShaderTag);	//Tag를 통해 원하는 셰이더 파일의 LPD3DXEFFECT 값을 얻어온다.
inline const D3DLIGHT9* Get_LightInfo(const _ulong& dwLightIdx); //인덱스 번호를 통해 현재 CLightMgr가 관리하는 빛에서 번호에 해당하는 빛의 정보를 얻어온다.
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