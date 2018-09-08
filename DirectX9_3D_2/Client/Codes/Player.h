#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "LandObject.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;

	class CTexture;
	class CLayer;

	class CHPoint;
	
}

class CPlayer	: public CLandObject
{
public:
	enum ANISTATE {STATE_IDLE, STATE_RUN, STATE_RUNBACK, STATE_JUMP,
		STATE_LAND, STATE_ATTACK, STATE_DMG, STATE_SKILL, STATE_END};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);
public:
	_float		Get_Speed(void){return m_fSpeed;}
	void		Set_Speed(_float fSpeed){ m_fSpeed = fSpeed; }
	void		Set_StateSlow(_bool bSlow) { m_bSlow = bSlow; }

	void		Set_Layer(Engine::CLayer* pLayer){ m_pGameLogicLayer = pLayer; }

	void		Set_GroundHit(Engine::CGameObject* pGHit){ m_pGroundHit = pGHit;}
public:
	ANISTATE	Get_State(){ return m_eAniStat; }
	_int		Get_SkillIndex(){ return m_iSkillIndex; }
	_int		Get_AttackCnt(){ return m_iAttackAniIdx; }

	_vec3*	Get_AtkCamPos(){ return &m_vAtkCamPos; }
	_vec3*	Get_DftCamPos(){ return &m_vDftCamPos; }
	_vec3*	Get_NuckCamPos(){ return &m_vNuckCamPos; }
	
	_bool	GetKeyPress(){ return m_bKeyPress; }
	_bool	GetStateJump();

	_vec3*	Get_SwordEndPos(){ return &m_vSwordEndPos; }
	_vec3*	Get_SwordStartPos(){ return &m_vSwordStartPos; }

	void	Set_SwordEndPos(_vec3* pvEndpos);

	bool	Get_Damaged(void){ return m_bDMG; }
	void	Set_Damaged(_bool bDMG) {  m_bDMG = bDMG; }

	void	Set_Boss(Engine::CGameObject* pBoss) { m_pBoss = pBoss; }

	Engine::CTransform*	GetTransCom(){ return m_pTransCom; }

public:
	void	SetDirection(_vec3 vDir);

private:	// 이펙트
	Engine::CLayer*				m_pGameLogicLayer;

private:	// 이펙트 생성
	void	Create_Blood(_vec3 vPos);
	void	Create_Impact(_int iIndex);

	void	Create_GroundHit();
	void	Create_Wang();

	void	Create_Bloody();

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:	
	Engine::CDynamicMesh*		m_pMeshCom;

	Engine::CGameObject*		m_pBoss;
	Engine::CTransform*			m_pBossTransCom;

	Engine::CHPoint*			m_pHpPtCom;

	Engine::CGameObject*		m_pGroundHit;
	
private:
	LPD3DXEFFECT				m_pEffect;
	ANISTATE					m_eAniStat;

private:
	const _matrix*				m_pmatBone;

	_vec3*						m_pSwordEndPos;	

	_vec3						m_vSwordEndPos;	
	_vec3						m_vSwordStartPos;

	_int						m_iFrame;
	_float						m_fSpeed;
	_bool						m_bSlow;
	_float						m_fSlowTime;
private:
	_float						m_fTimeDelta;
	_uint						m_iAniIdx;

	_bool						m_bKeyPress;
	_bool						m_bRunJump;

	_float						m_fJumpTime;
	_int						m_iAttackAniIdx;

private:
	_float						m_fJumpPos;
	_bool						m_bJumpFall;

	_bool						m_bJumpDmg;

	_bool						m_bSound;

	
private:	//애니메이션			
	_bool						m_bDMG;
	_float						m_fFixTime;
	_bool						m_DiagRun;

	_bool						m_bPattern[4];
	_int						m_iSkillIndex;
	_float						m_fSkillAccTime;

private:	// 카메라
	_vec3						m_vAtkCamPos;
	_vec3						m_vDftCamPos;
	_vec3						m_vNuckCamPos;
	const _matrix*				m_matLocalAtkBone;
	const _matrix*				m_matLocalDftBone;
	const _matrix*				m_matLocalNuckBone;

private:	//보스 스킬
	//2
	_bool						m_bFushing;
	_bool						m_bFushingJumping;
	_float						m_fAccelSpeed;
	_float						m_fAccelSpeed2;
	_float						m_fSkillStartTime;
	_float						m_fSkillStartTime2;

	void	Fire_Skill2(void);
	void	Fire_Skill4(void);

	
private:
	void	Set_CamposToBoneMatrix(void);

	
private:
	void	Set_ContantTable(void);
	void	Check_KeyState(const _float& fTimeDelta);

private: //트레일

	const _matrix*		m_pmatFrameMat;
	void	Create_Trail();
private: //점프
	void	Jump_Player(void);
	void	Check_Jumping(const _float& fTimeDelta);

private: //상태이상
	void	StateSet_Slow(void);

public:
	virtual _ulong Release(void);
};


#endif // Player_h__
