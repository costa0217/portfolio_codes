#include "stdafx.h"
#include "RoyalGuard.h"

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

_vec3	CRoyalGuard::m_vPos;

CRoyalGuard::CRoyalGuard(LPDIRECT3DDEVICE9 pGraphicDev)
: CMonster(pGraphicDev)
, m_bDamage(false)
, m_bDisHP(false)
, m_bAttack(false)
, m_bDie(false)
, m_iAttackPattern(0)
, m_bEffectOnce(false)
, m_iWalkPattern(0)
, m_fPatternTime(0.f)
, m_fDieTime(0.f)
, m_fDieOverTime(0.f)
{

}
CRoyalGuard::~CRoyalGuard(void)
{

}
HRESULT CRoyalGuard::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Monster");
	if(NULL == m_pEffect)		return E_FAIL;

	m_pShadowEffect = Engine::Get_EffectHandle(L"Shader_Shadow");
	if(NULL == m_pShadowEffect)		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	//������ �ʱ�ȭ
	m_pTransCom->m_vScale = _vec3(0.08f, 0.08f, 0.08f);
	m_pTransCom->m_vPosition = m_vPos;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(90);

	m_iMonsterAnimationIndex = RoyalGuard::spawn;
	m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);

	// 	bInput = false;

	//�ٿ���ڽ� �ε�
	CMonster::Load_AttackBox(L"../Bin/Resources/Data/Box/Royal/Royal_AttackBox.dat");
	CMonster::Load_DamageBox(L"../Bin/Resources/Data/Box/Royal/Royal_DamageBox.dat");
	// Boss_HP UI

	// ���� �ε�
	Attack_Pattern();
	Walk_Pattern();

	// �ִϸ��̼� ��ũ
	CMonster::Load_FootAnimationSync(L"../Bin/Resources/Data/Box/Royal/Royal_Sync.dat");

	Initialize_Min_Max_Radius();

	m_fMaxDamageValue = 500.f;
	m_fDownValue = 0.f;
	m_fMonsterDownValue = 0.f;

	m_vMoveMin		= _vec3(-7.f, 0.f, -7.f);
	m_vMoveMax		= _vec3(7.f, 10.f, 7.f);
	m_fMoveRadius	= D3DXVec3Length(&(m_vMoveMax - m_vMoveMin)) * m_pTransCom->m_vScale.x;

	m_tMonsterInfo.fAttack = 100.f;
	m_tMonsterInfo.fMaxHP = 20000.f;
	m_tMonsterInfo.fHP = m_tMonsterInfo.fMaxHP;
	m_tMonsterInfo.fDownValue = 5.f; 

	//��ũ��ŭ �����ϰŸ� 1 
	m_fDistanceSize = 1.f;

	m_fAttackDistance = 20.f;
	m_fFollowDistance = 35.f;

	m_eMonsterType = MT_Monster;
	return S_OK;
}
HRESULT CRoyalGuard::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh Component	// �÷��̾ ������ ���̳��͸޽����� CDynamicMesh �� �ε��� ��.
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, L"Mesh_Royal");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_vAfterPos = m_pTransCom->m_vPosition;

	m_pMeshCom->SetLeftAndRihgtMatrix("ValveBiped_Bip01_L_Toe0", "ValveBiped_Bip01_R_Toe0");

	return S_OK;
}
_int CRoyalGuard::Update(const float& fTimeDelta)
{
	m_fTimeDelta		= fTimeDelta;
	m_fPatternTime += m_fTimeDelta;

	if( m_bMoveCollCheck == false)
		return -1;

	KeyInput();

	CMonster::CollisionCheckFunction();			//�浹�� ���� �˻縦 �Ѵ�.
	Check_Monster_Satae();						//�ִϸ��̼�, �ð�, Ư�� ���ǿ� ���� ������ ���� Ÿ���� ���������� üũ�Ͽ� �ٲپ� �ִ� �� => �� ���¿� ���� �÷��̾�� �浹 �˻縦 �ǳ� �ٰų� ���Ͱ� �ٿ� �����϶� �������� �� ���ų� �ٽ� �ٿ�ִϸ��̼��� Set ���ϵ��� �Ѵ�.
	AnimationPattern_Immediately_Change();		//�浹�� ���� ������ �ִϸ��̼��� �ﰢ������ �ٲ۴�. �浹 ������ ���� Monster class���� ���������� ������� �浹, Math Mgr���� ����� Ȱ��, ������ Ÿ���� ���� Ư�� ����� �������϶��� �浹 ���ص� �������� �ް� �ؾ� �Ѵ�.
	Check_PlayerAttack_AccordingToHP();			//�浹�� ���� ������ HP������ �ǰ� ����Ʈ ����
	BattleAnimationPattern();					//�÷��̾��� ���ݿ� �浹�� ���ϰ� �ִϸ��̼��� ������ ����� ���� �˰����� ���� ������ AI�� ���ư����� �Ѵ�.
	AnimationTimeChange();						//�⺻������ m_fAnimationSpeed�� �ӵ��� 2.f������ ���ο� ���³� Ư������� �ӵ��� ���⶧ ���⼭ �Ѵ�.

	//Check_Sync(); // �̰Ÿ� Render�� �Űܼ� ��ũ �ȸ´°� �ذ��ѰŰ���.
	SoundPlayer();

	CGameObject::Update(fTimeDelta);	
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_SHADOW, this);

	return 0;
}
void CRoyalGuard::Render(void)
{
	if(NULL == m_pEffect)		return;
	Set_ContantTable();	

	CMonster::Render();

	if(false == m_bDie)
	{
		m_pMeshCom->Move_Frame(m_fRenderTimeDelta);
	}
	else
	{
		if(!m_pMeshCom->Check_EndPeriod())
			m_pMeshCom->Move_Frame(m_fRenderTimeDelta);
	}
	m_pMeshCom->Render_MeshForShader(m_pEffect, false);

	Check_Sync();
}
void CRoyalGuard::Render_Shadow(void)
{
	if(NULL == m_pShadowEffect)		return;

	//����
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)	return;

	_matrix			matLightView, matLightProj;
	D3DXMatrixIdentity(&matLightView);
	D3DXMatrixIdentity(&matLightProj);
	_vec3	vAt, vUp, vEye;
	_vec3	vLightDir =  _vec3(pLightInfo->Direction);
	D3DXVec3Normalize(&vLightDir,  &vLightDir);

	//vEye = m_pTransCom->m_vPosition + ( vLightDir * -30.f);
	vEye = CPlayer::GetInstance()->Get_TransCom()->m_vPosition + (vLightDir * -100.f);
	vAt = vEye + vLightDir;
	vUp = _vec3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&matLightView, &vEye, &vAt, &vUp);
	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DX_PI * 0.4f, float(WINSIZEX) / WINSIZEY,  0.1f, 1000.f);

	//CPlayer::GetInstance()->Get_TransCom()->m_matWorld;
	m_pShadowEffect->SetMatrix("g_matLightWV", &(m_pTransCom->m_matWorld * matLightView));
	m_pShadowEffect->SetMatrix("g_matLightProj", &matLightProj);

	//m_pMeshCom->Render_MeshForShader(m_pShadowEffect);
	m_pMeshCom->Render_MeshForShaderShadow(m_pShadowEffect);
}
CRoyalGuard* CRoyalGuard::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CRoyalGuard*	pMonster = new CRoyalGuard(pGraphicDev);

	m_vPos = vPos;

	if( FAILED(pMonster->Initialize()) )
	{
		MSG_BOX("CRoyalGuard Create Failed");
		::Safe_Release(pMonster);
	}
	return pMonster;
}
void CRoyalGuard::KeyInput()
{
}
void CRoyalGuard::Set_ContantTable(void)
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
void CRoyalGuard::Check_Monster_Satae(void)
{

}
void CRoyalGuard::AnimationPattern_Immediately_Change(void)
{
	if(true == m_bDie)
	{
		if(m_pMeshCom->Check_EndPeriod())
		{
			printf("����\n");

			m_bMoveCollCheck = false;
		}
	}
	else
	{
		if( m_pMeshCom->Check_EndPeriod())
		{
			m_bEffectOnce = false;

			Check_Rotation();
			if(RoyalGuard::spawn== m_iMonsterAnimationIndex)
			{
				m_iMonsterAnimationIndex = RoyalGuard::idle;
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
			else if(RoyalGuard::down == m_iMonsterAnimationIndex)
			{
				//m_bDie = true;
			}
			else if(RoyalGuard::runN == m_iMonsterAnimationIndex)
			{
				_vec3 vPlayerPos = CPlayer::GetInstance()->GetPos();
				float fDistance = D3DXVec3Length( &(vPlayerPos - m_pTransCom->m_vPosition) );
				if(fDistance >= 10.f)
				{
					m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
				}
			}
		}
		if(RoyalGuard::spawn == m_iMonsterAnimationIndex)
		{
			if(m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.4f))
			{
				if(false == m_bEffectOnce)
				{
					//CSoundMgr::GetInstance()->PlayEffect(SOUND::step_lv3); �Ҹ� �̻��ؼ� �ϴ� ���� ������
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(-2.f, 0.f, 2.f));
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(0.f, 0.f, 2.f));
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(2.f, 0.f, 2.f));
					m_bEffectOnce = true;
				}
			}
			else
				m_bEffectOnce = false;
		}
	}
	
}
void CRoyalGuard::Check_PlayerAttack_AccordingToHP(void)
{
	if( (m_bPlayerAttack_Body_CollisionCheck || m_bPlayerEffect_Body_CollisionCheck) && false == m_bDie )	
	{
		m_bDamage = true;
	}
	else
		m_bDamage = false;

	if( m_bDamage )
	{
		if( m_bDisHP )
			return;

		m_bDisHP = true;
		printf("�浹�̴�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

		_float	m_fPlayerAttack = CPlayer::GetInstance()->GetPlayerAttackValue() + rand()%100;
		m_tMonsterInfo.fHP -= m_fPlayerAttack;
		//����ų ó��
		float MonsterHp = m_tMonsterInfo.fHP;	//���� ����ų�� ���� �ӽ÷� ���� 
		m_tMonsterInfo.fHP -= CPlayer::GetInstance()->GetPlayerAttackValue();
	
		if(m_tMonsterInfo.fHP <= 0.f)
		{
			m_tMonsterInfo.fHP = 0.f;

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
		printf("���� ü�� == %f\n", m_tMonsterInfo.fHP);

	
		CPlayer::GetInstance()->Player_AttackSuccess_InfoChange();
		CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);

		CPlayerBeShotUI::GetInstance()->PlayerBeShotUI();

		if(MS_Super != m_eMonsterState)
		{
			CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);

			_vec3 m_vPlayerPos = CPlayer::GetInstance()->GetPos();
			DamageDirectionType eType = CMathMgr::GetInstance()->DamageDirectionDecision(m_vPlayerPos, m_pTransCom);


			switch(eType)
			{
			case DDT_FRONT:  
				printf("DDT_FRONT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_back);
				break;
			case DDT_BACK:
				printf("DDT_BACK\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_bottom);
				break;
			case DDT_FRONT_LEFT:
				printf("DDT_FRONT_LEFT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_back);
				break;
			case DDT_BACK_LEFT:
				printf("DDT_BACK_LEFT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_bottom);
				break;
			case DDT_FRONT_RIGHT:
				printf("DDT_FRONT_RIGHT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_back);
				break;
			case DDT_BACK_RIGHT:
				printf("DDT_BACK_RIGHT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_bottom);
				break;
			}
		}
	}
	else
		m_bDisHP = false;

	if(false == m_bDie && m_tMonsterInfo.fHP <= 0)
	{
		m_iMonsterAnimationIndex = RoyalGuard::down;
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		SoundEffectPlay(SOUND::vampire_dying, 0.0f, 0, 2.f);
		m_bDie = true;
	}
}
void CRoyalGuard::BattleAnimationPattern(void)
{
	if(false == m_bDie && m_pMeshCom->Check_EndPeriod())
	{
		Check_AttackRange();
	}
}
void CRoyalGuard::AnimationTimeChange(void)
{
	m_fRenderTimeDelta	= m_fTimeDelta * m_fAnimationSpeed * 1.f;
}
void CRoyalGuard::Check_Sync()
{
	boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter = m_mapAniSync.find(m_iMonsterAnimationIndex);
	if( iter != m_mapAniSync.end() )
	{
		vector<FOOTANISYNC*>::iterator iter_vec = iter->second.begin();
		vector<FOOTANISYNC*>::iterator iter_end = iter->second.end();

		for( ; iter_vec != iter_end; ++iter_vec )
		{
			if(m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fStartTime) && !m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fEndTime))
			{
				CMonster::MonsterMoveFunctionTwo((*iter_vec)->bCompulsion, (*iter_vec)->bLeftFoot, (*iter_vec)->bFront);
			}
		}
	}
}
void CRoyalGuard::SoundPlayer(void)
{
	if(m_pMeshCom->Get_CurrentAniIdx() != m_iAnimationSoundCheck || m_pMeshCom->Check_EndPeriod())
	{
		m_iAnimationSoundCheck = m_pMeshCom->Get_CurrentAniIdx();
		ZeroMemory(m_bSoundPlay, sizeof(_bool) * 10);
	}

	if(RoyalGuard::vampire_attack_double == m_iMonsterAnimationIndex)
	{
		SoundEffectPlay(SOUND::Swing_MetalStrong, 0.45f, 0, 2);
		SoundEffectPlay(SOUND::Swing_MetalStrong, 0.57f, 1, 2);
	}
	else if(RoyalGuard::vampire_attack_pierce == m_iMonsterAnimationIndex)
	{
		SoundEffectPlay(SOUND::vampire_rapier_pierce, 0.54f, 0, 2);
	}
}

void CRoyalGuard::Check_AttackRange()
{
	_vec3 vPlayerPos = CPlayer::GetInstance()->GetPos();

	float fDistance = D3DXVec3Length( &(vPlayerPos - m_pTransCom->m_vPosition) );

	if( fDistance < m_fFollowDistance )				// ���󰡴� ���� ��
	{
		if( fDistance <= m_fAttackDistance )			// ���ݹ��� ��
		{
			m_tMonsterInfo.fAttack = 100 + rand() % 200;
			if(m_iAttackPattern >= (_int)m_vecAttackPattern.size())
				m_iAttackPattern = 0;
			if(m_iMonsterAnimationIndex == m_vecAttackPattern[m_iAttackPattern])
			{
				m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
			}
			else
			{
				m_iMonsterAnimationIndex = m_vecAttackPattern[m_iAttackPattern];
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
			++m_iAttackPattern;
		}
		else// ���ݹ��� �� & ���󰡴� ���� ��
		{
			if(m_iMonsterAnimationIndex == m_vecWalkPattern[m_iWalkPattern])
			{
				m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
			}
			else
			{
				m_iMonsterAnimationIndex = m_vecWalkPattern[m_iWalkPattern];
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
			++m_iWalkPattern;
			if(m_iWalkPattern >= (_int)m_vecWalkPattern.size())
			{
				_int i = ((_int)m_fPatternTime) % 3;
				m_iWalkPattern = i * 3;
			}
		}
	}
	else
	{
		if(m_iMonsterAnimationIndex == RoyalGuard::runN)
		{
			m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
		}
		else
		{
			m_iMonsterAnimationIndex = RoyalGuard::runN;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
	}

}
void CRoyalGuard::Check_Rotation()
{
	_vec3 vDir = CPlayer::GetInstance()->GetPos() - m_pTransCom->m_vPosition;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vMonsterDir;
	memcpy( &vMonsterDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&vMonsterDir, &vMonsterDir);	

	vMonsterDir *= -1.f; //x���� ��ü�� 0 0 -1�����ִµ� ���� ���⺤�ʹ� 0 0 1�̶� ������
	//�÷��̾���� ������ ����.
	_float fAngle = acosf( D3DXVec3Dot(&vMonsterDir, &vDir) );

	//acos�Լ��� 0 ~ 180�� �����������.
	_matrix matRotationDefault;
	D3DXMatrixRotationY(&matRotationDefault, D3DXToRadian(0.f));
	_vec3 vRight;
	memcpy( &vRight, &(m_pTransCom->m_matWorld.m[0][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&vRight, &vRight);									
	D3DXVec3TransformNormal(&vRight, &vRight, &matRotationDefault);	
	_float fAngle_Right = acosf( D3DXVec3Dot(&vRight, &vDir) );

	if( D3DXToDegree(fAngle_Right) <= 90.f )
		fAngle = D3DX_PI * 2.f - fAngle;//fAngle = fAngle * -1.f;

	//�Ҽ��� ���� 
	_float fResultAngle = (_int)((m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] + fAngle) * 100 ) / 100.f;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fResultAngle;

}
void CRoyalGuard::Attack_Pattern()
{
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
}
void CRoyalGuard::Walk_Pattern()
{
	m_vecWalkPattern.push_back(RoyalGuard::walkE);	//0
	m_vecWalkPattern.push_back(RoyalGuard::walkE);
	m_vecWalkPattern.push_back(RoyalGuard::idle);

	m_vecWalkPattern.push_back(RoyalGuard::walkW);	//3
	m_vecWalkPattern.push_back(RoyalGuard::walkW);
	m_vecWalkPattern.push_back(RoyalGuard::idle);
	
	m_vecWalkPattern.push_back(RoyalGuard::walkN);	//6
	m_vecWalkPattern.push_back(RoyalGuard::walkN);
	m_vecWalkPattern.push_back(RoyalGuard::idle);
}
_ulong CRoyalGuard::Release(void)
{
	_ulong dwRefCnt = CMonster::Release();

	if(0 == dwRefCnt)
	{
		m_vecAttackPattern.clear();
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





