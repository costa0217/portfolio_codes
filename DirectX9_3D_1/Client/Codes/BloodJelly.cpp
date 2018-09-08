#include "stdafx.h"
#include "BloodJelly.h"

#include "Engine_Defines.h"
#include "Export_Function.h"

#include "Line2D.h"
#include "CullingMgr.h"

#include "Player.h"

#include "Pierceaura.h"

#include "SoundMgr.h"
#include "DamageFont.h"

#include "EffectMgr.h"

#include "PlayerBeShotUI.h"
#include "CollisionMgr.h"

_vec3	CBloodJelly::m_vPos;

CBloodJelly::CBloodJelly(LPDIRECT3DDEVICE9 pGraphicDev)
: CMonster(pGraphicDev)
, m_bDamage(false)
, m_bDisHP(false)
, m_bAttack(false)
, m_fAttackDistance(0.f)
, m_fFollowDistance(0.f)
, m_fOrigAngle(0.f)
, m_vMonsterDir(0.f, 0.f, -1.f)
, m_iIdlePattern(0)
, m_bHP_0(false)
, m_bDie(false)
, m_bDieSound(false)
, m_bSpawn(true)
, m_fDieTime(0.f)
, m_fDieOverTime(0.f)
{
	D3DXMatrixRotationY(&m_matRotationDefault, D3DXToRadian(180));

}
CBloodJelly::~CBloodJelly(void)
{

}
HRESULT CBloodJelly::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Monster");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	//������ �ʱ�ȭ
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vPosition = m_vPos;//_vec3(190.f, 0.f, 100.f);
	m_pTransCom->m_vPosition.y = -15.f;

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(0);

	m_iMonsterAnimationIndex = BloodJelly::spawn;
	m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);

	////�ٿ���ڽ� �ε�
	CMonster::Load_AttackBox(L"../Bin/Resources/Mesh/DynamicMesh/Monster/Jelly/Jelly_AttackBox.dat");
	CMonster::Load_DamageBox(L"../Bin/Resources/Mesh/DynamicMesh/Monster/Jelly/Jelly_DamageBox.dat");
	// Boss_HP UI

	//// �ִϸ��̼� ��ũ
	CMonster::Load_FootAnimationSync(L"../Bin/Resources/Mesh/DynamicMesh/Monster/Jelly/Jelly_Sync.dat");

	Initialize_Min_Max_Radius();

	m_fMaxDamageValue = 500.f;
	m_fDownValue = 0.f;
	m_fMonsterDownValue = 0.f;

	m_vMoveMin		= _vec3(-7.f, 0.f, -7.f);
	m_vMoveMax		= _vec3(7.f, 10.f, 7.f);
	m_fMoveRadius	= D3DXVec3Length(&(m_vMoveMax - m_vMoveMin)) * m_pTransCom->m_vScale.x;

	m_tMonsterInfo.fAttack = 100.f;
	m_tMonsterInfo.fMaxHP = 12000.f;
	m_tMonsterInfo.fHP = m_tMonsterInfo.fMaxHP;
	m_tMonsterInfo.fDownValue = 5.f;

	m_fDistanceSize = 2.f;

	m_fAttackDistance = 20.f;
	m_fFollowDistance = 50.f;

	Set_Idle_Pattern();

	return S_OK;
}
HRESULT CBloodJelly::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh Component	// �÷��̾ ������ ���̳��͸޽����� CDynamicMesh �� �ε��� ��.
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, L"Mesh_Jelly");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_vAfterPos = m_pTransCom->m_vPosition;

	m_pMeshCom->SetLeftAndRihgtMatrix("Jelly_Bottom", "SMDImport");

	return S_OK;
}
_int CBloodJelly::Update(const float& fTimeDelta)
{
	m_fTimeDelta		= fTimeDelta;

	KeyInput();

	if( m_bSpawn )
	{
		m_pTransCom->m_vPosition.y += 3.f * fTimeDelta;
	}

	if( m_bDie )
	{
		m_fDieOverTime += fTimeDelta;

		if( m_fDieOverTime >= m_fDieTime + 2.f )
		{
			m_bMoveCollCheck = false;
			return -1;
		}
	}

	CMonster::CollisionCheckFunction();			//�浹�� ���� �˻縦 �Ѵ�.
	Check_Monster_Satae();						//�ִϸ��̼�, �ð�, Ư�� ���ǿ� ���� ������ ���� Ÿ���� ���������� üũ�Ͽ� �ٲپ� �ִ� �� => �� ���¿� ���� �÷��̾�� �浹 �˻縦 �ǳ� �ٰų� ���Ͱ� �ٿ� �����϶� �������� �� ���ų� �ٽ� �ٿ�ִϸ��̼��� Set ���ϵ��� �Ѵ�.
	AnimationPattern_Immediately_Change();		//�浹�� ���� ������ �ִϸ��̼��� �ﰢ������ �ٲ۴�. �浹 ������ ���� Monster class���� ���������� ������� �浹, Math Mgr���� ����� Ȱ��, ������ Ÿ���� ���� Ư�� ����� �������϶��� �浹 ���ص� �������� �ް� �ؾ� �Ѵ�.
	Check_PlayerAttack_AccordingToHP();			//�浹�� ���� ������ HP������ �ǰ� ����Ʈ ����
	BattleAnimationPattern();					//�÷��̾��� ���ݿ� �浹�� ���ϰ� �ִϸ��̼��� ������ ����� ���� �˰����� ���� ������ AI�� ���ư����� �Ѵ�.
	AnimationTimeChange();						//�⺻������ m_fAnimationSpeed�� �ӵ��� 2.f������ ���ο� ���³� Ư������� �ӵ��� ���⶧ ���⼭ �Ѵ�.

	//Check_Sync(); // Render�� �ű�

	if( m_bDie == false && (m_bPlayerAttack_Body_CollisionCheck || m_bPlayerEffect_Body_CollisionCheck) )	// ��ũ ���°� �ƴϸ鼭 �÷��̾� ���ݿ� �浹���� ��
	{
		m_bDamage = true;
	}
	else
		m_bDamage = false;

	SoundPlayer();

	CGameObject::Update(fTimeDelta);	
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	return 0;
}
void CBloodJelly::Render(void)
{
	if(NULL == m_pEffect)		return;
	Set_ContantTable();	

	CMonster::Render();

	if( m_bDie == false)
		m_pMeshCom->Move_Frame(m_fRenderTimeDelta);
	m_pMeshCom->Render_MeshForShader(m_pEffect, false);

	Check_Sync();

}
CBloodJelly* CBloodJelly::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBloodJelly*	pMonster = new CBloodJelly(pGraphicDev);

	m_vPos = vPos;

	if( FAILED(pMonster->Initialize()) )
	{
		MSG_BOX("CBloodJelly Create Failed");
		::Safe_Release(pMonster);
	}
	return pMonster;
}
void CBloodJelly::KeyInput()
{
	if( Engine::GetDIKeyState(DIK_V) & 0x80 )
	{
		printf("W\n");
		m_iMonsterAnimationIndex = 6;
		m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
	}
	else if( Engine::GetDIKeyState(DIK_B) & 0x80 )
	{
		printf("E\n");
		m_iMonsterAnimationIndex = BloodJelly::runE;
		m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
	}
	else if( Engine::GetDIKeyState(DIK_H) & 0x80 )
	{
		printf("S\n");
		m_iMonsterAnimationIndex = BloodJelly::runS;
		m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
	}
	else if( Engine::GetDIKeyState(DIK_J) & 0x80 )
	{
		printf("N\n");
		m_iMonsterAnimationIndex = BloodJelly::runN;
		m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
	}
}
void CBloodJelly::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj, matInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	_vec3		vCamera;
	D3DXMatrixInverse(&matInv, NULL, &matView);
	memcpy(&vCamera, &matInv.m[3][0], sizeof(_vec3));

	m_pEffect->SetVector("g_WorldCamPos", &_vec4(vCamera, 1.f));
	m_pEffect->SetMatrix("g_matProj", &matProj);
}
void CBloodJelly::Check_Monster_Satae(void)
{

}
void CBloodJelly::AnimationPattern_Immediately_Change(void)
{
	if( m_iMonsterAnimationIndex == BloodJelly::spawn )
	{
		if( m_pMeshCom->Check_EndPeriodMulTime(0.9f) && !m_pMeshCom->Check_EndPeriodMulTime(0.91f))
		{
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, false);
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, false, _vec3(0, 0, 0), _vec3(0, 90, 0) );

			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2", m_pTransCom->m_vPosition + _vec3(0.f, 4.f, 0.f), false, false);
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2", m_pTransCom->m_vPosition + _vec3(0.f, 4.f, 0.f), false, false, _vec3(0, 0, 0), _vec3(0, 90, 0) );
		}
	}
	if( m_pMeshCom->Check_EndPeriod())
	{
		if( m_iMonsterAnimationIndex == BloodJelly::spawn )
		{
			m_bSpawn = false;
			m_iMonsterAnimationIndex = BloodJelly::idle;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
		else if( m_iMonsterAnimationIndex == BloodJelly::jelly_damage_dying_bottom )
		{
			m_bDie = true;
			m_fDieTime = m_fTimeDelta;
		}
	}
	if( m_iMonsterAnimationIndex == BloodJelly::runW)
	{
		m_fDistanceSize = 15.f;
	}
	else if( m_iMonsterAnimationIndex == BloodJelly::runN)
	{
		m_fDistanceSize = 50.f;
	}
	else if( m_iMonsterAnimationIndex == BloodJelly::runE)
	{
		m_fDistanceSize = 5.f;
	}
	else if( m_iMonsterAnimationIndex == BloodJelly::runS)
	{
		m_fDistanceSize = 5.f;
	}

	if( m_iMonsterAnimationIndex == BloodJelly::jelly_attack_explosion )
	{
		if( m_pMeshCom->Check_EndPeriodMulTime(0.4f) == true && 
			m_pMeshCom->Check_EndPeriodMulTime(0.55f) == false)
		{
			if( m_pTransCom->m_vScale.x  < 0.12f )
				m_pTransCom->m_vScale += _vec3( 1.2f, 1.2f, 1.2f) * m_fTimeDelta;
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.55f) == true )
		{
			if( m_pTransCom->m_vScale.x > 0.1f )
				m_pTransCom->m_vScale -= _vec3( 1.f, 1.f, 1.f) * m_fTimeDelta;
		}
	}
}
void CBloodJelly::Check_PlayerAttack_AccordingToHP(void)
{
	if( m_bDie )
		return;

	if( m_bDamage )
	{
		if( m_bDisHP )
			return;

		CPlayerBeShotUI::GetInstance()->PlayerBeShotUI();

		m_bDisHP = true;

		//����ų ó��
		float MonsterHp = m_tMonsterInfo.fHP;	//���� ����ų�� ���� �ӽ÷� ���� 
		_float	m_fPlayerAttack = CPlayer::GetInstance()->GetPlayerAttackValue() + rand()%100;
		m_tMonsterInfo.fHP -= m_fPlayerAttack;

		if(!m_bHP_0)//1���� ������ ��Ʈ�� �����ϱ� ���� ����
		{
			if(m_tMonsterInfo.fHP <= 0.f)
			{
				m_tMonsterInfo.fHP = 0.f;
				m_bHP_0 = true;
				if((CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp) <= 100)//Ȥ�� 100���� �������� ������ ����� ���� ����ó����
				{
					CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(m_fPlayerAttack - MonsterHp + 100), m_vCollisionPos);	//���Ͱ� ���� �������� ����.
				}
				else
					CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(m_fPlayerAttack - MonsterHp), m_vCollisionPos);	//���Ͱ� ���� �������� ����.
			}
			else
			{
				CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_DAMAGE, DFONT_OBJ_MONSTER, (int)m_fPlayerAttack, m_vCollisionPos);	//���Ͱ� ���� �������� ����.
			}	
		}		

		if( m_bHP_0 )
		{
			m_iMonsterAnimationIndex = BloodJelly::jelly_damage_dying_bottom;
			m_pMeshCom->Set_AnimationSet(BloodJelly::jelly_damage_dying_bottom);
			m_bMoveCollCheck = false;
		}
		else
		{
			//���� ���ݿ� ���� �ٸ� ���带 �����Ű�� �Ѵ�.
			//CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Flesh_Slash + rand()%7));

			CPlayer::GetInstance()->Player_AttackSuccess_InfoChange();
			CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);

			_vec3 m_vPlayerPos = CPlayer::GetInstance()->GetPos();
			DamageDirectionType eType = CMathMgr::GetInstance()->DamageDirectionDecision(m_vPlayerPos, m_pTransCom);


			switch(eType)
			{
			case DDT_FRONT:
				printf("DDT_FRONT\n");
				m_pMeshCom->Set_AnimationSet(BloodJelly::jelly_damage_weak_back);
				break;
			case DDT_BACK:
				printf("DDT_BACK\n");
				m_pMeshCom->Set_AnimationSet(BloodJelly::jelly_damage_weak_bottom);
				break;
			case DDT_FRONT_LEFT:
				printf("DDT_FRONT_LEFT\n");
				m_pMeshCom->Set_AnimationSet(BloodJelly::jelly_damage_weak_back);
				break;
			case DDT_BACK_LEFT:
				printf("DDT_BACK_LEFT\n");
				m_pMeshCom->Set_AnimationSet(BloodJelly::jelly_damage_weak_bottom);
				break;
			case DDT_FRONT_RIGHT:
				printf("DDT_FRONT_RIGHT\n");
				m_pMeshCom->Set_AnimationSet(BloodJelly::jelly_damage_weak_back);
				break;
			case DDT_BACK_RIGHT:
				printf("DDT_BACK_RIGHT\n");
				m_pMeshCom->Set_AnimationSet(BloodJelly::jelly_damage_weak_bottom);
				break;
			}
		}
	}
	else
		m_bDisHP = false;

}
void CBloodJelly::BattleAnimationPattern(void)
{
	if( m_bDie )
		return;

	Check_AttackRange();
}
void CBloodJelly::AnimationTimeChange(void)
{
	m_fRenderTimeDelta	= m_fTimeDelta * m_fAnimationSpeed * 1.f;
}

void CBloodJelly::Check_Sync()
{
	if( m_bSpawn )
		return;

	boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter = m_mapAniSync.find(m_iMonsterAnimationIndex);
	if( iter != m_mapAniSync.end() )
	{
		vector<FOOTANISYNC*>::iterator iter_vec = iter->second.begin();
		vector<FOOTANISYNC*>::iterator iter_end = iter->second.end();

		for( ; iter_vec != iter_end; ++iter_vec )
		{
			if(m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fStartTime) && !m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fEndTime))
				CMonster::MonsterMoveFunctionTwo((*iter_vec)->bCompulsion, (*iter_vec)->bLeftFoot, (*iter_vec)->bFront);
		}
	}
}

void CBloodJelly::SoundPlayer(void)
{
	if(m_pMeshCom->Get_CurrentAniIdx() != m_iAnimationSoundCheck || m_pMeshCom->Check_EndPeriod())
	{
		m_iAnimationSoundCheck = m_pMeshCom->Get_CurrentAniIdx();
		ZeroMemory(m_bSoundPlay, sizeof(_bool) * 20);
	}

	if( m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::spawn ||
		m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::idle )
	{
		SoundEffectPlay(SOUND::jelly_idle, 0.0f, 1, 2.5f);
	}
	else if( m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::runE ||
		m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::runW ||
		m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::runN ||
		m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::runS )
	{
		SoundEffectPlay(SOUND::jelly_walk1, 0.0f, 1, 3.f);
	}
	else if( m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::jelly_attack_swoop )
	{
		SoundEffectPlay(SOUND::jelly_attack1, 0.0f, 1, 2.5f);
	}
	else if( m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::jelly_attack_explosion )
	{
		SoundEffectPlay(SOUND::jelly_attack_explosion, 0.0f, 1, 2.5f);
	}
	else if( m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::jelly_damage_weak_back ||
		m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::jelly_damage_weak_bottom)
	{
		SoundEffectPlay(SOUND::jelly_weak1, 0.0f, 1, 2.5f);
	}
	else if( m_pMeshCom->Get_CurrentAniIdx() == BloodJelly::jelly_damage_dying_bottom )
	{
		if(!m_bDieSound)
		{
			m_bDieSound = true;
			SoundEffectPlay(SOUND::jelly_damage_strong1, 0.0f, 1, 2.5f);
		}
	}
}

void CBloodJelly::Check_AttackRange()
{
	// ���� ���� �ȿ� ������ ����
	// ���� ���� ���̸� �������
	// ���� ���� ��2 �̸� ���ƴٴϱ� / ���

	if( m_iMonsterAnimationIndex !=  BloodJelly::jelly_attack_swoop ||
		m_iMonsterAnimationIndex !=  BloodJelly::jelly_attack_explosion )
	{
		Check_Rotation();
	}

	if(  m_pMeshCom->Check_EndPeriod() && m_bDie == false )
	{
		if( m_pTransCom->m_vScale.x != 0.1f )
			m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
		_vec3 vPlayerPos = CPlayer::GetInstance()->GetPos();

		float fDistance = D3DXVec3Length( &(vPlayerPos - m_pTransCom->m_vPosition) );

		if( fDistance < m_fFollowDistance )				// ���󰡴� ���� ��
		{
			if( fDistance < m_fAttackDistance )			// ���ݹ��� ��
			{
				printf("�����ϱ�\n");
				m_tMonsterInfo.fAttack = 100 + rand() % 100;
//  				if( m_iMonsterAnimationIndex !=  BloodJelly::jelly_attack_swoop &&
//  					m_iMonsterAnimationIndex !=  BloodJelly::jelly_attack_explosion)
				{
					int iRnd = rand() % 2 + 6;

					printf("attack : %d\n",iRnd);

					m_iMonsterAnimationIndex = iRnd;
					m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
				}
// 				else
// 				{
// 					m_iMonsterAnimationIndex = BloodJelly::idle;
// 					m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
// 				}
			}
 			else if( fDistance > m_fAttackDistance )	// ���ݹ��� �� & ���󰡴� ���� ��
 			{
				printf("���󰡱�\n");

 				if( m_iMonsterAnimationIndex !=  BloodJelly::runN )
 				{
 					m_iMonsterAnimationIndex = BloodJelly::runN;
 					m_pMeshCom->Set_AnimationSet(BloodJelly::runN);
 				}
 				else
 					m_pMeshCom->Set_AnimationSetSameFinish(BloodJelly::runN);
 			}
		}
		else	// ��� & ���ƴٴϱ�
		{
			Idle_Pattern();
		}
	}
}

void CBloodJelly::Check_Rotation()
{
	if( m_bDie )
		return;

	_vec3 m_vPlayerPos = CPlayer::GetInstance()->GetPos();
	_vec3 vDir = m_vPlayerPos - m_pTransCom->m_vPosition;
	D3DXVec3Normalize(&vDir, &vDir);

	memcpy( &m_vMonsterDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&m_vMonsterDir, &m_vMonsterDir);									
	D3DXVec3TransformNormal(&m_vMonsterDir, &m_vMonsterDir, &m_matRotationDefault);	

	_float fAngle = acosf( D3DXVec3Dot(&m_vMonsterDir, &vDir) );
	_vec3 vRight;

	memcpy( &vRight, &(m_pTransCom->m_matWorld.m[0][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&vRight, &vRight);									
	D3DXVec3TransformNormal(&vRight, &vRight, &m_matRotationDefault);	
	_float fAngle_Right = acosf( D3DXVec3Dot(&vRight, &vDir) );

	if( D3DXToDegree(fAngle_Right) <= 90.f )
		fAngle = D3DX_PI * 2.f - fAngle;
	//printf("%f // %f \n", m_vMonsterDir.x, m_vMonsterDir.z);


	if(D3DXToDegree(fAngle) - 5.f < m_fOrigAngle &&  m_fOrigAngle < D3DXToDegree(fAngle) + 5.f)
	{
		m_fOrigAngle = D3DXToDegree(fAngle);
	}
	else
	{
		if( D3DXToDegree(fAngle) >= 180 - 5.f)
		{
			m_fOrigAngle +=  (360 - D3DXToDegree(fAngle)) * 10.f * m_fTimeDelta;
		}
		else if( D3DXToDegree(fAngle) >= 0.f && D3DXToDegree(fAngle) < 180.f - 5.f )
		{
			m_fOrigAngle -=  (D3DXToDegree(fAngle)) * 10.f * m_fTimeDelta;
		}
	}

	if( m_fOrigAngle > (360.f) - 10.f)
		m_fOrigAngle = (0.f);
	else if( m_fOrigAngle < (0.f) + 10.f)
		m_fOrigAngle = (360.f);

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian( m_fOrigAngle ) + D3DXToRadian( -180.f );
}

void CBloodJelly::Set_Idle_Pattern()
{
	m_vecIdlePattern.push_back(BloodJelly::idle);
	m_vecIdlePattern.push_back(BloodJelly::runS);
	m_vecIdlePattern.push_back(BloodJelly::idle);
	m_vecIdlePattern.push_back(BloodJelly::runN);
	m_vecIdlePattern.push_back(BloodJelly::runW);
	m_vecIdlePattern.push_back(BloodJelly::idle);
	m_vecIdlePattern.push_back(BloodJelly::runE);
}

void CBloodJelly::Idle_Pattern()
{
	if( m_bDie )
		return;

	if( m_iIdlePattern > m_vecIdlePattern.size() - 1 )
		m_iIdlePattern = 0;

	printf("Idle : %d", m_iIdlePattern);
	m_iMonsterAnimationIndex = m_vecIdlePattern[m_iIdlePattern];
	m_pMeshCom->Set_AnimationSetSameFinish(m_vecIdlePattern[m_iIdlePattern]);
	++m_iIdlePattern;
}

void CBloodJelly::Release_Box()
{

}

_ulong CBloodJelly::Release(void)
{
	_ulong dwRefCnt = CMonster::Release();

	if(0 == dwRefCnt)
	{
		{
			list<CMonster*>::iterator iter = CCollisionMgr::GetInstance()->Get_MonsterList()->begin();
			list<CMonster*>::iterator iterEnd = CCollisionMgr::GetInstance()->Get_MonsterList()->end();

			for(; iter != iterEnd; )
			{
				if((*iter) == this)
					iter = CCollisionMgr::GetInstance()->Get_MonsterList()->erase(iter);
				else
					++iter;
			}
		}

		boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter = m_mapAniSync.begin();
		boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter_end = m_mapAniSync.end();

		for( ; iter != iter_end; )
		{
			vector<FOOTANISYNC*>::iterator iter_box = iter->second.begin();
			vector<FOOTANISYNC*>::iterator iter_box_end = iter->second.end();

			for( ; iter_box != iter_box_end; )
			{
				Engine::Safe_Delete( (*iter_box) );
				++iter_box;
			}
			iter->second.clear();
			iter = m_mapAniSync.erase(iter);

			if( iter == iter_end )
				m_mapAniSync.clear();
		}
		delete this;
	}

	return 0;
}
