#ifndef RoyalGuard_h__
#define RoyalGuard_h__

#include "Defines.h"
#include "Monster.h"

#include "Transform.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CNaviCell;
}

class CRoyalGuard : public CMonster
{
private:
	explicit CRoyalGuard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoyalGuard(void);

public:
	virtual _int Update(const float& fTimeDelta);
	virtual	void Render(void);
	virtual	void Render_Shadow(void);

public:
	static	CRoyalGuard*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	static	_vec3			m_vPos;

private:
	_int	m_iIdlePattern;
	_float	m_fOrigAngle;
	_float	m_fAttackDistance;	// 공격 범위
	_float	m_fFollowDistance;	// 따라가는 범위

	_float m_fDieTime;
	_float m_fDieOverTime;

	_bool	m_bAttack;			// 공격중인지
	_bool	m_bDamage;			// 피격되었는지
	_bool	m_bDisHP;			// HP감소
	_bool	m_bDie;
	_bool	m_bHP_0;

	_vec3	m_vMonsterDir;
	_matrix m_matRotationDefault;	// -90도 회전하기 위한 회전행렬

	vector<RoyalGuard::ROYALGUARDANIMATION>	m_vecAttackPattern;
	_int	m_iAttackPattern;		// Attack패턴 인덱스

	vector<RoyalGuard::ROYALGUARDANIMATION>	m_vecWalkPattern;
	_int	m_iWalkPattern;		// Walk패턴 인덱스
	_float	m_fPatternTime;

	_bool			m_bEffectOnce;

private:

	void	Check_AttackRange(); // 공격 범위에 플레이어가 있는지
	void	Check_Rotation();
	void	Check_Sync();

	void	Attack_Pattern();
	void	Walk_Pattern();

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

public:
	virtual _ulong Release(void);

};
#endif	//RoyalGuard_h__