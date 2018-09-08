/*!
 * \file Player.cpp
 * \date 2015/11/27 1:09
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CGameObject를 상속 받되 싱글톤으로 구현.
 *
 * \note
*/

#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

#include "Transform.h"
#include "Input.h"
#include "PlayerDynamicMesh.h"
#include "Monster.h"

namespace Engine
{
	class CPlayerDynamicMesh;
	class CTransform;
	class CNaviCell;
	class CTexture;
}

class CPlayerSkill;
class CPlayer : public Engine::CGameObject
{
	DECLARE_SINGLETON(CPlayer)

private:
	explicit CPlayer(void);
	virtual ~CPlayer(void);

public: //함수
#pragma region 외부에서 호출 되는 함수들
	virtual _int Update(const float& fTimeDelta);
	virtual void Render(void);
	_bool PlayerSkill_Input(_int iSKillNumber);	//PlayerSkill class에서 호출하면서 사용한다.
	void Player_AttackSuccess_InfoChange(void); //플레이어의 공격이 성공 한후 연관된 수치를 바꾸어주는 곳
#pragma endregion
#pragma region Getter, Setter
	CPlayer* ReturnThisPoint(void) { return this; }	//플레이어의 주소 반환
	void SetEnvironmentTexCom(Engine::CTexture* SetPoint) { m_pEnvironmentTexCom = SetPoint; }
	void SetNaviCell(Engine::CNaviCell* SetPoint) { m_pNaviCell = SetPoint; }
	_float GetDegreeAngleY(void) { return D3DXToDegree(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]); } //플레이어 Y축에 대한 디그리 회전값 반환
	_vec3 GetPos(void) { return m_pTransCom->m_vPosition; }
	void SetPos(_vec3 vSetPos) { m_pTransCom->m_vPosition = vSetPos; }
	_float GetAngleY(void) { return m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]; }
	void SetAngleY(_float fSetAngle) { m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fSetAngle; }
	Engine::CPlayerDynamicMesh* Get_MeshCom(void) { return m_pMeshCom; }
	Engine::CTransform*	Get_TransCom(void) { return m_pTransCom; }
	Player::PLAYERANIMATION Get_PlayerAnimationType(void) { return m_ePlayerAnimation; }
	void Set_PlayerAnimationType(Player::PLAYERANIMATION eAnimation) { m_ePlayerAnimation = eAnimation; m_pMeshCom->Set_AnimationSetThree(m_ePlayerAnimation); }

	//플레이어 정보
	_float GetPlayerMaxHP(void) { return m_fPlayerMaxHP; }
	_float GetPlayerHP(void) { return m_fPlayerHP; }
	_float GetPlayerMaxStamina(void) { return m_fPlayerMaxStamina; }
	_float GetPlayerStamina(void) { return m_fPlayerStamina; }
	_float GetPlayerMaxSP(void) { return m_fPlayerMaxSP; }
	_float GetPlayerSP(void) { return m_fPlayerSP; }
	_float GetPlayerAngle(void) {return m_fOriginalAngleY; }

	_float GetPlayerAttackValue(void) { return m_fPlayerAttackValue; }
	_float GetPlayerDownValue(void) { return m_fPlayerDownValue; }
	_int GetPlayerAttackCombo(void) {return m_iPlayerAttackCombo; }

	void SetInfoInitialize(void) { 
		m_fPlayerHP = m_fPlayerMaxHP;
		m_fPlayerMaxSP = m_fPlayerMaxSP;
		Cloth_Reset();
	}

	//충돌 관련
	CLIENTBOXINFO* Get_ClientBoxInfo(void) { return m_pAttackBox[m_ePlayerAnimationCheck]; }	//현재 애니메이션의 어택 박스를 반환한다.
	_bool	Get_Check_EndPeriodMulTime_Start(void) { return m_pMeshCom->Check_EndPeriodMulTime(Get_ClientBoxInfo()->fStartTime); }
	_bool	Get_Check_EndPeriodMulTime_End(void) { return m_pMeshCom->Check_EndPeriodMulTime(Get_ClientBoxInfo()->fEndTime); }
	const _matrix*	GetpAttachment_RH_Matrix(void) { return m_pAttachment_RH_Matrix; }

	_float Get_MoveRadius(void) { return m_fMoveRadius; }
	_bool	GetInGame(void) { return m_bInGame; }
	void	SetInGame(_bool SetBool) { m_bInGame = SetBool; }

	_bool	GetBerserkerCheck(void) { return m_bBerserkerCheck; }

	//걍 public으로함
	CPlayerSkill*	m_pPlayerSkill;

#pragma region //플레이어 상태 관련 열거형
public:
	enum PlayerState { PS_BasicAttack, PS_BreakAttack, PS_SmashAttack, PS_SmashAttack_0, PS_SkillAttack, PS_Run, PS_Walk, PS_Idle, PS_Down, PS_Damage };
	enum PlayerDefenseState { PS_Basic, PS_SuperArmour };
	PlayerState Get_PlayerState(void) { return m_ePlayerState; }

private:
	PlayerState			m_ePlayerState;
	PlayerDefenseState	m_ePlayerDefenseState;
#pragma endregion

#pragma endregion

private: //변수
#pragma region Default
	Engine::CPlayerDynamicMesh*	m_pMeshCom;	//다이나믹 매쉬 객체
	Engine::CTransform*		m_pTransCom;	//트랜스폼 객체
	Engine::CNaviCell*		m_pNaviCell;	//NavigationMesh Cell ㅊㅂㅇ
	LPD3DXEFFECT			m_pEffect;		//셰이더
	LPD3DXEFFECT			m_pShadowEffect; //★ 재훈추가 - 그림자 쉐이더
	LPD3DXEFFECT			m_pVelocityEffect;
	Engine::CTexture*		m_pEnvironmentTexCom; // 환경
	_matrix						m_matPreView;
	_vec3						m_vAttackStartPos;

	_float					m_fTimeDelta;		//업데이트에서 실제 시간 차이를 받아서 필요한 곳에서 사용할 변수
	_float					m_fRenderTimeDelta;	//랜더에서 사용할 float변수
	_float					m_fAnimationSpeed;	//m_fRenderTimeDelta 와 m_fMoveTimeDelta 2개를 동시에 조절하기 위한 변수 2015.12.06 애니메이션 구조 바꾸면서 m_fMoveTimeDelta 삭제함
#pragma endregion
#pragma region HP, Stamina, SP, Attack 관련
	_float					m_fPlayerMaxHP;
	_float					m_fPlayerHP;
	_float					m_fPlayerMaxStamina;
	_float					m_fPlayerStamina;
	_float					m_fPlayerMaxSP;
	_float					m_fPlayerSP;

	_float					m_fPlayerPureAttackValue;	//전체적인 공격 밸런스를 잡기 위한 변수
	_float					m_fPlayerAttackValue;		//실제 공격 수치
	_float					m_fPlayerDownValue;			//다운 수치
	_int					m_iPlayerAttackCombo;		//콤보
	_float					m_fPlayerAttackComboTime;	//콤보 초기화 타임

	_float					m_fWakeUpAttackMagnification; //각성 수치
	Player::PLAYERANIMATION	m_ePlayerAttackStaminaAnimationCheck;

	_float					m_fPlayerRunFullTime;
	_bool					m_bPlayerRunFull;

	_bool					m_bBerserkerCheck;
	_float					m_fBerserkerFullTime;
	_float					m_fBerserkerTime;

	_float					m_fBerserkerCreateTime;
#pragma endregion
#pragma region 각도, 좌우구분, 타입 구분
public:
	_float					m_fOriginalAngleY;	//플레이어 Y의 디그리 각도
private:
	//플레이어 키입력에 따른 고정 방향 각도
	enum AngleFix{ AF_LeftFront, AF_RightFront, AF_LeftBack, AF_RightBack, AF_Front, AF_Back, AF_Left, AF_Right, AF_End};
	_float					m_fAngleFixed[AF_End];

	//플레이어의 좌우를 구분하기 위해 선언한 변수들
	enum FootFront{ Left_Foot_Front, Right_Foot_Front, FF_End};
	const _matrix* matLFoot;
	const _matrix* matRFoot;

#pragma endregion
#pragma region 애니메이션 인덱스, 키 관련
	Player::PLAYERANIMATION m_ePlayerAnimation;	//재생해야할 플레이어 인덱스의 열거형
	_float					m_fIdleToRestTime;	//공격대기에서 쉬는 모션으로 가기위해 시간을 판단하기 위한 변수

	bool	m_bOnceSpaceKeyCheck;	//SpaceKey를 한 번만 체크하기 위해 선언 이미 눌렸으면 true 아직 안눌렸으면 false
	bool	m_bOnceLbuttonCheck;	//왼쪽 버튼 클릭을 한번만 체크하기 위한 변수
	bool	m_bOnceRbuttonCheck;	//오른쪽 버튼 클릭을 한번만 체크하기 위한 변수
#pragma endregion
#pragma region 공격, 충돌 관련 변수
	//플레이어 공격 관련
	enum BasicAttack { BA_1a, BA_1b, BA_2, BA_3, BA_4, BA_End};
	enum SmashAttack { SA_0, SA_1, SA_2, SA_3, SA_4a, SA_4b, SA_End};
	BasicAttack BasicAttackOrder;
	SmashAttack SmashAttackOrder;

	_int	m_iChargeSlashNumber;	//차징을 몇번 했는지 에 대한 변수

	//플레이어 충돌 관련 변수들
	_bool			m_bMonsterAttack_Body_CollisionCheck;
	_bool			m_bMonsterEffect_Body_CollisionCheck;

	_bool			m_bPlayerHitCheck;	//플레이어가 공격을 1번만 받기 위해 선언
	CMonster*		m_pMonster; //플레이어를 때린 몬스터의 주소를 사용하기 위해 선언
	DamageDirectionType	m_eDamageDirectionType;	//피격 데미지 방향

	_vec3	m_vCollisionPos;	// 충돌 위치 데미지 폰트를 띄우기 위해 선언

	_float	m_fEffAttackValue;	// 충돌한 이펙트의 공격력을 사용하기위해 선언 ㅊㅂㅇ
	_float	m_fEffDownValue;	// 충돌한 이펙트의 다운력

	_float	m_fDownValue;
#pragma endregion
#pragma region 염색, 장비 교체 관련 변수
public:
	void Set_HairColor( _matrix matColor ) { matHairColor = matColor; m_pMeshCom->Set_HairColor(matHairColor);}
	void Set_UpperColor( _matrix matColor ) { matUpperColor = matColor; m_pMeshCom->Set_UpperColor(matUpperColor);}
	void Set_LowerColor( _matrix matColor ) { matLowerColor = matColor; m_pMeshCom->Set_LowerColor(matLowerColor);}
	void Set_HandColor( _matrix matColor ) { matHandColor = matColor;  m_pMeshCom->Set_HandColor(matHandColor);}
	void Set_FootColor( _matrix matColor ) { matFootColor = matColor; m_pMeshCom->Set_FootColor(matFootColor);}

	void Set_HairMesh( Engine::CPlayerDynamicMesh* pMesh ) { m_pHairMeshCom = pMesh; m_pMeshCom->Change_ClothMesh(0, m_pHairMeshCom->GetMeshCon());}
	void Set_UpperMesh( Engine::CPlayerDynamicMesh* pMesh ) { m_pUpperMeshCom = pMesh; m_pMeshCom->Change_ClothMesh(1, m_pUpperMeshCom->GetMeshCon());}
	void Set_LowerMesh( Engine::CPlayerDynamicMesh* pMesh ) { m_pLowerMeshCom = pMesh; m_pMeshCom->Change_ClothMesh(3, m_pLowerMeshCom->GetMeshCon());}
	void Set_HandMesh( Engine::CPlayerDynamicMesh* pMesh ) { m_pHandMeshCom = pMesh; m_pMeshCom->Change_ClothMesh(2, m_pHandMeshCom->GetMeshCon());}
	void Set_FootMesh( Engine::CPlayerDynamicMesh* pMesh ) { m_pFootMeshCom = pMesh; m_pMeshCom->Change_ClothMesh(4, m_pFootMeshCom->GetMeshCon());}

	//// 부위파괴된 메쉬를 로드하기 위해서 염색화면에서 고른 메쉬의 경로와 이름들을 세팅해준다.
	TCHAR m_pPath[6][128];
	TCHAR m_pOirgMeshName[6][128];
	TCHAR m_pMeshName[6][128];
	TCHAR m_pOrigFileName[6][128];
	TCHAR m_pFileName[6][128];
	void Initialize_TCHAR();
	void Set_MeshPath(int iIndex, TCHAR* pPath) { lstrcpy(m_pPath[iIndex], pPath); }
	void Set_MeshName(int iIndex, TCHAR* pFileName) { lstrcpy(m_pMeshName[iIndex], pFileName); }
	void Set_OrigMeshName(int iIndex, TCHAR* pFileName) { lstrcpy(m_pOirgMeshName[iIndex], pFileName); }
	void Set_MeshFileName(int iIndex, TCHAR* pFileName) { lstrcpy(m_pFileName[iIndex], pFileName); }
	void Set_OrigMeshFileName(int iIndex, TCHAR* pFileName) { lstrcpy(m_pOrigFileName[iIndex], pFileName); }
	void Ready_ClothMesh(void);	// 선택한 옷으로 부위파괴된 메쉬로드
	void Set_BrokenMesh();		// 옷 메쉬 변경하기위해 필요한 PlayerDynamicMesh 객체 생성
	void Change_BrokenMesh();	// 부위별로 많이 파괴되었을 경우 메쉬 교체

	void Undraw() { m_pMeshCom->Undraw(); }
	void Undraw_Hand2() { m_pMeshCom->Undraw_Hand2(); }
	void Draw() { m_pMeshCom->Draw(); }
	void Draw_Hand2() { m_pMeshCom->Draw_Hand2(); }

	void Cloth_Reset();


private:
	_matrix	matHairColor;
	_matrix	matUpperColor;
	_matrix	matLowerColor;
	_matrix	matHandColor;
	_matrix	matFootColor;

	Engine::CPlayerDynamicMesh*	m_pHairMeshCom;		//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pUpperMeshCom;	//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pLowerMeshCom;	//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pHandMeshCom;		//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pFootMeshCom;		//다이나믹 매쉬 객체

	Engine::CPlayerDynamicMesh*	m_pHairOrigMeshCom;		//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pUpperOrigMeshCom;	//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pLowerOrigMeshCom;	//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pHandOrigMeshCom;		//다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pFootOrigMeshCom;		//다이나믹 매쉬 객체


	Engine::CPlayerDynamicMesh*	m_pHair_Broken_MeshCom;	//부위파괴 다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pUpper_Broken_MeshCom;	//부위파괴 다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pLower_Broken_MeshCom;	//부위파괴 다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pHand_Broken_MeshCom;		//부위파괴 다이나믹 매쉬 객체
	Engine::CPlayerDynamicMesh*	m_pFoot_Broken_MeshCom;		//부위파괴 다이나믹 매쉬 객체
#pragma endregion
#pragma region 네비게이션 용 변수
	_vec3					m_vOriginalPos;	//Navigation판단용 원래위치 ㅊㅂㅇ
	_vec3					m_vAfterPos;	//Navigation판단용 Move후 위치 ㅊㅂㅇ
	_vec3					m_vEffectPos;

	_bool					m_bInGame;
#pragma endregion
#pragma region 이펙트, 사운드 관련
	const _matrix* m_pAttachment_LH_Matrix;
	const _matrix* m_pAttachment_RH_Matrix;
	const _matrix* m_pL_Foot_Matrix;
	const _matrix* m_pR_Foot_Matrix;

	_bool	m_bInitialize_CreateEffect_Bool;
	Player::PLAYERANIMATION	m_ePlayerAnimationCheck;
	_bool	m_bCreateEffect[50];

	float	m_fEffectMoveTime;	// 이펙트의 이동을 위한 시간값

	//사운드 관련
	_bool	m_bPlayerSound[5];
	_bool	m_bSwordAttack;		//검박스가 끝날때 true

#pragma endregion

private: //변수와 함수 같이 있는 곳
#pragma region 바운딩 박스 관련 변수, 함수들
	SPHEREINFO*					m_pSphere;		// 바운딩스피어
	CLIENTBOXINFO*				m_pAttackBox[Player::PlayerAnimation_End]; //공격 바운딩박스[인덱스]

	enum BodyType { BT_Head, BT_Upper, BT_LowerRight, BT_LowerLeft, BT_HandRight, BT_HandLeft, BT_FootRight, /*BT_FootLeft, */BT_End };
	CLIENTBOXINFO*				m_pDamageBox[BT_End];

	_float						m_fBoxHP[5];
	_bool						m_bBox[5];
	_int						m_iColBox;

	_vec3	m_vMoveMin, m_vMoveMax;
	_float	m_fMoveRadius;


	void	Load_AttackBox();	// 바운딩박스 로드
	void	Load_DamageBox();	// 피격 박스 로드
#pragma endregion
#pragma region 디버그용 전용 변수와 함수들 과 확인용 임시코드
#ifdef _DEBUG
private:
	ID3DXMesh*				m_pSphereMesh;		// 경계구 그리기용 메쉬 & 버퍼
	LPD3DXBUFFER			m_pSphereBuffer;

	void	Render_AttackBox();				// 공격 바운딩박스 그리기
	void	Render_DamageBox();				// 피격 바운딩박스 그리기
	void	Render_BoudingSphere();				// 바운딩스피어 그리기

public:
	bool		m_bClick;				// 바운딩박스
	bool		m_bShowBounding;
#endif
#pragma endregion

private: //함수
#pragma region Default
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);
	void Set_ContantTable(void);
#pragma endregion	
#pragma region 플레이터 타입 체크, 애니메이션 변경 관련
	void PlayerStateCheck(void);
	void Key_Input_Change_PlayerAnimationState(const float& fTimeDelta); //현재 애니메이션과 키, 마우스의 입력에 따라 ePlayerAnimation 값을 변화 시킨다.
	void AnimationPattern_Immediately_Change(void);	//조건에 따라 즉시 애니메이션 패턴을 바꿔주는 곳
	void Current_Animation_Change_Direction(Player::PLAYERANIMATION ePlayAnimationType, _float fCheckEndPeriodMulTime = 0.f); //조건에 만족하면 현재 애니메이션을 m_ePlayerAnimation으로 Change 해준다.
#pragma endregion
#pragma region 회전 및 좌우 체크
	void Key_Input_Rotation(void);						//키 입력에 따라 플레이어의 각도를 천천히 변화 시키는 함수
	void Change_AngleY(_float fAngleFixed);				//카메라와 플레이어의 각도 간의 관계를 생각하여 플레이어의 Y각도를 천천히 회전시킬 함수
	void Key_Input_Immediately_Rotation(void);			//키 입력에 따라 플레이어의 각도를 바로 변화 시키는 함수
	void Change_AngleY_Immediately(_float fAngleFixed);	//카메라와 플레이어의 각도 간의 관계를 생각하여 플레이어의 Y각도를 바로 회전시킬 함수	
	FootFront LeftAndRightCheck(void);					//왼쪽발이 앞에 있는지 오른쪾 발이 앞에있는지 체크s
#pragma endregion
#pragma region 인풋 관련
	void Current_Player_Animation_Type_Change_KeyInput(BYTE KeyFlag, Player::PLAYERANIMATION eChangePlayerAnimationType, _float fCheckEndPeriodMulTime = 0.f, _bool bOnceBool = false); //현재 실행중인 애니메이션의 인덱스 값과 키의 입력, 타이밍을 통해서 m_ePlayerAnimation의 상태를 바꿔준다.
	void Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::MOUSECLICK MouseFlag, Player::PLAYERANIMATION eChangePlayerAnimationType, _float fCheckEndPeriodMulTime = 0.f, _bool bOnceBool = false); //바꾼 상태를 통해 AnimationPattern_Immediately_Change에서 조건에 따라 애니메이션을 Set해준다.
	void Current_Player_Animation_Type_Change_SpaceInput(void);
	void BlockadeRush_MouseInput(void);	//봉쇄, 달리면서 봉쇄, 돌진 예외처리
	void Judgement_MouseInput(void);	//심판 기술 사용중 예외처리
#pragma endregion
#pragma region 충돌 관련
	void CollisionCheckFunction(void);			//충돌 함수들을 호출 하는 부분
	void MonsterAttackBox_And_MyBodyBox(void);	//몬스터의 공격과 나의 피격 박스
	void MonsterEffectBox_And_MyBodyBox(void);	//몬스터의 이펙트와 나의 피격 박스
	bool ObjectBox_And_MyMoveBox(void);			//오브젝트박스와 나의 무브 박스
	bool CDynamicMesh_And_MyMoveBox(void);		//다이나믹 박스들끼리의 충돌
#pragma endregion
#pragma region 기타
	void Player_InfoChange(void);				//공격력, 다운 수치등 실시간으로 바꿔야 하는곳
	void Player_UnderAttack_InfoChange(void);	//적에게 공격 을 받고 난후 연관된 정보를 관리 하는 곳
	void PlayerMoveFunction(void);
	void Check_Navigation(); // 네비메쉬 타는 부분
	void CreateEffectFunction(void);
	//플레이할 이펙트 데이터 이름, 애니메이션 시간, 필요한 행렬 주소, 로컬 위치, 부울변수Number / 이펙트 변경 각도, 재생이 끝난후 true면 다시 재생, false면 삭제
	void PlayEffect(TCHAR* pName, _float fCheckEndPeriodMulTime,  const _matrix* pMatrix, _vec3 vLocalPos
		, _int iboolNumber, _bool bReStart = false, _vec3 vSize = _vec3(0.f, 0.f, 0.f)
		, _vec3 vAngle = _vec3(0.f, 0.f, 0.f), _bool bNoTesting = false, _bool bDistort = false);	
	virtual void Render_Shadow(void);
	virtual void	Render_Velocity(void);
	void PlayShortEffects(TCHAR* pName, const _matrix* pPlayerMatrix, int iEffectCnt, _vec3 vPos, _vec3 vAngle = _vec3(0.f, 0.f, 0.f), _vec3 vSize = _vec3(0.f, 0.f, 0.f));
	void PlayerSound(void);
	void SoundEffectPlay(SOUND::EFF eSoundType, _float fCheckEndPeriodMulTime, _int iboolNumber, _float fVolume = 1.f);
#pragma endregion

public:
#pragma region 트레일
	const	_matrix*		m_pmatEye;
	_vec3				m_vEyePos;
	_vec3*				Get_EyePos(){return &m_vEyePos;}
#pragma endregion

public:
	virtual _ulong Release(void);
};
#endif // Player_h__
