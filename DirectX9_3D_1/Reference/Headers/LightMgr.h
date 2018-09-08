/*!
 * \file LightMgr.h
 * \date 2015/11/07 1:16
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CLight를 List STL 를 통해서 관리하는 Singleton CLightMgr class.
 *
 * \note
*/

#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CLight;
class ENGINE_DLL CLightMgr
{
public:
	DECLARE_SINGLETON(CLightMgr)
private:
	explicit CLightMgr(void);
	~CLightMgr(void);
public:
	const D3DLIGHT9* Get_LightInfo(const _ulong& dwLightIdx);
	void Set_LightInfo(const _ulong& dwLightIdx, D3DLIGHT9* pLightInfo);

	void	Delete_AllLight(void);

	void				Set_PlayerPos(const _vec3* pvPlayerPos);
	const _vec3*	Get_PlayerPos(void){ return m_pvPlayerPos; }
	void				Set_BossPos(const _vec3* pvBossPos);
	const _vec3*	Get_BossPos(void){ return m_pvBossPos; }
public:
	HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);
	void Render(LPD3DXEFFECT pEffect);
	void Delete_Light(_int iIndex);

private:
	list<CLight*>					m_LightList;
	typedef list<CLight*>		LIGHTLIST;	
	int			m_iLightSize;

	const _vec3*				m_pvPlayerPos;
	const _vec3*				m_pvBossPos;

	//컬링 관련
	D3DXVECTOR3						m_Vtx[8];
	D3DXPLANE						m_Plane[6];
	LPDIRECT3DDEVICE9				m_pGraphicDev;

private:
	bool CullingCamera(const D3DXVECTOR3* pPos, float fRadius);	//카메라의 절두체와 객체의 위치와 반지름을 이용하여 컬링을 한다.

public:
	int		Get_LightCnt(void){ return m_iLightSize; }
public:
	void Release(void);
};

END

#endif // LightMgr_h__
