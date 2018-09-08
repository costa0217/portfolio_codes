/*!
 * \file Monster.h
 * \date 2015/12/13 16:03
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Monster의 최상위 class.
 *
 * \note
*/

#ifndef Monster_h__
#define Monster_h__

#include "Defines.h"
#include "GameObject.h"

#include "Transform.h"
#include "BossFaceUI.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CNaviCell;
}

class CPlayer;
class CMonster : public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster(void);

public:
	virtual _int Update(const _float& fTimeDelta) { return 0; }
	virtual void Render(void);

public: //getter setter
	void SetObjectPos(_vec3 vPos) { m_pTransCom->m_vPosition = vPos; }
	void SetObjectAngleY(_float fAngleY) { m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fAngleY; }

	Engine::CTransform*	Get_TransCom(void) { return m_pTransCom; }
	Engine::CDynamicMesh* Get_MeshCom(void) { return m_pMeshCom; }

	//Move 충돌 관련
	_bool	Get_PlayerDistanceCheck(void) { return m_bPlayerDistanceCheck; }
	_float				Get_MoveRadius(void) { return m_fMoveRadius; }
	_bool	Get_MoveCollCheck(void){ return m_bMoveCollCheck;}

	//충돌 관련
	boost::unordered_map<int, vector<CLIENTBOXINFO*>>* Get_MonsterMapAttackBox(void) { return &m_mapAttackBox; }
	_int	Get_MonsterAnimationIndex(void) { return m_iMonsterAnimationIndex; }

	//HP&공격력
	CHARACTERINFO	Get_MonsterInfo(void) { return m_tMonsterInfo; }
	_float			Get_MonsterAttackValue(void) { return m_tMonsterInfo.fAttack; }
	//공격에 따른 다운 수치
	_float			Get_MonsterDownValue(void) { return m_fMonsterDownValue; }

	//이펙트 관련
	//플레이할 이펙트 데이터 이름, 애니메이션 시간, 필요한 행렬 주소, 로컬 위치, 부울변수Number / 이펙트 변경 각도, 재생이 끝난후 true면 다시 재생, false면 삭제
	void Play_Effect(TCHAR* pName, _float fCheckEndPeriodMulTime,  const _matrix* pMatrix, _vec3 vLocalPos, _int iboolNumber, _bool bReStart = false, _bool bHDR = true, _vec3 vSize = _vec3(0.f, 0.f, 0.f), _vec3 vAngle = _vec3(0.f, 0.f, 0.f));

	//등장여부 관련
	_bool	Set_Appear(_bool bAppear){ m_bAppear = bAppear; }

protected: //변수
#pragma region Defualt
	Engine::CDynamicMesh*	m_pMeshCom;
	Engine::CTexture*			m_pTexCom;
	Engine::CTransform*		m_pTransCom;	//Player를 제외한 나머지는 CDynamicMesh
	Engine::CNaviCell*		m_pNaviCell;	//NavigationMesh Cell ㅊㅂㅇ
	LPD3DXEFFECT			m_pEffect;
	LPD3DXEFFECT			m_pShadowEffect;

	CPlayer*				m_pPlayer;

	_float					m_fAngleX;
	_float					m_fAngleZ;

	_bool					m_bAppear; // true == spawn, false == 나중에 spawn

#pragma endregion
#pragma region 몬스터의 타입과 상태에 대한 열거형
	//몬스터와 연관된 열거형들
	enum MonsterType	{ MT_Boss, MT_Monster};	//각 몬스터에서 초기화
	enum MonsterState	{ MS_Attack, MS_Idle, MS_Damage, MS_Super}; //각 몬스터에서 초기화

	MonsterType		m_eMonsterType;
	MonsterState	m_eMonsterState;

	CHARACTERINFO	m_tMonsterInfo;
#pragma endregion
#pragma region 시간 관련 변수들
	_float		m_fTimeDelta; //업데이트에서 실제 시간 차이를 받아서 필요한 곳에서 사용할 변수
	_float		m_fRenderTimeDelta;	//랜더에서 사용할 float변수
	_float		m_fAnimationSpeed;	//애니메이션의 속도를 조절할 변수

	_bool		m_bSoundPlay[20]; //사운드를 재생하기 위해 선언
	_int		m_iAnimationSoundCheck; //사운드 관련 변수를 초기화 하기 위해 선언
#pragma endregion
#pragma region 충돌 관련 변수들
	_bool		m_bPlayerDistanceCheck;
	_bool		m_bPlayerAttack_Body_CollisionCheck;
	_bool		m_bPlayerEffect_Body_CollisionCheck;
	_bool		m_bPlayerAttack_MyAttack_CollisionCheck;

	_vec3		m_vCenter;	//객체의 중심값
	_float		m_fRadius;

	//충돌관련
	_int	m_iMonsterAnimationIndex;
	boost::unordered_map<int, vector<CLIENTBOXINFO*>>	m_mapAttackBox;	// 애니메이션 인덱스 + 바운딩박스 vector
	vector<CLIENTBOXINFO*>				m_vecDamageBox;	// 애니메이션 인덱스 + 바운딩박스 vector
	SPHEREINFO*							m_pSphere;	// 바운딩스피어
	_bool								m_bMoveCollCheck; // (Moonchopper의 경우 spin할때) 충돌체크 여부에 관한 변수 true면 검사

	//애니메이션 싱크관련
	boost::unordered_map<int, vector<FOOTANISYNC*>>	m_mapAniSync;	// 애니메이션 인덱스 + 바운딩박스 vector

	_vec3	m_vMoveMin, m_vMoveMax, m_vMoveCenter;
	_float	m_fMoveRadius;		//무브 스피어의 반지름

	_float	m_fMaxDamageValue;		// 몬스터 다운 기준
	_float	m_fDownValue;			// 몬스터에 쌓이는 다운 수치
	_float	m_fMonsterDownValue;	// 몬스터 공격에 따른 다운 수치
	_vec3	m_vCollisionPos;	// 충돌 위치 데미지 폰트를 띄우기 위해 선언
#pragma endregion
#pragma region 네비게이션 관련
	_vec3		m_vOriginalPos;		//Navigation판단용 원래위치 ㅊㅂㅇ
	_vec3		m_vAfterPos;		//Navigation판단용 Move후 위치 ㅊㅂㅇ
#pragma endregion
#pragma region 보스에서만 사용하는 변수
	CBossFaceUI*	m_pBossFaceUI;	//블러드 로드, 글리터만 사용할꺼야
	_bool			m_bCameraModeOn;
	_bool			m_bCameraModeCheck;
#pragma endregion
#pragma region 싱크 보다 조금/많이 Position을 이동시키기위한 변수
	_float		m_fDistanceSize;

	//이펙트 관련
	_bool	m_bCreateEffect[50];
#pragma endregion
protected: //Monster에서 받아서 공통적으로 사용될 함수들
	void SoundEffectPlay(SOUND::EFF eSoundType, _float fCheckEndPeriodMulTime, _int iboolNumber, _float fVolume = 1.f);
#pragma region Move, Box Loading
	void MonsterMoveFunction(const char* pFrameName);
	void MonsterMoveFunctionTwo(_bool bCompulsion, _bool bLeftAndRihgt, _bool bFrontAndBack);
	void Check_Navigation(void); // 네비메쉬 타는 부분.
	void Load_AttackBox(TCHAR* pBoundingBoxPath);	// 공격 바운딩박스 로드
	void Load_DamageBox(TCHAR* pBoundingBoxPath);	// 피격 바운딩박스 로드
#pragma endregion
#pragma region Move, Sync Loading
	void Load_FootAnimationSync(TCHAR* pSyncPath);	// 싱크 로드
#pragma endregion
#pragma region 충돌 관련 함수들
	void CollisionCheckFunction(void);				//충돌 함수들을 호출 하는 부분
	void PlayerDistanceCheck(void);					//몬스터와 플레이어의 거리를 체크하는 함수
	void PlayerAttackBox_And_MyBodyBox(void);		//플레이어의 공격과 나의 피격 박스
	void PlayerEffectBox_And_MyBodyBox(void);		//플레이어의 이펙트 와 나의 피격 박스
	void PlayerAttackBox_And_MyAttackBox(void);		//플레이어의 공격박스와 나의 공격 박스
	bool ObjectBox_And_MyMoveBox(void);				//오브젝트박스와 나의 무브 박스
	bool CDynamicMesh_And_MyMoveBox(void);			//다이나믹 박스들끼리의 충돌
	void Initialize_Min_Max_Radius(void);
	void Update_Monster_AnimationIndex();			//메쉬 애니메이션과 다르면 갱신?
#pragma endregion
protected: //자식들에서 반드시 정의 되어야 하는 함수들
#pragma region 가상 함수
	virtual void Check_Monster_Satae(void)					PURE; //애니메이션과 특정 조건들에 의해 몬스터의 상태를 정해 주는 곳.
	virtual void AnimationPattern_Immediately_Change(void)	PURE; //조건에 따라 애니메이션 패턴을 즉각적으로 바꾸어 줘야 하는 부분(플레이어에게 공격 당했을때 등)
	virtual void Check_PlayerAttack_AccordingToHP(void)		PURE; //플레이어의 공격에 따른 에너지 관리
	virtual void BattleAnimationPattern(void)				PURE; //몬스터의 AI를 결정하는 곳
	virtual void AnimationTimeChange(void)					PURE; //몬스터 애니메이션의 시간을 바꿔주는 곳		
	virtual void SoundPlayer(void)							PURE; //사운드를 플레이한다. 맞는 동작은 Check_PlayerAttack_AccordingToHP 해주고 나머지만 해주면 될듯
#pragma endregion


//디버깅 디버깅 디버깅 디버깅 디버깅 디버깅 디버깅 디버깅 디버깅 디버깅 디버깅
#ifdef _DEBUG
protected:
	void	Render_AttackBox();				// 바운딩박스 그리기
	void	Render_DamageBox();				// 바운딩박스 그리기
	void	Render_BoudingSphere();				// 바운딩스피어 그리기

	ID3DXMesh*				m_pSphereMesh;		// 경계구 그리기용 메쉬 & 버퍼
	LPD3DXBUFFER			m_pSphereBuffer;	
#endif

private:
	virtual HRESULT Initialize(void) { return S_OK; }
	virtual HRESULT Add_Component(void) { return S_OK; }
	virtual void Set_ConstantTable(void) { }

public:
	virtual _ulong Release(void);
};
#endif // Monster_h__