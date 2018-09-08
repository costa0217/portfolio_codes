#ifndef IceAge_h__
#define IceAge_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CIceAge	: public Engine::CGameObject
{
private:
	explicit CIceAge(LPDIRECT3DDEVICE9 pGraphicDev, int iIdx);
	virtual ~CIceAge(void);

public: //Get
	void	Get_Switch(void);
public: //Set
	void	Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }
	void	Set_Pos(_vec3 vPos);
	void	Set_DataInit(void);

	void	Set_Player(Engine::CGameObject* pPlayer) { m_pPlayer = pPlayer; }
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CIceAge* Create(LPDIRECT3DDEVICE9 pGraphicDev, int iIdx);

private:	
	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

	Engine::CTransform*			m_pPlayerTransCom;
	Engine::CTransform*			m_pBossTransCom;
	Engine::CGameObject*		m_pPlayer;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;
	_int						m_iIndex;

	_float						m_fPlayerSpeed;

	_float						m_fAlpha;
	_vec3						m_vViewPos;

	_bool						m_bSound;


private: // Collision
	Engine::OBBINFO*			m_pPlayerOBBInfo;
	Engine::OBBINFO*			m_pMyOBBInfo;

	_vec3*						m_pvPlayerMin;
	_vec3*						m_pvPlayerMax;
	_vec3*						m_pvMyMin;
	_vec3*						m_pvMyMax;	

	_float						m_fPlayerSphereRad;
	_float						m_fMySphereRad;

	_float						m_fAccTime;
	_matrix						m_matColli;

private:
	void	Check_Collision(void);

private:
	void	Set_ContantTable(void);
	void	Set_SettingData(void);
public:
	virtual _ulong Release(void);


};

#endif // IceAge_h__
