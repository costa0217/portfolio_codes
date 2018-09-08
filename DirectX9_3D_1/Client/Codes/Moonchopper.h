/*!
 * \file Glitter.h
 * \date 2016/01/02 22:59
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Moonchopper 클래스
 *
 * TODO: StageOne 중간 보스
 *
 * \note
*/
#ifndef Moonchopper_h__
#define Moonchopper_h__

#include "Defines.h"
#include "Monster.h"

#include "Transform.h"
class CEffect;
namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CNaviCell;
}

class CMoonchopper : public CMonster
{
private:
	explicit CMoonchopper(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoonchopper(void);

public:
	_vec3*	Get_LeftHandPos(){return &m_vLeftHand;}
	_vec3*	Get_LeftElbowPos(){return &m_vLeftElbow;}

	_vec3*	Get_RightHandPos(){return &m_vRightHand;}
	_vec3*	Get_RightElbowPos(){return &m_vRightElbow;}

public:
	virtual _int Update(const float& fTimeDelta);
	virtual	void Render(void);
	virtual	void Render_Shadow(void);

public:
	static	CMoonchopper*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool bInput;

	_vec3	m_vPlayerPos;			// 플레이어 Position
	_bool	m_bRotationOnce;		// Run시작할때만 각도 계산하려고

	vector<Moonchopper::MOONCHOPPERANIMATION>	m_vecInPattern;		// 근거리패턴 
	
	_int	m_iNearPattern;		// 근거리패턴 인덱스
	_int	m_iRandNum;			// 패턴에 쓰일 변수
	_float	m_fRunDistance;		// 달리기 멈추는 기준
	_bool	m_bRun;				// 달리고 있는지
	_bool	m_bLowHealth;		// 체력 낮을 때
	_bool	m_bLowHealthOnce;	// m_bLowHealth 제한주려고
	_bool	m_bDie;				// 죽은상태
	_vec3	m_vSpin;			// 스핀할 때 방향

	_bool	m_bDamage;			// 피격되었는지
	_bool	m_bDisHP;			// HP감소

	_bool			m_bEffectOnce;
	const _matrix*	m_pmatForearm_R; // 요격에 쓰는 오른손 행렬

	_int	m_iSoundStep;		// 발소리 순차적으로 재생하기위한 변수.
	_float	m_fStepTime;		// 발소리 사운드 타임.

	CEffect*		m_pSpinEffect; // 폭발 이펙트

	_float	m_fOriginAngle;		// 마지막 각도값
	_vec3	m_vOriginDir;		// 마지막 방향벡터



	const	_matrix*		m_pmatLeftHand;
	const	_matrix*		m_pmatRightHand;
	const	_matrix*		m_pmatLeftElbow;
	const	_matrix*		m_pmatRightElbow;

	_vec3				m_vLeftHand;
	_vec3				m_vRightHand;

	_vec3				m_vLeftElbow;
	_vec3				m_vRightElbow;

	
			
private:
	void	KeyInput();

private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);
	void Set_ContantTable(void);

private:
	virtual void Check_Monster_Satae(void);
	virtual void AnimationPattern_Immediately_Change(void);
	virtual void Check_PlayerAttack_AccordingToHP(void);
	virtual void BattleAnimationPattern(void);
	virtual void AnimationTimeChange(void);
	virtual void SoundPlayer(void);

private:
	void	Compute_AngleToPlayer();	// 플레이어 바라보게 돌릴 각도 계산
	void	Set_AnimationSyncInfo();	// 불러온 싱크정보중에서 현재 애니메이션에 맞는 정보를 셋해준다.
	_float	Compute_DisToPlayer();		// 플레이어와의 거리 계산
	void	Load_Pattern();				// 패턴 초기화 설정
	_bool	Pattern_Near_Distance();	// 근거리 패턴 (범위밖이면 false반환)
	_bool	Pattern_Out_Distance();		// 멀리 있을 경우 달려오기
	void	Pattern_LowHealth();		// 체력이 낮을 때 (동작1가지)

public:
	virtual _ulong Release(void);

};
#endif // CMoonchopper_h__

