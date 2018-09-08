/*!
 * \file Glitter.h
 * \date 2015/12/29 13:07
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Glitter Ŭ����
 *
 * TODO: StageOne �߰� ����
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

	_vec3	m_vPlayerPos;			// �÷��̾� Position
	_bool	m_bRotationOnce;		// Run�����Ҷ��� ���� ����Ϸ���

	vector<Glitter::GLITTERANIMATION>	m_vecInPattern;		// �ٰŸ����� 
	vector<Glitter::GLITTERANIMATION>	m_vecMidPattern;	// �߰��Ÿ����� 
	vector<Glitter::GLITTERANIMATION>	m_vecOutPattern;	// ���Ÿ����� 

	_int	m_iNearPattern;		// �ٰŸ����� �ε���
	_int	m_iMidPattern;		// �߰��Ÿ����� �ε���
	_int	m_iFarPattern;		// ���Ÿ����� �ε���

	_float	m_fRunDistance;		// �޸��� ���ߴ� ����
	_bool	m_bRun;				// �޸��� �ִ���
	_bool	m_bLowHealth;		// ü�� ���� ��
	_bool	m_bLowHealthOnce;	// m_bLowHealth �����ַ���
	_bool	m_bDie;				// ��������

	const _matrix*	m_pmatFireball; // ��Ʈ ���
	const _matrix*	m_pmatAttachLH; // AttachLH ���
	const _matrix*	m_pmatAttachRH; // AttachRH ���
	const _matrix*	m_pmatSMDImport; // �߾� ���
	_bool			m_bEffectOnce;  // ����Ʈ��ü �ѹ��� ����
	CEffect*		m_pLHShortEffect; // LH������ ���� �� ����ٴϴ� ����Ʈ
	CEffect*		m_pALHShortEffect; // ��հ��� L����Ʈ
	CEffect*		m_pARHShortEffect; // R����Ʈ
	CEffect*		m_pExplosionEffect; // ���� ����Ʈ

	_bool	m_bDamage;			// �ǰݵǾ�����
	_bool	m_bDisHP;			// HP����

	_int	m_iSoundStep;		// �߼Ҹ� ���������� ����ϱ����� ����.
	_float	m_fStepTime;		// �߼Ҹ� ���� Ÿ��.

	_float	m_fOriginAngle;		// ������ ������
	_vec3	m_vOriginDir;		// ������ ���⺤��

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
	_bool	Pattern_Mid_Distance();		// �߰��Ÿ� ����
	_bool	Pattern_Far_Distance();		// ���Ÿ� ����
	_bool	Pattern_Out_Distance();		// �ָ� ���� ��� �޷�����
	void	Pattern_LowHealth();		// ü���� ���� �� (����3����)
public:
	virtual _ulong Release(void);

};
#endif // Glitter_h__