/*!
 * \file Terrain.h
 * \date 2015/11/23 15:35
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 지형을 띄우기 위한 클래스
 *
 * TODO: 이 클래스 하나로 모든 스테이지의 지형을 띄운다.
 *
 * \note
*/

#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCETYPE eResourceType);

private:
	Engine::CVIBuffer*	m_pBufferCom;
	Engine::CTransform*	m_pTransCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CTexture*	m_pTextureNormalCom;

	LPD3DXEFFECT		m_pEffect;

	NxActor*			m_pPlaneActor;
	RESOURCETYPE	m_eResourceType;

private:
	virtual HRESULT Initialize(RESOURCETYPE eResourceType);
	virtual HRESULT Add_Component(void);

	void Set_ContantTable(void);

public:
	virtual _ulong Release(void);
};
#endif // Terrain_h__