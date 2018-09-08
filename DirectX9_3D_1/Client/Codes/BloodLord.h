/*!
 * \file CBloorLord.h
 * \date 2015/12/11 15 : 08
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief BloodLord 클래스 
 *
 * TODO: StageOne 최종 보스
 *
 * \note
*/
#ifndef BloodLord_h__
#define BloodLord_h__

#include "Defines.h"
#include "Monster.h"

#include "Transform.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CNaviCell;
	class CLayer;
}

class CBloodLord : public CMonster
{
private:
	explicit CBloodLord(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBloodLord(void);
	

public:
	virtual _int Update(const float& fTimeDelta);
	virtual void Render(void);

	virtual void Render_Shadow(void);

public:
	static	CBloodLord*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void	KeyInput();
	//////////////////////////////////////////////////////////////////////////

	enum BLOODSTATAE { BS_IDLE, BS_ATTACK, BS_SUPER, BS_BLINK, BS_RUN, BS_END };

	_float			m_fPlaySpeed;
	_bool			m_bStart;			// 컷신이후인지
	_bool			m_bCutScene1;		// 컷신중인지
	_bool			m_bCutScene2;		// 컷신중인지
	_bool			m_bCutSceneTerm;

	_bool			m_bIdle;			// 강제 대기상태
	_bool			m_bBlink;			// 블링크 상태인지 확인위해
	_bool			m_bSwamp;			// 블링크 상태인지 확인위해
	_bool			m_bRotation;		// 플레이어 바라보게
	_bool			m_bAttackDuring;	// 공격모션중인지
	_bool			m_bDamageSuperArmor;

	_bool			m_bDie;
	_bool			m_bRealDie;
	_float			m_fLowHealth;		// lowhealth 모션 플레이 해야하는지
	BLOODSTATAE		m_eCurState;

	_float	m_fCutSceneEnd;	// 컷신 텀
	_float	m_fCutSceneTerm;	// 컷신 텀

	_float	m_fNearDistance;	// 근거리 값
	_float	m_fMidDistance;		// 중간거리 값
	_float	m_fFarDistance;		// 원거리 값

	_int	m_iNearPattern;		// 근거리패턴 인덱스
	_int	m_iMidPattern;		// 중간거리패턴 인덱스
	_int	m_iFarPattern;		// 원거리패턴 인덱스

	_bool	m_bNearPattern;		// 근거리 패턴이었는지	(피격시 피격모션 후 다시 인덱스 돌려주기 위해)
	_bool	m_bMidPattern;		// 원거리 패턴이었는지
	_bool	m_bFarPattern;		// 중간거리패턴이었는지
	_bool	m_bRun;				// 달리기 동작 2번 넣어주려고

	_bool	m_bDamage;			// 피격되었는지
	_bool	m_bDisHP;			// HP감소

	_bool	m_bDownState;		// 다운상태인지
	_bool	m_bDownTime;		// 다운상태인지
	_bool	m_bDisDown;			// 다운게이지 50% 줄었는지

	_float	m_fDownTime;		// 다운시작시간
	_float	m_fDownOverTime;	// 다운계산시간

	_bool	m_bInsideNavi;

	_float	m_fRunDistance;		// 달리기 범위
	
	_float	m_fBlinkTime;		// 블링크 시작되는 시간
	_float	m_fBlinkOverTime;	// 블링크 진행되는 시간
	_float	m_fBlinkEffTime;	// 블링크 시작 동작 자를 시간
	_float	m_fBlinkEndTime;	// 블링크 시작 동작 자를 시간
	_int	m_iBlinkNot;
	_bool	m_bBlinkEnd;

	_float	m_fSuperTime;		// 피격되고 슈퍼아머상태 되는 시간
	_float	m_fSuperOverTime;	// 슈퍼아머 진행되는 시간

	_float	m_fOrigAngle;		// 플레이어 바라보기위한 회전 전 각도?
	_float	m_fAfterAngle;		// 회전해야할 각도
	
	_matrix m_matRotation;		// 공중회전용 행렬
	_float	m_fRevoluAngle;		// attack_skill에서 몬스터 회전시킬 각도 값

	_float	m_fDancingDistance_x;	// attack_dancing에서 사용할 x거리
	_float	m_fDancingDistance_z;	// attack_dancing에서 사용할 z거리

	_vec3	m_vPlayerPos;			// 플레이어 Position

	_matrix	m_matDancing;			// attack_dancing에서 사용
	_matrix m_matRotationDefault;	// -90도 회전하기 위한 회전행렬

	vector<BloodLord::BLOODLORDANIMATION>	m_vecInPattern;		// 근거리패턴 
	vector<BloodLord::BLOODLORDANIMATION>	m_vecMidPattern;	// 중간거리패턴 
	vector<BloodLord::BLOODLORDANIMATION>	m_vecOutPattern;	// 원거리패턴 

	_bool			m_bMeteoSkill;			// 메테오 상태인지 확인위해
	_bool			m_bMeteo;

	_vec3	m_vMonsterDir;

	_float	m_fRotSpeed;
	_bool	m_bRot_BreathEnd;
	_matrix m_matBloodBreath;
	_vec3	m_vDir;

	_bool	m_bCutsceneSoundPlay[20];
	_bool	m_bCutsceneFadeCheck;
	_bool	m_bCutsceneFadeCheckTwo;
private:	// ★ 트레일
	const	_matrix*		m_pmatLeftElbow;
	const	_matrix*		m_pmatRightElbow;

	const	_matrix*		m_pmatLeftOne;
	const	_matrix*		m_pmatLeftTwo;
	const	_matrix*		m_pmatLeftThree;
	const	_matrix*		m_pmatRightOne;
	const	_matrix*		m_pmatRightTwo;
	const	_matrix*		m_pmatRightThree;
	
	_vec3				m_vLeftElbow, m_vRightElbow;
	_vec3				m_vLeftOne,m_vLeftTwo,m_vLeftThree;
	_vec3				m_vRightOne,m_vRightTwo,m_vRightThree;
public:	// 트레일
	_vec3*				Get_LeftElbowPos(){return &m_vLeftElbow;}
	_vec3*				Get_RightElbowPos(){return &m_vRightElbow;}
	_vec3*				Get_LeftOnePos(){return &m_vLeftOne;}
	_vec3*				Get_LeftTwoPos(){return &m_vLeftTwo;}
	_vec3*				Get_LeftThreePos(){return &m_vLeftThree;}
	_vec3*				Get_RightOnePos(){return &m_vRightOne;}
	_vec3*				Get_RightTwoPos(){return &m_vRightTwo;}
	_vec3*				Get_RightThreePos(){return &m_vRightThree;}

	const _matrix*				Get_RightOneMatrix(){return m_pmatRightOne;}
	const _matrix*				Get_RightTwoMatrix(){return m_pmatRightTwo;}
	const _matrix*				Get_RightThreeMatrix(){return m_pmatRightThree;}
	const _matrix*				Get_LeftOneMatrix(){return m_pmatLeftOne;}
	const _matrix*				Get_LeftTwoMatrix(){return m_pmatLeftTwo;}
	const _matrix*				Get_LeftThreeMatrix(){return m_pmatLeftThree;}


private:
	_bool	Check_Player_Distance(float fDist);	//플레이어와 거리 계산
	void	Check_Player_Radian();				//플레이어 방향으로 회전
	void	Check_Turn();				//플레이어 방향으로 회전
	void	Check_Rotation();				//플레이어 방향으로 회전

	_bool	Pattern_Near_Distance();// 근거리 패턴 (범위밖이면 false반환)
	_bool	Pattern_Mid_Distance();	// 중간거리 패턴
	_bool	Pattern_Far_Distance();	// 원거리 패턴
	_bool	Pattern_Out_Distance();	// 멀리 있을 경우 블링크

	void	Load_Pattern();			// 패턴 초기화 설정
	void	Check_Skill_Blink();	// 블링크 상태인지 확인		/ 다음 동작 설정
	void	Check_Skill_Swamp();	// 피웅덩이 상태인지 확인	/ 다음 동작 설정
	void	Check_Skill_Dancing();	// 공중회전 상태인지 확인	/ 회전 설정
	void	Check_Skill_BloodField();
	void	Check_Skill_BloodBreath();

	void	Check_DamageAfter_SuperArmor();		// 피격후 슈퍼아머 상태 확인

	void	Set_BlinkPosition();	// blink 끝나고 나타날 위치 설정

	void	Check_NearAttackPower();	// 공격에 따라 공격력 설정
	void	Check_MidAttackPower();		
	void	Check_FarAttackPower();	

	void	Check_DownState();			// 다운상태일때 체크

	void	Check_LowHealth();			// 피 많이 닳았을때 공격모션-lowhealth모션-공격모션 이렇게 플레이
	void	Check_DieMotion();			// 죽는 모션

	void	Set_BloodFieldPattern(_int iIdx);
	void	BloodFieldPattern_0();
	void	BloodFieldPattern_1();
	void	BloodFieldPattern_2();

	void	SwampPattern_0();
	void	SwampPattern_1();
	void	SwampPattern_2();

	void	BatEffect();

	_int	m_iBloodField;
	_float	m_fBreathSize;
	_float	m_fBreathTime;
	_float	m_fBreathOverTime;
	_bool	m_bBreath;
	//////////////////////////////////////////////////////////////////////////
	void	Create_TrailPos();

public:
	void Set_Layer(Engine::CLayer* pLayer){m_pLayer = pLayer;}

private:
	Engine::CLayer*		m_pLayer;

private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);
	void Set_ContantTable(void);

private:
	void CameraModeCheck(void);	//플레이어의 위치, 몬스터의 피 등과 관련하여 카메라모드를 on한다.

	virtual void Check_Monster_Satae(void);
	virtual void AnimationPattern_Immediately_Change(void);
	virtual void Check_PlayerAttack_AccordingToHP(void);
	virtual void BattleAnimationPattern(void);
	virtual void AnimationTimeChange(void);
	virtual void SoundPlayer(void);

public:
	virtual _ulong Release(void);

};
#endif // Player_h__