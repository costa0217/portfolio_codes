/*!
 * \file Monster.h
 * \date 2015/12/13 16:03
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Monster�� �ֻ��� class.
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

	//Move �浹 ����
	_bool	Get_PlayerDistanceCheck(void) { return m_bPlayerDistanceCheck; }
	_float				Get_MoveRadius(void) { return m_fMoveRadius; }
	_bool	Get_MoveCollCheck(void){ return m_bMoveCollCheck;}

	//�浹 ����
	boost::unordered_map<int, vector<CLIENTBOXINFO*>>* Get_MonsterMapAttackBox(void) { return &m_mapAttackBox; }
	_int	Get_MonsterAnimationIndex(void) { return m_iMonsterAnimationIndex; }

	//HP&���ݷ�
	CHARACTERINFO	Get_MonsterInfo(void) { return m_tMonsterInfo; }
	_float			Get_MonsterAttackValue(void) { return m_tMonsterInfo.fAttack; }
	//���ݿ� ���� �ٿ� ��ġ
	_float			Get_MonsterDownValue(void) { return m_fMonsterDownValue; }

	//����Ʈ ����
	//�÷����� ����Ʈ ������ �̸�, �ִϸ��̼� �ð�, �ʿ��� ��� �ּ�, ���� ��ġ, �οﺯ��Number / ����Ʈ ���� ����, ����� ������ true�� �ٽ� ���, false�� ����
	void Play_Effect(TCHAR* pName, _float fCheckEndPeriodMulTime,  const _matrix* pMatrix, _vec3 vLocalPos, _int iboolNumber, _bool bReStart = false, _bool bHDR = true, _vec3 vSize = _vec3(0.f, 0.f, 0.f), _vec3 vAngle = _vec3(0.f, 0.f, 0.f));

	//���忩�� ����
	_bool	Set_Appear(_bool bAppear){ m_bAppear = bAppear; }

protected: //����
#pragma region Defualt
	Engine::CDynamicMesh*	m_pMeshCom;
	Engine::CTexture*			m_pTexCom;
	Engine::CTransform*		m_pTransCom;	//Player�� ������ �������� CDynamicMesh
	Engine::CNaviCell*		m_pNaviCell;	//NavigationMesh Cell ������
	LPD3DXEFFECT			m_pEffect;
	LPD3DXEFFECT			m_pShadowEffect;

	CPlayer*				m_pPlayer;

	_float					m_fAngleX;
	_float					m_fAngleZ;

	_bool					m_bAppear; // true == spawn, false == ���߿� spawn

#pragma endregion
#pragma region ������ Ÿ�԰� ���¿� ���� ������
	//���Ϳ� ������ ��������
	enum MonsterType	{ MT_Boss, MT_Monster};	//�� ���Ϳ��� �ʱ�ȭ
	enum MonsterState	{ MS_Attack, MS_Idle, MS_Damage, MS_Super}; //�� ���Ϳ��� �ʱ�ȭ

	MonsterType		m_eMonsterType;
	MonsterState	m_eMonsterState;

	CHARACTERINFO	m_tMonsterInfo;
#pragma endregion
#pragma region �ð� ���� ������
	_float		m_fTimeDelta; //������Ʈ���� ���� �ð� ���̸� �޾Ƽ� �ʿ��� ������ ����� ����
	_float		m_fRenderTimeDelta;	//�������� ����� float����
	_float		m_fAnimationSpeed;	//�ִϸ��̼��� �ӵ��� ������ ����

	_bool		m_bSoundPlay[20]; //���带 ����ϱ� ���� ����
	_int		m_iAnimationSoundCheck; //���� ���� ������ �ʱ�ȭ �ϱ� ���� ����
#pragma endregion
#pragma region �浹 ���� ������
	_bool		m_bPlayerDistanceCheck;
	_bool		m_bPlayerAttack_Body_CollisionCheck;
	_bool		m_bPlayerEffect_Body_CollisionCheck;
	_bool		m_bPlayerAttack_MyAttack_CollisionCheck;

	_vec3		m_vCenter;	//��ü�� �߽ɰ�
	_float		m_fRadius;

	//�浹����
	_int	m_iMonsterAnimationIndex;
	boost::unordered_map<int, vector<CLIENTBOXINFO*>>	m_mapAttackBox;	// �ִϸ��̼� �ε��� + �ٿ���ڽ� vector
	vector<CLIENTBOXINFO*>				m_vecDamageBox;	// �ִϸ��̼� �ε��� + �ٿ���ڽ� vector
	SPHEREINFO*							m_pSphere;	// �ٿ�����Ǿ�
	_bool								m_bMoveCollCheck; // (Moonchopper�� ��� spin�Ҷ�) �浹üũ ���ο� ���� ���� true�� �˻�

	//�ִϸ��̼� ��ũ����
	boost::unordered_map<int, vector<FOOTANISYNC*>>	m_mapAniSync;	// �ִϸ��̼� �ε��� + �ٿ���ڽ� vector

	_vec3	m_vMoveMin, m_vMoveMax, m_vMoveCenter;
	_float	m_fMoveRadius;		//���� ���Ǿ��� ������

	_float	m_fMaxDamageValue;		// ���� �ٿ� ����
	_float	m_fDownValue;			// ���Ϳ� ���̴� �ٿ� ��ġ
	_float	m_fMonsterDownValue;	// ���� ���ݿ� ���� �ٿ� ��ġ
	_vec3	m_vCollisionPos;	// �浹 ��ġ ������ ��Ʈ�� ���� ���� ����
#pragma endregion
#pragma region �׺���̼� ����
	_vec3		m_vOriginalPos;		//Navigation�Ǵܿ� ������ġ ������
	_vec3		m_vAfterPos;		//Navigation�Ǵܿ� Move�� ��ġ ������
#pragma endregion
#pragma region ���������� ����ϴ� ����
	CBossFaceUI*	m_pBossFaceUI;	//���� �ε�, �۸��͸� ����Ҳ���
	_bool			m_bCameraModeOn;
	_bool			m_bCameraModeCheck;
#pragma endregion
#pragma region ��ũ ���� ����/���� Position�� �̵���Ű������ ����
	_float		m_fDistanceSize;

	//����Ʈ ����
	_bool	m_bCreateEffect[50];
#pragma endregion
protected: //Monster���� �޾Ƽ� ���������� ���� �Լ���
	void SoundEffectPlay(SOUND::EFF eSoundType, _float fCheckEndPeriodMulTime, _int iboolNumber, _float fVolume = 1.f);
#pragma region Move, Box Loading
	void MonsterMoveFunction(const char* pFrameName);
	void MonsterMoveFunctionTwo(_bool bCompulsion, _bool bLeftAndRihgt, _bool bFrontAndBack);
	void Check_Navigation(void); // �׺�޽� Ÿ�� �κ�.
	void Load_AttackBox(TCHAR* pBoundingBoxPath);	// ���� �ٿ���ڽ� �ε�
	void Load_DamageBox(TCHAR* pBoundingBoxPath);	// �ǰ� �ٿ���ڽ� �ε�
#pragma endregion
#pragma region Move, Sync Loading
	void Load_FootAnimationSync(TCHAR* pSyncPath);	// ��ũ �ε�
#pragma endregion
#pragma region �浹 ���� �Լ���
	void CollisionCheckFunction(void);				//�浹 �Լ����� ȣ�� �ϴ� �κ�
	void PlayerDistanceCheck(void);					//���Ϳ� �÷��̾��� �Ÿ��� üũ�ϴ� �Լ�
	void PlayerAttackBox_And_MyBodyBox(void);		//�÷��̾��� ���ݰ� ���� �ǰ� �ڽ�
	void PlayerEffectBox_And_MyBodyBox(void);		//�÷��̾��� ����Ʈ �� ���� �ǰ� �ڽ�
	void PlayerAttackBox_And_MyAttackBox(void);		//�÷��̾��� ���ݹڽ��� ���� ���� �ڽ�
	bool ObjectBox_And_MyMoveBox(void);				//������Ʈ�ڽ��� ���� ���� �ڽ�
	bool CDynamicMesh_And_MyMoveBox(void);			//���̳��� �ڽ��鳢���� �浹
	void Initialize_Min_Max_Radius(void);
	void Update_Monster_AnimationIndex();			//�޽� �ִϸ��̼ǰ� �ٸ��� ����?
#pragma endregion
protected: //�ڽĵ鿡�� �ݵ�� ���� �Ǿ�� �ϴ� �Լ���
#pragma region ���� �Լ�
	virtual void Check_Monster_Satae(void)					PURE; //�ִϸ��̼ǰ� Ư�� ���ǵ鿡 ���� ������ ���¸� ���� �ִ� ��.
	virtual void AnimationPattern_Immediately_Change(void)	PURE; //���ǿ� ���� �ִϸ��̼� ������ �ﰢ������ �ٲپ� ��� �ϴ� �κ�(�÷��̾�� ���� �������� ��)
	virtual void Check_PlayerAttack_AccordingToHP(void)		PURE; //�÷��̾��� ���ݿ� ���� ������ ����
	virtual void BattleAnimationPattern(void)				PURE; //������ AI�� �����ϴ� ��
	virtual void AnimationTimeChange(void)					PURE; //���� �ִϸ��̼��� �ð��� �ٲ��ִ� ��		
	virtual void SoundPlayer(void)							PURE; //���带 �÷����Ѵ�. �´� ������ Check_PlayerAttack_AccordingToHP ���ְ� �������� ���ָ� �ɵ�
#pragma endregion


//����� ����� ����� ����� ����� ����� ����� ����� ����� ����� �����
#ifdef _DEBUG
protected:
	void	Render_AttackBox();				// �ٿ���ڽ� �׸���
	void	Render_DamageBox();				// �ٿ���ڽ� �׸���
	void	Render_BoudingSphere();				// �ٿ�����Ǿ� �׸���

	ID3DXMesh*				m_pSphereMesh;		// ��豸 �׸���� �޽� & ����
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