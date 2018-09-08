#ifndef GroundHit_h__
#define GroundHit_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;

	class CFrame;
}

class CGroundHit	: public Engine::CGameObject
{
private:
	explicit CGroundHit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGroundHit(void);

public: //Get
	void	Get_Switch(void);
public: //Set
	void	Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }
	void	Set_Pos(_vec3 vPos);
	void	Set_DataInit(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CGroundHit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	Engine::CFrame*				m_pFrame;

	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;
	_bool						m_bRender;

private: 
	_vec3						m_vViewPos;
	_float						m_fAccTime;

	_vec4						m_vColor;


private:
	void	Check_Collision(void);

private:
	void	Set_ContantTable(void);
	void	Set_SettingData(void);
public:
	virtual _ulong Release(void);

};
#endif // GroundHit_h__
