/*!
 * \file Glitter.h
 * \date 2016/01/02 22:59
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Moonchopper Ŭ����
 *
 * TODO: StageOne �߰� ����
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

	_vec3	m_vPlayerPos;			// �÷��̾� Position
	_bool	m_bRotationOnce;		// Run�����Ҷ��� ���� ����Ϸ���

	vector<Moonchopper::MOONCHOPPERANIMATION>	m_vecInPattern;		// �ٰŸ����� 
	
	_int	m_iNearPattern;		// �ٰŸ����� �ε���
	_int	m_iRandNum;			// ���Ͽ� ���� ����
	_float	m_fRunDistance;		// �޸��� ���ߴ� ����
	_bool	m_bRun;				// �޸��� �ִ���
	_bool	m_bLowHealth;		// ü�� ���� ��
	_bool	m_bLowHealthOnce;	// m_bLowHealth �����ַ���
	_bool	m_bDie;				// ��������
	_vec3	m_vSpin;			// ������ �� ����

	_bool	m_bDamage;			// �ǰݵǾ�����
	_bool	m_bDisHP;			// HP����

	_bool			m_bEffectOnce;
	const _matrix*	m_pmatForearm_R; // ��ݿ� ���� ������ ���

	_int	m_iSoundStep;		// �߼Ҹ� ���������� ����ϱ����� ����.
	_float	m_fStepTime;		// �߼Ҹ� ���� Ÿ��.

	CEffect*		m_pSpinEffect; // ���� ����Ʈ

	_float	m_fOriginAngle;		// ������ ������
	_vec3	m_vOriginDir;		// ������ ���⺤��



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
	void	Compute_AngleToPlayer();	// �÷��̾� �ٶ󺸰� ���� ���� ���
	void	Set_AnimationSyncInfo();	// �ҷ��� ��ũ�����߿��� ���� �ִϸ��̼ǿ� �´� ������ �����ش�.
	_float	Compute_DisToPlayer();		// �÷��̾���� �Ÿ� ���
	void	Load_Pattern();				// ���� �ʱ�ȭ ����
	_bool	Pattern_Near_Distance();	// �ٰŸ� ���� (�������̸� false��ȯ)
	_bool	Pattern_Out_Distance();		// �ָ� ���� ��� �޷�����
	void	Pattern_LowHealth();		// ü���� ���� �� (����1����)

public:
	virtual _ulong Release(void);

};
#endif // CMoonchopper_h__

