/*!
 * \file CBloorLord.h
 * \date 2015/12/11 15 : 08
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief BloodLord Ŭ���� 
 *
 * TODO: StageOne ���� ����
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
	_bool			m_bStart;			// �ƽ���������
	_bool			m_bCutScene1;		// �ƽ�������
	_bool			m_bCutScene2;		// �ƽ�������
	_bool			m_bCutSceneTerm;

	_bool			m_bIdle;			// ���� ������
	_bool			m_bBlink;			// ��ũ �������� Ȯ������
	_bool			m_bSwamp;			// ��ũ �������� Ȯ������
	_bool			m_bRotation;		// �÷��̾� �ٶ󺸰�
	_bool			m_bAttackDuring;	// ���ݸ��������
	_bool			m_bDamageSuperArmor;

	_bool			m_bDie;
	_bool			m_bRealDie;
	_float			m_fLowHealth;		// lowhealth ��� �÷��� �ؾ��ϴ���
	BLOODSTATAE		m_eCurState;

	_float	m_fCutSceneEnd;	// �ƽ� ��
	_float	m_fCutSceneTerm;	// �ƽ� ��

	_float	m_fNearDistance;	// �ٰŸ� ��
	_float	m_fMidDistance;		// �߰��Ÿ� ��
	_float	m_fFarDistance;		// ���Ÿ� ��

	_int	m_iNearPattern;		// �ٰŸ����� �ε���
	_int	m_iMidPattern;		// �߰��Ÿ����� �ε���
	_int	m_iFarPattern;		// ���Ÿ����� �ε���

	_bool	m_bNearPattern;		// �ٰŸ� �����̾�����	(�ǰݽ� �ǰݸ�� �� �ٽ� �ε��� �����ֱ� ����)
	_bool	m_bMidPattern;		// ���Ÿ� �����̾�����
	_bool	m_bFarPattern;		// �߰��Ÿ������̾�����
	_bool	m_bRun;				// �޸��� ���� 2�� �־��ַ���

	_bool	m_bDamage;			// �ǰݵǾ�����
	_bool	m_bDisHP;			// HP����

	_bool	m_bDownState;		// �ٿ��������
	_bool	m_bDownTime;		// �ٿ��������
	_bool	m_bDisDown;			// �ٿ������ 50% �پ�����

	_float	m_fDownTime;		// �ٿ���۽ð�
	_float	m_fDownOverTime;	// �ٿ���ð�

	_bool	m_bInsideNavi;

	_float	m_fRunDistance;		// �޸��� ����
	
	_float	m_fBlinkTime;		// ��ũ ���۵Ǵ� �ð�
	_float	m_fBlinkOverTime;	// ��ũ ����Ǵ� �ð�
	_float	m_fBlinkEffTime;	// ��ũ ���� ���� �ڸ� �ð�
	_float	m_fBlinkEndTime;	// ��ũ ���� ���� �ڸ� �ð�
	_int	m_iBlinkNot;
	_bool	m_bBlinkEnd;

	_float	m_fSuperTime;		// �ǰݵǰ� ���۾Ƹӻ��� �Ǵ� �ð�
	_float	m_fSuperOverTime;	// ���۾Ƹ� ����Ǵ� �ð�

	_float	m_fOrigAngle;		// �÷��̾� �ٶ󺸱����� ȸ�� �� ����?
	_float	m_fAfterAngle;		// ȸ���ؾ��� ����
	
	_matrix m_matRotation;		// ����ȸ���� ���
	_float	m_fRevoluAngle;		// attack_skill���� ���� ȸ����ų ���� ��

	_float	m_fDancingDistance_x;	// attack_dancing���� ����� x�Ÿ�
	_float	m_fDancingDistance_z;	// attack_dancing���� ����� z�Ÿ�

	_vec3	m_vPlayerPos;			// �÷��̾� Position

	_matrix	m_matDancing;			// attack_dancing���� ���
	_matrix m_matRotationDefault;	// -90�� ȸ���ϱ� ���� ȸ�����

	vector<BloodLord::BLOODLORDANIMATION>	m_vecInPattern;		// �ٰŸ����� 
	vector<BloodLord::BLOODLORDANIMATION>	m_vecMidPattern;	// �߰��Ÿ����� 
	vector<BloodLord::BLOODLORDANIMATION>	m_vecOutPattern;	// ���Ÿ����� 

	_bool			m_bMeteoSkill;			// ���׿� �������� Ȯ������
	_bool			m_bMeteo;

	_vec3	m_vMonsterDir;

	_float	m_fRotSpeed;
	_bool	m_bRot_BreathEnd;
	_matrix m_matBloodBreath;
	_vec3	m_vDir;

	_bool	m_bCutsceneSoundPlay[20];
	_bool	m_bCutsceneFadeCheck;
	_bool	m_bCutsceneFadeCheckTwo;
private:	// �� Ʈ����
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
public:	// Ʈ����
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
	_bool	Check_Player_Distance(float fDist);	//�÷��̾�� �Ÿ� ���
	void	Check_Player_Radian();				//�÷��̾� �������� ȸ��
	void	Check_Turn();				//�÷��̾� �������� ȸ��
	void	Check_Rotation();				//�÷��̾� �������� ȸ��

	_bool	Pattern_Near_Distance();// �ٰŸ� ���� (�������̸� false��ȯ)
	_bool	Pattern_Mid_Distance();	// �߰��Ÿ� ����
	_bool	Pattern_Far_Distance();	// ���Ÿ� ����
	_bool	Pattern_Out_Distance();	// �ָ� ���� ��� ��ũ

	void	Load_Pattern();			// ���� �ʱ�ȭ ����
	void	Check_Skill_Blink();	// ��ũ �������� Ȯ��		/ ���� ���� ����
	void	Check_Skill_Swamp();	// �ǿ����� �������� Ȯ��	/ ���� ���� ����
	void	Check_Skill_Dancing();	// ����ȸ�� �������� Ȯ��	/ ȸ�� ����
	void	Check_Skill_BloodField();
	void	Check_Skill_BloodBreath();

	void	Check_DamageAfter_SuperArmor();		// �ǰ��� ���۾Ƹ� ���� Ȯ��

	void	Set_BlinkPosition();	// blink ������ ��Ÿ�� ��ġ ����

	void	Check_NearAttackPower();	// ���ݿ� ���� ���ݷ� ����
	void	Check_MidAttackPower();		
	void	Check_FarAttackPower();	

	void	Check_DownState();			// �ٿ�����϶� üũ

	void	Check_LowHealth();			// �� ���� ������� ���ݸ��-lowhealth���-���ݸ�� �̷��� �÷���
	void	Check_DieMotion();			// �״� ���

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
	void CameraModeCheck(void);	//�÷��̾��� ��ġ, ������ �� ��� �����Ͽ� ī�޶��带 on�Ѵ�.

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