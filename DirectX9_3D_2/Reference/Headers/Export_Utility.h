/*!
 * \file Export_Utility.h
 * \date 2015/04/03 1:44
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
#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "LightMgr.h"
#include "ShaderMgr.h"
#include "Transform.h"
#include "Quaternion.h"
#include "Frame.h"
#include "HPoint.h"

#include "Material.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Collision.h"

#include "Frustum.h"
#include "QuadTree.h"
#include "NaviMesh.h"



BEGIN(Engine)


//Getter
inline CFrustum* Get_Frustum(void);
inline CQuadTree* Get_QuadTree(void);
inline CNaviMesh* Get_NaviMesh(void);

//For.Getter------------------------------------------------------------------------------------------------------------
inline Engine::CComponent* Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt = 0);
inline LPD3DXEFFECT Get_EffectHandle(const TCHAR* pShaderTag);
inline const D3DLIGHT9* Get_LightInfo(const _ulong& dwLightIdx);

//For.Setter------------------------------------------------------------------------------------------------------------
inline void Add_RenderGroup(Engine::CRenderer::RENDERTYPE eType, Engine::CGameObject* pGameObject, _float fViewZ = 0.f);

//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Ready_ShaderFromFiles(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath);
inline HRESULT Ready_Management(LPDIRECT3DDEVICE9 pDevice);
inline _int Update_Management(const _float& fTimeDelta);
inline void Render_Management(void);
template <typename T> HRESULT Change_Scene(T& Functor);
inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);
inline _bool Collision_AABB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld);
inline _bool Collision_OBB(const OBBINFO& DestInfo, const _matrix& mDestWorld, const OBBINFO& SourInfo, const _matrix& mSourWorld);
inline _bool Collision_SphereSphere(const _vec3& vDestPos, const _float& fDestRadius, const _vec3& vSourPos, const _float& fSourRadius);
inline _bool Collision_SphereOBB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld, const _vec3& vSourPos, const _float& fSourRadius);

//For.Release------------------------------------------------------------------------------------------------------------
inline void Release_Light(void);
inline void Release_Utility(void);



#include "Export_Utility.inl"

END


#endif // Export_Utility_h__