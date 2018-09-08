#ifndef BossSword_h__
#define BossSword_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CQuaternion;
	class CStaticMesh;
}

class CBossSword	: public Engine::CGameObject
{
private:
	explicit CBossSword(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName, _int iIdx);
	virtual ~CBossSword(void);

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
	static CBossSword* Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName, _int iIdx);

private:	
	Engine::CQuaternion*		m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

	Engine::CTransform*			m_pBossTransCom;
	Engine::CTransform*			m_pPlayerTransCom;

	Engine::CGameObject*		m_pPlayer;
private:
	LPD3DXEFFECT				m_pEffect;

	_vec3						m_vSwordEndPos;
private:
	TCHAR						m_wstrMeshName[128];
	_int						m_iIndex;

	_bool						m_bSwitch;
	_bool						m_bOnce;
	_bool						m_bRender;

	_float						m_fGoTime;

	_float						m_fStartTime;

	_vec3						m_vAtkDir;
	_float						m_fAccelSpeed;
	const _matrix*				m_pMatrix;

private: // Collision
	Engine::OBBINFO*			m_pPlayerOBBInfo;
	Engine::OBBINFO*			m_pMyOBBInfo;

	_vec3*						m_pvPlayerMin;
	_vec3*						m_pvPlayerMax;
	_vec3*						m_pvMyMin;
	_vec3*						m_pvMyMax;	

	_float						m_fPlayerSphereRad;
	_float						m_fMySphereRad;


private:
	_bool						m_bSound;

private:
	void	Check_Collision(void);

private:
	void	Set_ContantTable(void);
	void	Set_SettingData(void);
public:
	virtual _ulong Release(void);


};

#endif // BossSword_h__
