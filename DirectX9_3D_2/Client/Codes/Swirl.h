#ifndef Swirl_h__
#define Swirl_h__


#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;

	class CFrame;
}

class CSwirl	: public Engine::CGameObject
{
private:
	explicit CSwirl(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwirl(void);

public:
	virtual HRESULT	Initialize(_vec3 vPos, _int iIdx);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CSwirl* Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos, _int iIdx);

private:	
	Engine::CFrame*				m_pFrame;

	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;

private: 
	_float						m_fAccTime;

	_vec4						m_vColor;
	_float						m_fAlpha;

	_int						m_iIdx;


private:
	void	Set_ContantTable(void);
	void	Set_SettingData(void);
public:
	virtual _ulong Release(void);

};
#endif // Swirl_h__
