/*!
 * \file Player.cpp
 * \date 2015/11/27 1:09
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CGameObject�� ��� �޵� �̱������� ����.
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

public: //�Լ�
#pragma region �ܺο��� ȣ�� �Ǵ� �Լ���
	virtual _int Update(const float& fTimeDelta);
	virtual void Render(void);
	_bool PlayerSkill_Input(_int iSKillNumber);	//PlayerSkill class���� ȣ���ϸ鼭 ����Ѵ�.
	void Player_AttackSuccess_InfoChange(void); //�÷��̾��� ������ ���� ���� ������ ��ġ�� �ٲپ��ִ� ��
#pragma endregion
#pragma region Getter, Setter
	CPlayer* ReturnThisPoint(void) { return this; }	//�÷��̾��� �ּ� ��ȯ
	void SetEnvironmentTexCom(Engine::CTexture* SetPoint) { m_pEnvironmentTexCom = SetPoint; }
	void SetNaviCell(Engine::CNaviCell* SetPoint) { m_pNaviCell = SetPoint; }
	_float GetDegreeAngleY(void) { return D3DXToDegree(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]); } //�÷��̾� Y�࿡ ���� ��׸� ȸ���� ��ȯ
	_vec3 GetPos(void) { return m_pTransCom->m_vPosition; }
	void SetPos(_vec3 vSetPos) { m_pTransCom->m_vPosition = vSetPos; }
	_float GetAngleY(void) { return m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]; }
	void SetAngleY(_float fSetAngle) { m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fSetAngle; }
	Engine::CPlayerDynamicMesh* Get_MeshCom(void) { return m_pMeshCom; }
	Engine::CTransform*	Get_TransCom(void) { return m_pTransCom; }
	Player::PLAYERANIMATION Get_PlayerAnimationType(void) { return m_ePlayerAnimation; }
	void Set_PlayerAnimationType(Player::PLAYERANIMATION eAnimation) { m_ePlayerAnimation = eAnimation; m_pMeshCom->Set_AnimationSetThree(m_ePlayerAnimation); }

	//�÷��̾� ����
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

	//�浹 ����
	CLIENTBOXINFO* Get_ClientBoxInfo(void) { return m_pAttackBox[m_ePlayerAnimationCheck]; }	//���� �ִϸ��̼��� ���� �ڽ��� ��ȯ�Ѵ�.
	_bool	Get_Check_EndPeriodMulTime_Start(void) { return m_pMeshCom->Check_EndPeriodMulTime(Get_ClientBoxInfo()->fStartTime); }
	_bool	Get_Check_EndPeriodMulTime_End(void) { return m_pMeshCom->Check_EndPeriodMulTime(Get_ClientBoxInfo()->fEndTime); }
	const _matrix*	GetpAttachment_RH_Matrix(void) { return m_pAttachment_RH_Matrix; }

	_float Get_MoveRadius(void) { return m_fMoveRadius; }
	_bool	GetInGame(void) { return m_bInGame; }
	void	SetInGame(_bool SetBool) { m_bInGame = SetBool; }

	_bool	GetBerserkerCheck(void) { return m_bBerserkerCheck; }

	//�� public������
	CPlayerSkill*	m_pPlayerSkill;

#pragma region //�÷��̾� ���� ���� ������
public:
	enum PlayerState { PS_BasicAttack, PS_BreakAttack, PS_SmashAttack, PS_SmashAttack_0, PS_SkillAttack, PS_Run, PS_Walk, PS_Idle, PS_Down, PS_Damage };
	enum PlayerDefenseState { PS_Basic, PS_SuperArmour };
	PlayerState Get_PlayerState(void) { return m_ePlayerState; }

private:
	PlayerState			m_ePlayerState;
	PlayerDefenseState	m_ePlayerDefenseState;
#pragma endregion

#pragma endregion

private: //����
#pragma region Default
	Engine::CPlayerDynamicMesh*	m_pMeshCom;	//���̳��� �Ž� ��ü
	Engine::CTransform*		m_pTransCom;	//Ʈ������ ��ü
	Engine::CNaviCell*		m_pNaviCell;	//NavigationMesh Cell ������
	LPD3DXEFFECT			m_pEffect;		//���̴�
	LPD3DXEFFECT			m_pShadowEffect; //�� �����߰� - �׸��� ���̴�
	LPD3DXEFFECT			m_pVelocityEffect;
	Engine::CTexture*		m_pEnvironmentTexCom; // ȯ��
	_matrix						m_matPreView;
	_vec3						m_vAttackStartPos;

	_float					m_fTimeDelta;		//������Ʈ���� ���� �ð� ���̸� �޾Ƽ� �ʿ��� ������ ����� ����
	_float					m_fRenderTimeDelta;	//�������� ����� float����
	_float					m_fAnimationSpeed;	//m_fRenderTimeDelta �� m_fMoveTimeDelta 2���� ���ÿ� �����ϱ� ���� ���� 2015.12.06 �ִϸ��̼� ���� �ٲٸ鼭 m_fMoveTimeDelta ������
#pragma endregion
#pragma region HP, Stamina, SP, Attack ����
	_float					m_fPlayerMaxHP;
	_float					m_fPlayerHP;
	_float					m_fPlayerMaxStamina;
	_float					m_fPlayerStamina;
	_float					m_fPlayerMaxSP;
	_float					m_fPlayerSP;

	_float					m_fPlayerPureAttackValue;	//��ü���� ���� �뷱���� ��� ���� ����
	_float					m_fPlayerAttackValue;		//���� ���� ��ġ
	_float					m_fPlayerDownValue;			//�ٿ� ��ġ
	_int					m_iPlayerAttackCombo;		//�޺�
	_float					m_fPlayerAttackComboTime;	//�޺� �ʱ�ȭ Ÿ��

	_float					m_fWakeUpAttackMagnification; //���� ��ġ
	Player::PLAYERANIMATION	m_ePlayerAttackStaminaAnimationCheck;

	_float					m_fPlayerRunFullTime;
	_bool					m_bPlayerRunFull;

	_bool					m_bBerserkerCheck;
	_float					m_fBerserkerFullTime;
	_float					m_fBerserkerTime;

	_float					m_fBerserkerCreateTime;
#pragma endregion
#pragma region ����, �¿챸��, Ÿ�� ����
public:
	_float					m_fOriginalAngleY;	//�÷��̾� Y�� ��׸� ����
private:
	//�÷��̾� Ű�Է¿� ���� ���� ���� ����
	enum AngleFix{ AF_LeftFront, AF_RightFront, AF_LeftBack, AF_RightBack, AF_Front, AF_Back, AF_Left, AF_Right, AF_End};
	_float					m_fAngleFixed[AF_End];

	//�÷��̾��� �¿츦 �����ϱ� ���� ������ ������
	enum FootFront{ Left_Foot_Front, Right_Foot_Front, FF_End};
	const _matrix* matLFoot;
	const _matrix* matRFoot;

#pragma endregion
#pragma region �ִϸ��̼� �ε���, Ű ����
	Player::PLAYERANIMATION m_ePlayerAnimation;	//����ؾ��� �÷��̾� �ε����� ������
	_float					m_fIdleToRestTime;	//���ݴ�⿡�� ���� ������� �������� �ð��� �Ǵ��ϱ� ���� ����

	bool	m_bOnceSpaceKeyCheck;	//SpaceKey�� �� ���� üũ�ϱ� ���� ���� �̹� �������� true ���� �ȴ������� false
	bool	m_bOnceLbuttonCheck;	//���� ��ư Ŭ���� �ѹ��� üũ�ϱ� ���� ����
	bool	m_bOnceRbuttonCheck;	//������ ��ư Ŭ���� �ѹ��� üũ�ϱ� ���� ����
#pragma endregion
#pragma region ����, �浹 ���� ����
	//�÷��̾� ���� ����
	enum BasicAttack { BA_1a, BA_1b, BA_2, BA_3, BA_4, BA_End};
	enum SmashAttack { SA_0, SA_1, SA_2, SA_3, SA_4a, SA_4b, SA_End};
	BasicAttack BasicAttackOrder;
	SmashAttack SmashAttackOrder;

	_int	m_iChargeSlashNumber;	//��¡�� ��� �ߴ��� �� ���� ����

	//�÷��̾� �浹 ���� ������
	_bool			m_bMonsterAttack_Body_CollisionCheck;
	_bool			m_bMonsterEffect_Body_CollisionCheck;

	_bool			m_bPlayerHitCheck;	//�÷��̾ ������ 1���� �ޱ� ���� ����
	CMonster*		m_pMonster; //�÷��̾ ���� ������ �ּҸ� ����ϱ� ���� ����
	DamageDirectionType	m_eDamageDirectionType;	//�ǰ� ������ ����

	_vec3	m_vCollisionPos;	// �浹 ��ġ ������ ��Ʈ�� ���� ���� ����

	_float	m_fEffAttackValue;	// �浹�� ����Ʈ�� ���ݷ��� ����ϱ����� ���� ������
	_float	m_fEffDownValue;	// �浹�� ����Ʈ�� �ٿ��

	_float	m_fDownValue;
#pragma endregion
#pragma region ����, ��� ��ü ���� ����
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

	//// �����ı��� �޽��� �ε��ϱ� ���ؼ� ����ȭ�鿡�� �� �޽��� ��ο� �̸����� �������ش�.
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
	void Ready_ClothMesh(void);	// ������ ������ �����ı��� �޽��ε�
	void Set_BrokenMesh();		// �� �޽� �����ϱ����� �ʿ��� PlayerDynamicMesh ��ü ����
	void Change_BrokenMesh();	// �������� ���� �ı��Ǿ��� ��� �޽� ��ü

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

	Engine::CPlayerDynamicMesh*	m_pHairMeshCom;		//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pUpperMeshCom;	//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pLowerMeshCom;	//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pHandMeshCom;		//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pFootMeshCom;		//���̳��� �Ž� ��ü

	Engine::CPlayerDynamicMesh*	m_pHairOrigMeshCom;		//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pUpperOrigMeshCom;	//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pLowerOrigMeshCom;	//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pHandOrigMeshCom;		//���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pFootOrigMeshCom;		//���̳��� �Ž� ��ü


	Engine::CPlayerDynamicMesh*	m_pHair_Broken_MeshCom;	//�����ı� ���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pUpper_Broken_MeshCom;	//�����ı� ���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pLower_Broken_MeshCom;	//�����ı� ���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pHand_Broken_MeshCom;		//�����ı� ���̳��� �Ž� ��ü
	Engine::CPlayerDynamicMesh*	m_pFoot_Broken_MeshCom;		//�����ı� ���̳��� �Ž� ��ü
#pragma endregion
#pragma region �׺���̼� �� ����
	_vec3					m_vOriginalPos;	//Navigation�Ǵܿ� ������ġ ������
	_vec3					m_vAfterPos;	//Navigation�Ǵܿ� Move�� ��ġ ������
	_vec3					m_vEffectPos;

	_bool					m_bInGame;
#pragma endregion
#pragma region ����Ʈ, ���� ����
	const _matrix* m_pAttachment_LH_Matrix;
	const _matrix* m_pAttachment_RH_Matrix;
	const _matrix* m_pL_Foot_Matrix;
	const _matrix* m_pR_Foot_Matrix;

	_bool	m_bInitialize_CreateEffect_Bool;
	Player::PLAYERANIMATION	m_ePlayerAnimationCheck;
	_bool	m_bCreateEffect[50];

	float	m_fEffectMoveTime;	// ����Ʈ�� �̵��� ���� �ð���

	//���� ����
	_bool	m_bPlayerSound[5];
	_bool	m_bSwordAttack;		//�˹ڽ��� ������ true

#pragma endregion

private: //������ �Լ� ���� �ִ� ��
#pragma region �ٿ�� �ڽ� ���� ����, �Լ���
	SPHEREINFO*					m_pSphere;		// �ٿ�����Ǿ�
	CLIENTBOXINFO*				m_pAttackBox[Player::PlayerAnimation_End]; //���� �ٿ���ڽ�[�ε���]

	enum BodyType { BT_Head, BT_Upper, BT_LowerRight, BT_LowerLeft, BT_HandRight, BT_HandLeft, BT_FootRight, /*BT_FootLeft, */BT_End };
	CLIENTBOXINFO*				m_pDamageBox[BT_End];

	_float						m_fBoxHP[5];
	_bool						m_bBox[5];
	_int						m_iColBox;

	_vec3	m_vMoveMin, m_vMoveMax;
	_float	m_fMoveRadius;


	void	Load_AttackBox();	// �ٿ���ڽ� �ε�
	void	Load_DamageBox();	// �ǰ� �ڽ� �ε�
#pragma endregion
#pragma region ����׿� ���� ������ �Լ��� �� Ȯ�ο� �ӽ��ڵ�
#ifdef _DEBUG
private:
	ID3DXMesh*				m_pSphereMesh;		// ��豸 �׸���� �޽� & ����
	LPD3DXBUFFER			m_pSphereBuffer;

	void	Render_AttackBox();				// ���� �ٿ���ڽ� �׸���
	void	Render_DamageBox();				// �ǰ� �ٿ���ڽ� �׸���
	void	Render_BoudingSphere();				// �ٿ�����Ǿ� �׸���

public:
	bool		m_bClick;				// �ٿ���ڽ�
	bool		m_bShowBounding;
#endif
#pragma endregion

private: //�Լ�
#pragma region Default
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);
	void Set_ContantTable(void);
#pragma endregion	
#pragma region �÷����� Ÿ�� üũ, �ִϸ��̼� ���� ����
	void PlayerStateCheck(void);
	void Key_Input_Change_PlayerAnimationState(const float& fTimeDelta); //���� �ִϸ��̼ǰ� Ű, ���콺�� �Է¿� ���� ePlayerAnimation ���� ��ȭ ��Ų��.
	void AnimationPattern_Immediately_Change(void);	//���ǿ� ���� ��� �ִϸ��̼� ������ �ٲ��ִ� ��
	void Current_Animation_Change_Direction(Player::PLAYERANIMATION ePlayAnimationType, _float fCheckEndPeriodMulTime = 0.f); //���ǿ� �����ϸ� ���� �ִϸ��̼��� m_ePlayerAnimation���� Change ���ش�.
#pragma endregion
#pragma region ȸ�� �� �¿� üũ
	void Key_Input_Rotation(void);						//Ű �Է¿� ���� �÷��̾��� ������ õõ�� ��ȭ ��Ű�� �Լ�
	void Change_AngleY(_float fAngleFixed);				//ī�޶�� �÷��̾��� ���� ���� ���踦 �����Ͽ� �÷��̾��� Y������ õõ�� ȸ����ų �Լ�
	void Key_Input_Immediately_Rotation(void);			//Ű �Է¿� ���� �÷��̾��� ������ �ٷ� ��ȭ ��Ű�� �Լ�
	void Change_AngleY_Immediately(_float fAngleFixed);	//ī�޶�� �÷��̾��� ���� ���� ���踦 �����Ͽ� �÷��̾��� Y������ �ٷ� ȸ����ų �Լ�	
	FootFront LeftAndRightCheck(void);					//���ʹ��� �տ� �ִ��� �����U ���� �տ��ִ��� üũs
#pragma endregion
#pragma region ��ǲ ����
	void Current_Player_Animation_Type_Change_KeyInput(BYTE KeyFlag, Player::PLAYERANIMATION eChangePlayerAnimationType, _float fCheckEndPeriodMulTime = 0.f, _bool bOnceBool = false); //���� �������� �ִϸ��̼��� �ε��� ���� Ű�� �Է�, Ÿ�̹��� ���ؼ� m_ePlayerAnimation�� ���¸� �ٲ��ش�.
	void Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::MOUSECLICK MouseFlag, Player::PLAYERANIMATION eChangePlayerAnimationType, _float fCheckEndPeriodMulTime = 0.f, _bool bOnceBool = false); //�ٲ� ���¸� ���� AnimationPattern_Immediately_Change���� ���ǿ� ���� �ִϸ��̼��� Set���ش�.
	void Current_Player_Animation_Type_Change_SpaceInput(void);
	void BlockadeRush_MouseInput(void);	//����, �޸��鼭 ����, ���� ����ó��
	void Judgement_MouseInput(void);	//���� ��� ����� ����ó��
#pragma endregion
#pragma region �浹 ����
	void CollisionCheckFunction(void);			//�浹 �Լ����� ȣ�� �ϴ� �κ�
	void MonsterAttackBox_And_MyBodyBox(void);	//������ ���ݰ� ���� �ǰ� �ڽ�
	void MonsterEffectBox_And_MyBodyBox(void);	//������ ����Ʈ�� ���� �ǰ� �ڽ�
	bool ObjectBox_And_MyMoveBox(void);			//������Ʈ�ڽ��� ���� ���� �ڽ�
	bool CDynamicMesh_And_MyMoveBox(void);		//���̳��� �ڽ��鳢���� �浹
#pragma endregion
#pragma region ��Ÿ
	void Player_InfoChange(void);				//���ݷ�, �ٿ� ��ġ�� �ǽð����� �ٲ�� �ϴ°�
	void Player_UnderAttack_InfoChange(void);	//������ ���� �� �ް� ���� ������ ������ ���� �ϴ� ��
	void PlayerMoveFunction(void);
	void Check_Navigation(); // �׺�޽� Ÿ�� �κ�
	void CreateEffectFunction(void);
	//�÷����� ����Ʈ ������ �̸�, �ִϸ��̼� �ð�, �ʿ��� ��� �ּ�, ���� ��ġ, �οﺯ��Number / ����Ʈ ���� ����, ����� ������ true�� �ٽ� ���, false�� ����
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
#pragma region Ʈ����
	const	_matrix*		m_pmatEye;
	_vec3				m_vEyePos;
	_vec3*				Get_EyePos(){return &m_vEyePos;}
#pragma endregion

public:
	virtual _ulong Release(void);
};
#endif // Player_h__
