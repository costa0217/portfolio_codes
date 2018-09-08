#ifndef FireBall_h__
#define FireBall_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;

	class CFrame;
	class CLayer;
}

class CFireBall	: public Engine::CGameObject
{
private:
	explicit CFireBall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireBall(void);

public: //Get
	void	Get_Switch(void);
public: //Set
	void	Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }
	void	Set_Pos(_vec3 vPos);
	void	Set_DataInit(void);

	void	Set_Player(Engine::CGameObject* pPlayer) { m_pPlayer = pPlayer; }

	void	Set_Layer(Engine::CLayer* pLayer){ m_pGameLogicLayer = pLayer; }
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CFireBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	Engine::CFrame*				m_pFrame;

	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

	Engine::CTransform*			m_pPlayerTransCom;
	Engine::CTransform*			m_pBossTransCom;
	Engine::CGameObject*		m_pPlayer;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;

private: //effect
	Engine::CLayer*				m_pGameLogicLayer;


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

	_vec3						m_vAtkDir;

	_float						m_fTrailTime;

	_bool						m_bShotSound;

private:
	LPD3DXMESH					m_pSphereMesh;
	LPD3DXBUFFER				m_pSphereAdjancey;
	_float						m_fSphereRad;

private:
	void	Create_Trail(void);

private:
	void	Check_Collision(void);

private:
	void	Set_ContantTable(void);
	void	Set_SettingData(void);
public:
	virtual _ulong Release(void);


};
#endif // FireBall_h__
