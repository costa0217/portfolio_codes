/*!
 * \file Glitter.h
 * \date 2015/12/29 13:07
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Glitter 클래스
 *
 * TODO: StageOne 중간 보스
 *
 * \note
*/
#ifndef Glitter_h__
#define Glitter_h__

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

class CGlitter : public CMonster
{
private:
	explicit CGlitter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGlitter(void);

public:
	virtual _int Update(const float& fTimeDelta);
	virtual void Render(void);
	virtual void Render_Shadow(void);

public:
	static	CGlitter*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool bInput;

	_vec3	m_vPlayerPos;			// 플레이어 Position
	_bool	m_bRotationOnce;		// Run시작할때만 각도 계산하려고

	vector<Glitter::GLITTERANIMATION>	m_vecInPattern;		// 근거리패턴 
	vector<Glitter::GLITTERANIMATION>	m_vecMidPattern;	// 중간거리패턴 
	vector<Glitter::GLITTERANIMATION>	m_vecOutPattern;	// 원거리패턴 

	_int	m_iNearPattern;		// 근거리패턴 인덱스
	_int	m_iMidPattern;		// 중간거리패턴 인덱스
	_int	m_iFarPattern;		// 원거리패턴 인덱스

	_float	m_fRunDistance;		// 달리기 멈추는 기준
	_bool	m_bRun;				// 달리고 있는지
	_bool	m_bLowHealth;		// 체력 낮을 때
	_bool	m_bLowHealthOnce;	// m_bLowHealth 제한주려고
	_bool	m_bDie;				// 죽은상태

	const _matrix*	m_pmatFireball; // 볼트 행렬
	const _matrix*	m_pmatAttachLH; // AttachLH 행렬
	const _matrix*	m_pmatAttachRH; // AttachRH 행렬
	const _matrix*	m_pmatSMDImport; // 중앙 행렬
	_bool			m_bEffectOnce;  // 이펙트객체 한번만 생성
	CEffect*		m_pLHShortEffect; // LH날리기 전에 손 따라다니는 이펙트
	CEffect*		m_pALHShortEffect; // 양손공격 L이펙트
	CEffect*		m_pARHShortEffect; // R이펙트
	CEffect*		m_pExplosionEffect; // 폭발 이펙트

	_bool	m_bDamage;			// 피격되었는지
	_bool	m_bDisHP;			// HP감소

	_int	m_iSoundStep;		// 발소리 순차적으로 재생하기위한 변수.
	_float	m_fStepTime;		// 발소리 사운드 타임.

	_float	m_fOriginAngle;		// 마지막 각도값
	_vec3	m_vOriginDir;		// 마지막 방향벡터

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
	_bool	Pattern_Mid_Distance();		// 중간거리 패턴
	_bool	Pattern_Far_Distance();		// 원거리 패턴
	_bool	Pattern_Out_Distance();		// 멀리 있을 경우 달려오기
	void	Pattern_LowHealth();		// 체력이 낮을 때 (동작3가지)
public:
	virtual _ulong Release(void);

};
#endif // Glitter_h__