#ifndef Boss_h__
#define Boss_h__
#include "Defines.h"
#include "LandObject.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
	class CLayer;

	class CHPoint;
}

class CBoss	: public CLandObject
{

public:
	enum ANISTATE {
	Boss_DeadBody,
	Boss_Die,
	Boss_SK_Firing_05,
	Boss_SK_Firing_04,
	Boss_SK_Firing_03,

	Boss_SK_Firing_02,
	Boss_SK_Firing_01,
	Boss_ATK_02,
	Boss_ATK_01,
	Boss_Get_Up,

	Boss_Down_Loop,
	Boss_Down,
	Boss_DMG_F,
	Boss_Initate,
	Boss_Hibernate,

	Boss_Run,
	Boss_PassOut,
	Boss_Walk,
	Boss_Idle2,
	Boss_Idle1,
	};

private:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss(void);

public://set
	void		Set_Player(Engine::CGameObject* pPlayer) { m_pPlayer = pPlayer; }
	void		Set_FireBall(Engine::CGameObject* pFireBall) { m_pFireBall = pFireBall; }
	void		Set_SwordList(list<Engine::CGameObject*>* pList){ m_pBossSwordList = pList;}

	void		Set_IceAge(list<Engine::CGameObject*>* pList) { m_pIceAgeList = pList; }
	void		Set_IceThone(list<Engine::CGameObject*>* pList){ m_pIceThoneList = pList;}

	void		Set_Layer(Engine::CLayer* pLayer){ m_pGameLogicLayer = pLayer; }

public://get
	_bool		Get_Openning(){return m_bOpening;}
	_bool		Get_Start(){return m_bStart;}

	ANISTATE	Get_State(){ return m_eAniStat; }

	_vec3*		Get_StartPos(){ return &m_Finger1Pos;}
	_vec3*		Get_EndPos(){ return &m_Finger2Pos;}
	_vec3*		Get_StartPos2(){ return &m_Finger1Pos2;}
	_vec3*		Get_EndPos2(){ return &m_Finger2Pos2;}

	_vec3		Get_Pos();

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	// 이펙트
	Engine::CLayer*				m_pGameLogicLayer;

private:	
	Engine::CDynamicMesh*		m_pMeshCom;

	Engine::CGameObject*		m_pPlayer;

	Engine::CHPoint*			m_pHpCom;

	list<Engine::CGameObject*>*		m_pBossSwordList;	//보스스워드 리스트
	Engine::CGameObject*			m_pFireBall;

	list<Engine::CGameObject*>*		m_pIceAgeList;		//얼음바닥
	list<Engine::CGameObject*>*		m_pIceThoneList;	//얼음가시 리스트

private:
	LPD3DXEFFECT				m_pEffect;

	ANISTATE					m_eAniStat;

private:
	_matrix						m_mDMGTrans;	// 크기 행렬 바운딩박스 //피격
	_matrix						m_mATKTrans;	// 공격

	_float						m_fTimeDelta;
	_float						m_fSpeed;

	Engine::CTransform*			m_pShieldTransCom;
	Engine::CTransform*			m_pSwordTransCom;
	Engine::CTransform*			m_pPlayerTransCom;


	Engine::OBBINFO*			m_pPlayerOBBInfo;
	Engine::OBBINFO*			m_pSwordOBBInfo;
	Engine::OBBINFO*			m_pShieldOBBInfo;
	Engine::OBBINFO*			m_pMyOBBInfo;

private: // Collision Data
	_vec3*						m_pvSwordMin;
	_vec3*						m_pvSwordMax;
	_vec3*						m_pvShieldMin;
	_vec3*						m_pvShieldMax;
	_vec3*						m_pvPlayerMin;
	_vec3*						m_pvPlayerMax;
	_vec3*						m_pvMyMin;
	_vec3*						m_pvMyMax;	

	_float						m_fShieldSphereRad;
	_float						m_fSwordSphereRad;
	_float						m_fPlayerSphereRad;
	_float						m_fMySphereRad;

private:					//애니메이션
	_float						m_fFixTime;
	_float						m_fAtkStartTime;

	_int						m_iPlayerAtkIdx;
	_int						m_iPrePlayerAtkIdx;
	_bool						m_bColl;
	_bool						m_bDMG;
	_int						m_iDMGNum;

	_bool						m_bATK;

	_float						m_fHibernateTime;
	_bool						m_bHiber;

	_float						m_fSkillTime;
	_int						m_iRandPetternNum;
	_bool						m_bPettern[5];

	_bool						m_bSkillStart;
	_int						m_iDownCnt;

	_bool						m_bDie;
	_float						m_fDieTime;
	_float						m_fAlpha;
	_vec3						m_vViewPos;

private:
	LPD3DXMESH					m_pSphereMesh;
	LPD3DXBUFFER				m_pSphereAdjancey;

	LPD3DXMESH					m_pSphere2Mesh;
	LPD3DXBUFFER				m_pSphere2Adjancey;

	_float						m_fAttackRadius;

	const _matrix*				m_pmatRForearm;
	const _matrix*				m_pmatLForearm;

private:
	const _matrix*				m_pFin1Matrix;
	const _matrix*				m_pFin2Matrix;

	_vec3			m_FinLocalpos1;
	_vec3			m_FinLocalpos2;
	_vec3			m_Finger1Pos;
	_vec3			m_Finger2Pos;

	const _matrix*				m_pFin1Matrix2;
	const _matrix*				m_pFin2Matrix2;

	_vec3			m_FinLocalpos12;
	_vec3			m_FinLocalpos22;
	_vec3			m_Finger1Pos2;
	_vec3			m_Finger2Pos2;

private:	// 시작 패턴
	_bool		m_bStart;
	_bool		m_bOpening;



private:
	void	Set_Data(void);

private:
	void	Set_ContantTable(void);

private:	// 충돌
	void	Check_Collison(void);

private:	// 패턴
	void	Compute_Dir(void);
	void	Pattern_Move(void);
	void	Pattern_Hibernate(const float& fTimeDelta);
	void	Attack_Player(void);
	
	void	Pattern_Skill(void);

	void	Fire_Skill5(void);
	void	Fire_Skill4(void);
	void	Fire_Skill3(void);

private:	// 이펙트 생성
	void	Create_Blood(_vec3 vPos);
	void	Create_HitShock(void);
	void	Create_MagicCircle(_int iIdx);
	void	Create_Smoke(_vec3 vPos);
	void	Create_IceUp(_vec3 vPos);
	void	Create_Wang();
	void	Create_Swirl();
	void	Create_SwordCircle(_vec3 vPos);

public:
	virtual _ulong Release(void);
};

#endif // Boss_h__


