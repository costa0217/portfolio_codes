#ifndef BloodJelly_h__
#define BloodJelly_h__

#include "Defines.h"
#include "Monster.h"

#include "Transform.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CNaviCell;
}

class CBloodJelly : public CMonster
{
private:
	explicit CBloodJelly(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBloodJelly(void);

public:
	virtual _int Update(const float& fTimeDelta);
	virtual void Render(void);

public:
	static	CBloodJelly*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	static	_vec3			m_vPos;

private:
	_int	m_iIdlePattern;
	_float	m_fOrigAngle;
	_float	m_fAttackDistance;	// 공격 범위
	_float	m_fFollowDistance;	// 따라가는 범위

	_bool	m_bSpawn; 

	_float m_fDieTime;
	_float m_fDieOverTime;
	_bool	m_bAttack;			// 공격중인지
	_bool	m_bDamage;			// 피격되었는지
	_bool	m_bDisHP;			// HP감소
	_bool	m_bDie;
	_bool	m_bHP_0;

	_bool	m_bDieSound;			//죽으면 사운드 재생 한번만 ㅂㅊ

	_vec3	m_vMonsterDir;
	_matrix m_matRotationDefault;	// -90도 회전하기 위한 회전행렬

	vector<BloodJelly::BLOODJELLYANIMATION>	m_vecIdlePattern;

	void	Check_AttackRange(); // 공격 범위에 플레이어가 있는지
	void	Check_Rotation();
	void	Check_Sync();
	void	Idle_Pattern();
	void	Set_Idle_Pattern();
	void	Release_Box();

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
#endif