#include "stdafx.h"
#include "Boss.h"
#include "NaviMesh.h"
#include "Export_Function.h"
#include "Player.h"
#include "BossSword.h"
#include "FireBall.h"
#include "IceAge.h"
#include "IceThone.h"

#include "Layer.h"
#include "Blood.h"
#include "BloodBlack.h"
#include "MagicCircle.h"
#include "Shock.h"
#include "SmokeSphere.h"
#include "IceUp.h"
#include "Wang.h"
#include "Swirl.h"
#include "SwordCircle.h"

#include "SDevice.h"


#define HIBER_CNT 5.f
#define dSKILL_TIME 5.f

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
: CLandObject(pGraphicDev)
, m_pMeshCom(NULL)
, m_pEffect(NULL)
, m_fTimeDelta(0.f)
, m_fSpeed(0.f)
, m_pSwordTransCom(NULL)
, m_pShieldTransCom(NULL)
, m_pPlayerTransCom(NULL)
, m_pHpCom(NULL)

, m_bColl(false)
, m_bDMG(false)
, m_bATK(false)
, m_fFixTime(0.4f)
, m_iDMGNum(0)		//몇번 연속 맞았는지

, m_fAttackRadius(0)
, m_pmatRForearm(NULL)
, m_pmatLForearm(NULL)

//Min Max Init
, m_pShieldOBBInfo(NULL)
, m_pSwordOBBInfo(NULL)
, m_pMyOBBInfo(NULL)
, m_pPlayerOBBInfo(NULL)

, m_pvShieldMax(NULL), m_pvShieldMin(NULL)
, m_pvSwordMin(NULL), m_pvSwordMax(NULL)
, m_pvPlayerMin(NULL), m_pvPlayerMax(NULL)
, m_pvMyMin(NULL), m_pvMyMax(NULL)
, m_fSwordSphereRad(0.f), m_fPlayerSphereRad(0.f)
, m_fMySphereRad(0.f), m_fShieldSphereRad(0.f)

, m_fAtkStartTime(0.f)
, m_fHibernateTime(0.f)
, m_bHiber(false)
, m_fSkillTime(0.f)
, m_iRandPetternNum(0)			//장풍

, m_bSkillStart(false)
, m_iDownCnt(0)
, m_iPlayerAtkIdx(-1)
, m_iPrePlayerAtkIdx(-1)

, m_bStart(false)
, m_bOpening(false)
, m_bDie(false)
, m_fDieTime(0.f)
, m_fAlpha(1.f)
, m_vViewPos(0.f, 0.f, 0.f)
{
	ZeroMemory(m_bPettern, sizeof(_bool) * 5);
}


CBoss::~CBoss(void)
{

}

HRESULT CBoss::Initialize(void)
{
	if(FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_MeshesAlpha");
	if(NULL == m_pEffect)		return E_FAIL;		

	if(FAILED(Add_Component()))	return E_FAIL;

	m_eObjType = TYPE_NAVI;
	m_eAniStat = Boss_Idle2;

	D3DXMatrixScaling(&m_mDMGTrans, 0.7f, 1.f, 0.7f);
	D3DXMatrixScaling(&m_mATKTrans, 1.3f, 1.2f, 1.3f);

	m_pMeshCom->Set_AnimationSet(m_eAniStat);

	m_pTransCom->m_vScale		= _vec3(0.12f, 0.12f, 0.12f);
	m_pTransCom->m_vPosition	= _vec3(67.f, 0.f, 208.f);

	m_dwNaviIndex = m_pNaviMesh->FindIndex(m_pTransCom->m_vPosition);
	m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &m_pTransCom->m_vDir, m_dwNaviIndex, m_fSpeed, m_fTimeDelta);
	m_fSpeed = 3.f;


	m_fAttackRadius = 7.f;
	

#ifdef _DEBUG
	D3DXCreateSphere(m_pGraphicDev, m_fAttackRadius, 6, 6, &m_pSphereMesh, &m_pSphereAdjancey);
	D3DXCreateSphere(m_pGraphicDev, m_fAttackRadius, 6, 6, &m_pSphere2Mesh, &m_pSphere2Adjancey);
#endif

	m_fAttackRadius *= m_pTransCom->m_vScale.x;

	Render();

	return S_OK;
}

HRESULT CBoss::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Boss");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.HP Component
	pComponent = m_pHpCom = Engine::CHPoint::Create(400);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_HP", pComponent));

	return S_OK;
}

_int CBoss::Update(const _float& fTimeDelta)
{	
	m_fTimeDelta	= fTimeDelta;


	m_bColl = false;
	if(m_pSwordTransCom == NULL)// 데이터들 가져오기
	{
		Set_Data();
	}
	m_fMySphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * m_pTransCom->m_vScale.x;
	m_fPlayerSphereRad = D3DXVec3Length(&(*m_pvPlayerMax - *m_pvPlayerMin)) * m_pPlayerTransCom->m_vScale.x;

	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

	D3DXVec3TransformCoord(&m_Finger1Pos, &m_FinLocalpos1, &(matScale * *m_pFin1Matrix));
	D3DXVec3TransformCoord(&m_Finger2Pos, &m_FinLocalpos2, &(matScale * *m_pFin2Matrix));

	D3DXVec3TransformCoord(&m_Finger1Pos2, &m_FinLocalpos12, &(matScale * *m_pFin1Matrix2));
	D3DXVec3TransformCoord(&m_Finger2Pos2, &m_FinLocalpos22, &(matScale * *m_pFin2Matrix2));

	if(!m_bStart)
	{
		if(D3DXVec3Length(&(m_pTransCom->m_vPosition - m_pPlayerTransCom->m_vPosition)) < 100.f)
		{
			//m_bStart = true;

			if(!m_bOpening)
			{
				m_pMeshCom->Set_AnimationSet(Boss_Idle1);
				m_eAniStat = Boss_Idle1;

				CSDevice::GetInstance()->SoundPlay(SD_BOSSOPENING, 0);
				m_bOpening = true;
				
			}
			if(true == m_pMeshCom->Check_EndPeriod(m_fFixTime))
			{
				m_eAniStat = Boss_Hibernate;
				m_pMeshCom->Set_AnimationSet(Boss_Hibernate);

				m_bStart = true;
			}
		}
	}

	if(m_bStart)
	{
		if(m_pHpCom->m_iHP > 0)
		{
			if(!m_bHiber && !m_bOpening)
				Pattern_Move();

			if(m_eAniStat == Boss_Down){ m_fFixTime = 0.5f; }
			else{ m_fFixTime = 0.05f; }

			if(true == m_pMeshCom->Check_EndPeriod(m_fFixTime)
				&& m_eAniStat != Boss_Run 
				&& m_eAniStat != Boss_Hibernate)	// 애니메이션 셋팅
			{
				if(m_eAniStat == Boss_Initate)
					m_bOpening = false;
				if(m_eAniStat ==Boss_PassOut)
				{
					m_pMeshCom->Set_AnimationSet(Boss_Idle1);
					m_eAniStat = Boss_Idle1;
				}
				else if(m_eAniStat == Boss_Down)
				{
					m_pMeshCom->Set_AnimationSet(Boss_Down_Loop);
					m_eAniStat = Boss_Down_Loop;
				}
				else if(m_eAniStat == Boss_Down_Loop)
				{
					if(m_iDownCnt > 10)
					{
						m_pMeshCom->Set_AnimationSet(Boss_Get_Up);
						m_eAniStat = Boss_Get_Up;
					}
					else
					{
						++m_iDownCnt;
						m_pMeshCom->Set_AnimationSet(Boss_Down_Loop);
					}			
				}
				else
				{
					m_pMeshCom->Set_AnimationSet(Boss_Idle2);
					m_eAniStat = Boss_Idle2;
					m_iDownCnt = 0;
				}

				if(m_bDMG && !m_bColl)
					m_bDMG = false;
				if(m_bATK)
					m_bATK = false;

				m_fAtkStartTime = 0.f;

				if(m_bPettern[m_iRandPetternNum])
				{
					m_bPettern[m_iRandPetternNum] = false;
					m_fSkillTime = 0.f;	

					m_bSkillStart = false;

					++m_iRandPetternNum;
					if(m_iRandPetternNum > 4)
						m_iRandPetternNum = 0;
				}		
			}
			Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
			CLandObject::Update(fTimeDelta);

			if(m_eAniStat == Boss_Idle1) return 0;
			if(m_eAniStat == Boss_Down) return 0;
			if(m_eAniStat == Boss_Down_Loop) return 0;
			if(m_eAniStat == Boss_Get_Up) return 0;

			Fire_Skill5();		// 얼음장판
			Fire_Skill3();		// 파이어 볼
			Fire_Skill4();		// 칼날리기

			if(m_fSkillTime > dSKILL_TIME
				&& !m_bHiber )
			{
				Pattern_Skill();
			}
			else
			{
				Check_Collison();				// 체크 콜리젼 칼과 보스
				Pattern_Hibernate(fTimeDelta);	// 데미지 & 숨는패턴
			}

			if(!m_bHiber /*&& !m_bATK*/)
				m_fSkillTime	+= fTimeDelta;	// 스킬 타임

			Attack_Player();
		}
		else
		{
			m_fDieTime += fTimeDelta;
			if(m_eAniStat != Boss_Die && !m_bDie)
			{
				m_pMeshCom->Set_AnimationSet(Boss_Die);
				m_eAniStat = Boss_Die;
				m_bDie = true;
				m_fFixTime = 0.3f;
				CSDevice::GetInstance()->SoundPlay(SD_Boss_Death, 0);
			}	
			if(true == m_pMeshCom->Check_EndPeriod(m_fFixTime) 
				&& m_bDie)
			{
				m_pMeshCom->Set_AnimationSet(Boss_DeadBody);
				m_eAniStat = Boss_DeadBody;
			}

			if(m_fDieTime > 5.f)
			{
				m_fAlpha -= fTimeDelta * 0.2f;
				if(m_fAlpha < 0.5f)
				{
					m_pTransCom->m_vPosition.y += fTimeDelta;
				}
				_matrix			matView;
				m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
				D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 

				Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);
				CLandObject::Update(fTimeDelta);
			}
			else
			{
				Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
				CLandObject::Update(fTimeDelta);
			}			
		}		
	}
	else
	{
		Compute_Dir();
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
		CLandObject::Update(fTimeDelta);

	}

	return 0;
}

void CBoss::Render(void)
{
	if(NULL == m_pEffect)	return;
	if(m_bHiber)	return;


	m_pMeshCom->Move_Frame(m_fTimeDelta);
	Set_ContantTable();

#ifdef _DEBUG

	_matrix			mDestTrans;
	D3DXMatrixScaling(&mDestTrans, 0.7f, 1.f, 0.7f);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &(mDestTrans *m_pTransCom->m_matWorld));
#endif	 

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

#ifdef _DEBUG
	if(m_pSwordTransCom != NULL)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &(*m_pmatLForearm /** m_mDMGTrans*/ * m_pTransCom->m_matWorld));
		m_pSphereMesh->DrawSubset(0);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &(*m_pmatRForearm /** m_mDMGTrans */* m_pTransCom->m_matWorld));
		m_pSphere2Mesh->DrawSubset(0);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
#endif	
}

void CBoss::Check_Collison(void)
{
	
	//플레이어 어택에 의한 칼과 보스의 충돌
	if( ((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_ATTACK
		|| ((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_SKILL)
	{
		if(((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_ATTACK)
		{
			_vec3 vSwordWorldPos(0.f, 0.f, 0.f);
			D3DXVec3TransformCoord(&vSwordWorldPos, &m_pSwordTransCom->m_vPosition, &m_pSwordTransCom->m_matWorld);

			if(Engine::Collision_SphereSphere(vSwordWorldPos, m_fSwordSphereRad, m_pTransCom->m_vPosition, m_fMySphereRad))
			{				
				m_bColl = Engine::Collision_OBB(*m_pMyOBBInfo, m_mDMGTrans * m_pTransCom->m_matWorld, *m_pSwordOBBInfo, m_pSwordTransCom->m_matWorld);
	
				if(m_bColl && m_eAniStat != Boss_Hibernate)
				{
					m_iPlayerAtkIdx = ((CPlayer*)m_pPlayer)->Get_AttackCnt();
					if(m_iPlayerAtkIdx != m_iPrePlayerAtkIdx)
					{
						//피 생성
						_vec3 vBloodPos(0.f, 0.f, 0.f);
						_vec3 vBloodDir(0.f, 0.f, 0.f);

						D3DXVec3TransformCoord(&vBloodPos, &m_pMyOBBInfo->vCenterPos, &m_pTransCom->m_matWorld);
						vBloodDir = m_pPlayerTransCom->m_vPosition - m_pTransCom->m_vPosition;
						D3DXVec3Normalize(&vBloodDir, &vBloodDir);			

						Create_Blood(vBloodPos + (vBloodDir /** .f*/));
						//Create_HitShock();						
					}
					m_iPrePlayerAtkIdx = m_iPlayerAtkIdx;
				}
				else
				{
					m_iPrePlayerAtkIdx = 0;
					m_iPlayerAtkIdx = 0;
				}
				//MSG_BOX("Sword Hit");
			}
		}
		else
		{
			int iSkillIdx = ((CPlayer*)m_pPlayer)->Get_SkillIndex();

			if(iSkillIdx == 1)
			{
				_vec3 vSwordWorldPos(0.f, 0.f, 0.f);
				D3DXVec3TransformCoord(&vSwordWorldPos, &m_pSwordTransCom->m_vPosition, &m_pSwordTransCom->m_matWorld);
				if(Engine::Collision_SphereSphere(vSwordWorldPos, m_fSwordSphereRad, m_pTransCom->m_vPosition, m_fMySphereRad))
				{
					if(Engine::Collision_OBB(*m_pMyOBBInfo, m_mDMGTrans * m_pTransCom->m_matWorld, *m_pSwordOBBInfo, m_pSwordTransCom->m_matWorld))
					{
						//MSG_BOX("Down Hit");
						m_eAniStat = Boss_Down;
						m_pMeshCom->Set_AnimationSet(Boss_Down);
					}
				}				
			}
			else if(iSkillIdx == 2)
			{
				_vec3 vShieldWorldPos(0.f, 0.f, 0.f);
				D3DXVec3TransformCoord(&vShieldWorldPos, &m_pShieldTransCom->m_vPosition, &m_pShieldTransCom->m_matWorld);
				if(Engine::Collision_SphereSphere(vShieldWorldPos, m_fShieldSphereRad, m_pTransCom->m_vPosition, m_fMySphereRad))
				{
					if(Engine::Collision_OBB(*m_pMyOBBInfo, m_mDMGTrans * m_pTransCom->m_matWorld, *m_pShieldOBBInfo, m_pShieldTransCom->m_matWorld))
					{
						//MSG_BOX("PassOut Hit");

						if(m_eAniStat != Boss_PassOut)
							Create_HitShock();	
						m_eAniStat = Boss_PassOut;
						m_pMeshCom->Set_AnimationSet(Boss_PassOut);

						CSDevice::GetInstance()->SoundPlay(SD_Boss_PassOut, 0);

						m_fSkillTime = 0.f;
						
						
						//m_pTransCom->m_vPosition.y += 1.f;
					}
				}
			}
			else
			{
				_vec3 vSwordWorldPos(0.f, 0.f, 0.f);
				D3DXVec3TransformCoord(&vSwordWorldPos, &m_pSwordTransCom->m_vPosition, &m_pSwordTransCom->m_matWorld);
				if(Engine::Collision_SphereSphere(vSwordWorldPos, m_fSwordSphereRad, m_pTransCom->m_vPosition, m_fMySphereRad))
				{
					//_vec3 vBloodPos(0.f, 0.f, 0.f);
					//_vec3 vBloodDir(0.f, 0.f, 0.f);

					//D3DXVec3TransformCoord(&vBloodPos, &m_pMyOBBInfo->vCenterPos, &m_pTransCom->m_matWorld);
					//vBloodDir = m_pPlayerTransCom->m_vPosition - m_pTransCom->m_vPosition;
					//D3DXVec3Normalize(&vBloodDir, &vBloodDir);	

					//Create_Blood(vBloodPos + (vBloodDir /** .f*/));
					m_bColl = Engine::Collision_OBB(*m_pMyOBBInfo, m_mDMGTrans * m_pTransCom->m_matWorld, *m_pSwordOBBInfo, m_pSwordTransCom->m_matWorld);
				}
			}			 
		}	
	}
}

void CBoss::Compute_Dir(void)
{
	float fMeshAngle = acos(D3DXVec3Dot(&m_pTransCom->m_vDir, &g_vLook));
	if(m_pTransCom->m_vDir.z > g_vLook.z)	fMeshAngle = D3DX_PI * 2 - fMeshAngle;

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fMeshAngle;

	m_pTransCom->m_vDir = m_pPlayerTransCom->m_vPosition - m_pTransCom->m_vPosition;
	m_pTransCom->m_vDir = _vec3(m_pTransCom->m_vDir.x, 0.f, m_pTransCom->m_vDir.z);
	D3DXVec3Normalize(&m_pTransCom->m_vDir, &m_pTransCom->m_vDir);
}

void CBoss::Pattern_Move(void)
{

	if(m_bPettern[m_iRandPetternNum] ) return;
	if(m_eAniStat == Boss_Idle1) return;
	if(m_eAniStat == Boss_PassOut) return;
	
	if(m_eAniStat == Boss_Down) return;
	if(m_eAniStat == Boss_Down_Loop) return;
	if(m_eAniStat == Boss_Get_Up) return;

	m_fFixTime = 0.05f;

	if(m_bDMG) return;
	if(m_eAniStat == Boss_ATK_01
		|| m_eAniStat == Boss_ATK_02) return;

	if(m_eAniStat != Boss_Run)
	{
		m_eAniStat = Boss_Run;
		m_pMeshCom->Set_AnimationSet(Boss_Run);		
	}

	Compute_Dir();

	m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &m_pTransCom->m_vDir, m_dwNaviIndex, m_fSpeed, m_fTimeDelta);
}
void CBoss::Pattern_Hibernate(const float& fTimeDelta)
{
	/**********데미지입는부분*********/
	if(m_iDMGNum > HIBER_CNT )
	{
		if(m_eAniStat == Boss_DMG_F)
		{
			m_eAniStat = Boss_Hibernate;
			m_pMeshCom->Set_AnimationSet(Boss_Hibernate);
			m_iDMGNum = 0;
			m_fSkillTime = 0.f; //스킬타임 초기화.

			CSDevice::GetInstance()->SoundPlay(SD_Boss_In, 0);
		}		
	}
	else
	{
		if(m_bColl && !m_bDMG )
		{
			//MSG_BOX("DMG Hit");
			m_bDMG = true;
			m_eAniStat = Boss_DMG_F;
			m_pMeshCom->Set_AnimationSet(Boss_DMG_F);

			if(CSDevice::GetInstance()->SoundPlaying(SD_BOSSATTACK1)
				|| CSDevice::GetInstance()->SoundPlaying(SD_BOSSATTACK2))
			{
				CSDevice::GetInstance()->SoundStop(SD_BOSSATTACK1);
				CSDevice::GetInstance()->SoundStop(SD_BOSSATTACK2);
			}

			CSDevice::GetInstance()->SoundPlay(SD_BossDMG1 + rand()%2, 0);

			m_pHpCom->m_iHP -= 30;

			++m_iDMGNum; 
		}
	}
	/*********************************/
	if(m_eAniStat == Boss_Hibernate)
	{
		m_fFixTime = 0.1f;
		if(true == m_pMeshCom->Check_EndPeriod(m_fFixTime)
			&& !m_bHiber)
		{
			_float	fInitateDist;
			_vec3	vInitateDir;
			_float	fRandAngle;
			int i = 0;

			_matrix matRotY;
			if(m_bOpening)
			{
				
				m_dwNaviIndex = m_pNaviMesh->FindIndex(_vec3(67.f, 0.f, 165.f));
				m_pNaviMesh->Set_NaviCenterPos(&m_pTransCom->m_vPosition, m_dwNaviIndex);
			}
			else
			{
				while(true)
				{
					fRandAngle		= D3DXToRadian( rand()%360 );
					fInitateDist	= rand()%20 + 2.f;
					vInitateDir		= _vec3(fInitateDist, 0.f, 0.f);

					D3DXMatrixIdentity(&matRotY);
					D3DXMatrixRotationY(&matRotY, fRandAngle);
					D3DXVec3TransformNormal(&vInitateDir, &vInitateDir, &matRotY);

					m_dwNaviIndex = m_pNaviMesh->FindIndex(m_pTransCom->m_vPosition + vInitateDir);

					if(m_dwNaviIndex != -1)
						break;			
				}
				m_pNaviMesh->Set_NaviCenterPos(&m_pTransCom->m_vPosition, m_dwNaviIndex);
			}
			

			m_bHiber = true;
			m_fSkillTime = 0.f; //스킬타임 초기화.
		}

		if(m_bHiber)
		{
			m_fHibernateTime += fTimeDelta;

			if(m_fHibernateTime > 2.f)
			{
				CSDevice::GetInstance()->SoundPlay(SD_Boss_Out, 0);

				Compute_Dir();
				m_pMeshCom->Set_AnimationSet(Boss_Initate);
				m_eAniStat = Boss_Initate;
				m_fTimeDelta *= 0.5f;
				m_fHibernateTime = 0.f;		
				
				m_bHiber = false;
			}
		}
	}
}
void CBoss::Pattern_Skill(void)
{
	if(!m_bPettern[m_iRandPetternNum])
	{
		/*m_iRandPetternNum = rand()%4;*/

		switch(m_iRandPetternNum)
		{
		case 0:
			m_pMeshCom->Set_AnimationSet(Boss_SK_Firing_01);
			m_eAniStat = Boss_SK_Firing_01;

			CSDevice::GetInstance()->SoundPlay(SD_BOSSSKILL1, 0);

			Create_Swirl();
			break;
		case 1:
			m_pMeshCom->Set_AnimationSet(Boss_SK_Firing_02);
			m_eAniStat = Boss_SK_Firing_02;

			CSDevice::GetInstance()->SoundPlay(SD_BOSSSKILL2, 0);

			Create_Wang();
			break;
		case 2:
			m_pMeshCom->Set_AnimationSet(Boss_SK_Firing_03);
			m_eAniStat = Boss_SK_Firing_03;

			CSDevice::GetInstance()->SoundPlay(SD_BOSSSKILL3, 0);
			break;
		case 3:
			m_pMeshCom->Set_AnimationSet(Boss_SK_Firing_04);
			m_eAniStat = Boss_SK_Firing_04;

			CSDevice::GetInstance()->SoundPlay(SD_BOSSSKILL4, 0);
			break;
		case 4:
			m_pMeshCom->Set_AnimationSet(Boss_SK_Firing_05);
			m_eAniStat = Boss_SK_Firing_05;

			CSDevice::GetInstance()->SoundPlay(SD_BOSSSKILL5, 0);
			break;
		}
		Create_MagicCircle(m_iRandPetternNum);
		m_bPettern[m_iRandPetternNum] = true;
	}	
}
void CBoss::Fire_Skill3(void)
{
	if(m_bPettern[2])
	{
		if(!m_bSkillStart)
		{
			((CFireBall*)m_pFireBall)->Set_DataInit();
			((CFireBall*)m_pFireBall)->Set_Pos(m_pTransCom->m_vPosition + (_vec3(0.f, 1.f, 0.f) * 10.f));
			((CFireBall*)m_pFireBall)->Set_Switch(true);

			((CFireBall*)m_pFireBall)->Set_Layer(m_pGameLogicLayer);
		}
		Compute_Dir();
		m_bSkillStart = true;
	}
}

void CBoss::Fire_Skill4(void)
{
	if(m_bPettern[3])
	{
		if(!m_bSkillStart)
		{
			list<Engine::CGameObject*>:: iterator iter = m_pBossSwordList->begin();
			list<Engine::CGameObject*>:: iterator iter_end = m_pBossSwordList->end();

			_vec3 vRight(0.f, 0.f, 0.f);
			_vec3 vLook(0.f, 0.f, 0.f);

			vLook = m_pTransCom->m_vDir;
			D3DXVec3Normalize(&vLook, &vLook);
			D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
			D3DXVec3Normalize(&vRight, &vRight);

			_matrix		matAxis;

			_float		fAngle	= D3DXToRadian(30.f);
			_int		iCnt	= 1;

			for( ; iter != iter_end; ++iter)
			{
				_float	fRotAngle = fAngle * iCnt;
				_vec3	vCreateDir;
				D3DXMatrixRotationAxis(&matAxis, &vLook, fRotAngle);
				D3DXVec3TransformNormal(&vCreateDir, &vRight, &matAxis);
				D3DXVec3Normalize(&vCreateDir, &vCreateDir);

				((CBossSword*)(*iter))->Set_DataInit();
				((CBossSword*)(*iter))->Set_Pos(m_pTransCom->m_vPosition + (vCreateDir * 10.f));
				((CBossSword*)(*iter))->Set_Switch(true);

				Create_SwordCircle(m_pTransCom->m_vPosition + (vCreateDir * 10.f));

				++iCnt;
			}
		}
		m_bSkillStart = true;		
	}
}
void CBoss::Fire_Skill5(void)
{
	if(m_bPettern[4])
	{
		if(!m_bSkillStart)
		{
			list<Engine::CGameObject*>:: iterator iter = m_pIceAgeList->begin();
			list<Engine::CGameObject*>:: iterator iter_end = m_pIceAgeList->end();

			for( ; iter != iter_end; ++iter)
			{
				((CIceAge*)(*iter))->Set_DataInit();
				((CIceAge*)(*iter))->Set_Pos(m_pTransCom->m_vPosition);
				((CIceAge*)(*iter))->Set_Switch(true);
			}

			list<Engine::CGameObject*>:: iterator Iceiter = m_pIceThoneList->begin();
			list<Engine::CGameObject*>:: iterator Iceiter_end = m_pIceThoneList->end();

			for( ; Iceiter != Iceiter_end; ++Iceiter)
			{
				_vec3 vCreatePos(0.f, 0.f, 0.f);
				while(true)
				{
					_float	fRotAngle = D3DXToRadian(rand()%361);
					_float	fDist	  = float(rand()%30) + 5.f;
					_vec3	vCreateDir;
					_matrix matRotY;
					_ulong	dwNaviIdx;

					D3DXMatrixRotationY(&matRotY, fRotAngle);
					D3DXVec3TransformNormal(&vCreateDir, &_vec3(1.f, 0.f, 0.f), &matRotY);

					dwNaviIdx = m_pNaviMesh->FindIndex(m_pTransCom->m_vPosition + (vCreateDir * fDist));

					if(dwNaviIdx != -1)
					{
						vCreatePos = m_pTransCom->m_vPosition + (vCreateDir * fDist);
						break;			
					}
				}
				((CIceThone*)(*Iceiter))->Set_DataInit();
				((CIceThone*)(*Iceiter))->Set_Pos(vCreatePos);
				((CIceThone*)(*Iceiter))->Set_Switch(true);

				Create_Smoke(vCreatePos);
				//Create_IceUp(vCreatePos);
			}
			CSDevice::GetInstance()->SoundPlay(SD_CreateIce, 0);
		}
		m_bSkillStart = true;		
	}
}

void CBoss::Attack_Player(void)
{
	if(m_bPettern[m_iRandPetternNum] ) return;
	if(m_eAniStat == Boss_Hibernate) return;
	if(m_eAniStat == Boss_Down) return;
	if(m_eAniStat ==Boss_PassOut) return;


	if(m_bATK)
	{
		m_fAtkStartTime += m_fTimeDelta;
		if(((CPlayer*)m_pPlayer)->Get_Damaged())
		{
			m_fAtkStartTime = 0.f;
			return;
		}
		float fComputeTime = -0.1f;
		if(m_eAniStat == Boss_ATK_01)
			fComputeTime  = 0.2f;

		if(m_fAtkStartTime < 0.5f - fComputeTime )
			return;

		_vec3 vLeftPos(0.f, 0.f, 0.f);
		_vec3 vRightPos(0.f, 0.f, 0.f);
		memcpy(&vLeftPos, &m_pmatLForearm->m[3][0], sizeof(_vec3));
		memcpy(&vRightPos, &m_pmatRForearm->m[3][0], sizeof(_vec3));
		D3DXVec3TransformCoord(&vLeftPos, &vLeftPos, &m_pTransCom->m_matWorld);
		D3DXVec3TransformCoord(&vRightPos, &vRightPos, &m_pTransCom->m_matWorld);

		_vec3 vPlayerWorldCenPos(0.f, 0.f, 0.f);
		D3DXVec3TransformCoord(&vPlayerWorldCenPos, &m_pPlayerOBBInfo->vCenterPos, &m_pPlayerTransCom->m_matWorld);

		if(Engine::Collision_SphereSphere(vLeftPos, m_fAttackRadius, vPlayerWorldCenPos, m_fPlayerSphereRad))
		{
			if(Engine::Collision_SphereOBB(*m_pvPlayerMin, *m_pvPlayerMax, m_pPlayerTransCom->m_matWorld, vLeftPos, m_fAttackRadius))
			{
				((CPlayer*)m_pPlayer)->Set_Damaged(true);
				//MSG_BOX("LEFT TRUE");
				m_fAtkStartTime = 0.f;
			}
		}
		else if(Engine::Collision_SphereSphere(vRightPos, m_fAttackRadius, vPlayerWorldCenPos, m_fPlayerSphereRad))
		{
			if(Engine::Collision_SphereOBB(*m_pvPlayerMin, *m_pvPlayerMax, m_pPlayerTransCom->m_matWorld, vRightPos, m_fAttackRadius))
			{
				((CPlayer*)m_pPlayer)->Set_Damaged(true);
				//MSG_BOX("RIGHT TRUE");
				m_fAtkStartTime = 0.f;
			}
		}
	}
	if(m_bDMG) return;
	if(m_bATK) return;
	if(m_eAniStat == Boss_PassOut
		|| m_eAniStat == Boss_Down)
		return;
	
	_vec3 vPlayerWorldCenPos(0.f, 0.f, 0.f);
	D3DXVec3TransformCoord(&vPlayerWorldCenPos, &m_pPlayerOBBInfo->vCenterPos, &m_pPlayerTransCom->m_matWorld);

	if(Engine::Collision_SphereSphere(vPlayerWorldCenPos, m_fPlayerSphereRad, m_pTransCom->m_vPosition, m_fMySphereRad))
	{
		if(Engine::Collision_OBB(*m_pMyOBBInfo, m_mATKTrans * m_pTransCom->m_matWorld, *m_pPlayerOBBInfo, m_pPlayerTransCom->m_matWorld))
		{
			Compute_Dir();

			int i = rand()%2;
			if(i == 0)
			{
				m_eAniStat = Boss_ATK_01;
				m_pMeshCom->Set_AnimationSet(Boss_ATK_01);

				CSDevice::GetInstance()->SoundPlay(SD_BOSSATTACK1, 0);
				m_fAtkStartTime = 0.f;
			}
			else
			{
				m_eAniStat = Boss_ATK_02;
				m_pMeshCom->Set_AnimationSet(Boss_ATK_02);

				CSDevice::GetInstance()->SoundPlay(SD_BOSSATTACK2, 0);
				m_fAtkStartTime = 0.f;
			}	
			m_bATK = true;
		}
	}	
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss*		pGameObject = new CBoss(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CBoss Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CBoss::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);	

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	D3DXMATRIX matCamera;
	D3DXMatrixInverse(&matCamera, NULL, &matView);
	_vec4 vCameraPos = _vec4(matCamera._41, matCamera._42, matCamera._43, 1.f);

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));

	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	
	m_pEffect->SetVector("g_vLightSpecular", &_vec4((_float*)&pLightInfo->Specular));	

	m_pEffect->SetVector("g_vWorldCamPos", &vCameraPos);

	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);

}

_ulong CBoss::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if(0 == dwRefCnt)
	{	
#ifdef _DEBUG
		if(Engine::Safe_Release(m_pSphereMesh))
			MSG_BOX("SphereMesh Release Failed");

		if(Engine::Safe_Release(m_pSphere2Mesh))
			MSG_BOX("Sphere2Mesh Release Failed");
#endif
		delete this;		 
	}
	return dwRefCnt;
}


void CBoss::Set_Data(void)
{
	//setting OBB
	m_pMyOBBInfo = m_pMeshCom->Get_OBBInfo();

	Engine::CMesh*	 pPlayerMeshcom = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
	m_pPlayerOBBInfo = pPlayerMeshcom->Get_OBBInfo();

	Engine::CMesh*	 pSwordMeshCom = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Sword");
	m_pSwordOBBInfo = pSwordMeshCom->Get_OBBInfo();

	Engine::CMesh*	 pShieldMeshCom = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Shield");
	m_pShieldOBBInfo = pShieldMeshCom->Get_OBBInfo();


	//setting TransCom
	
	m_pPlayerTransCom	= (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player"); 		
	m_pShieldTransCom	= (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Shield"); 		
	m_pSwordTransCom	= (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Sword"); 		

	//setting Hand Matrix
	const _matrix*	pHandMatrix = m_pMeshCom->Get_FrameMatrix("Bip01-R-Finger2");
	m_pmatRForearm = pHandMatrix;
	pHandMatrix = m_pMeshCom->Get_FrameMatrix("Bip01-L-Finger2");
	m_pmatLForearm = pHandMatrix;

	m_pFin1Matrix = m_pMeshCom->Get_FrameMatrix("Bip01-R-Finger0");
	memcpy(&m_FinLocalpos1, &m_pFin1Matrix->m[3][0], sizeof(_vec3));
	m_pFin2Matrix = m_pMeshCom->Get_FrameMatrix("Bip01-R-Finger3");
	memcpy(&m_FinLocalpos2, &m_pFin2Matrix->m[3][0], sizeof(_vec3));

	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

	D3DXVec3TransformCoord(&m_Finger1Pos, &m_FinLocalpos1, &(matScale * *m_pFin1Matrix));
	D3DXVec3TransformCoord(&m_Finger2Pos, &m_FinLocalpos2, &(matScale * *m_pFin2Matrix));

	m_pFin1Matrix2 = m_pMeshCom->Get_FrameMatrix("Bip01-L-Finger0");
	memcpy(&m_FinLocalpos12, &m_pFin1Matrix2->m[3][0], sizeof(_vec3));
	m_pFin2Matrix2 = m_pMeshCom->Get_FrameMatrix("Bip01-L-Finger3");
	memcpy(&m_FinLocalpos22, &m_pFin2Matrix2->m[3][0], sizeof(_vec3));

	D3DXVec3TransformCoord(&m_Finger1Pos2, &m_FinLocalpos12, &(*m_pFin1Matrix2));
	D3DXVec3TransformCoord(&m_Finger2Pos2, &m_FinLocalpos22, &(*m_pFin2Matrix2));
	


	//setting MinMax
	m_pMeshCom->Get_pMinMax(m_pvMyMin, m_pvMyMax);
	pPlayerMeshcom->Get_pMinMax(m_pvPlayerMin, m_pvPlayerMax);
	pSwordMeshCom->Get_pMinMax(m_pvSwordMin, m_pvSwordMax);
	pShieldMeshCom->Get_pMinMax(m_pvShieldMin, m_pvShieldMax);

	m_fSwordSphereRad	= D3DXVec3Length(&( *m_pvSwordMax - *m_pvSwordMin)) * m_pPlayerTransCom->m_vScale.x;
	m_fShieldSphereRad	= D3DXVec3Length(&( *m_pvShieldMax - *m_pvShieldMin)) * m_pPlayerTransCom->m_vScale.x;
}

/* 이펙트 생성부분 *********************************/
void CBoss::Create_Blood(_vec3 vPos)
{
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"Blood");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CBloodBlack::Create(m_pGraphicDev, vPos);
	Engine::CGameObject* pGameObject2 = CBlood::Create(m_pGraphicDev, m_pTransCom->m_vPosition + m_pTransCom->m_vDir);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		NewObjectList.push_back(pGameObject2);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Blood", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Blood", pGameObject);
		m_pGameLogicLayer->Ready_Object(L"Blood", pGameObject2);
	}
}

void CBoss::Create_HitShock(void)
{
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"Shock");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	//Engine::CGameObject* pGameObject;
	Engine::CGameObject* pGameObject = CShock::Create(m_pGraphicDev, m_pTransCom->m_vPosition);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Shock", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Shock", pGameObject);
	}
}

void CBoss::Create_MagicCircle(_int iIdx)
{
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"Shock");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	//Engine::CGameObject* pGameObject;
	Engine::CGameObject* pGameObject = CMagicCircle::Create(m_pGraphicDev, m_pTransCom->m_vPosition, iIdx);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Shock", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Shock", pGameObject);
	}

}

void CBoss::Create_Smoke(_vec3 vPos)
{
	list<Engine::CGameObject*>* m_pSmokeList = m_pGameLogicLayer->Find_ObjectList(L"Smoke");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CSmokeSphere::Create(m_pGraphicDev, vPos);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pSmokeList)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Smoke", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Smoke", pGameObject);
	}
}

void CBoss::Create_IceUp(_vec3 vPos)
{
	list<Engine::CGameObject*>* m_pSmokeList = m_pGameLogicLayer->Find_ObjectList(L"IceUP");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CIceUp::Create(m_pGraphicDev, vPos);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pSmokeList)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"IceUP", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"IceUP", pGameObject);
	}
}

_vec3 CBoss::Get_Pos()
{
	return m_pTransCom->m_vPosition;
}	

void CBoss::Create_Wang()
{
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"Wang");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CWang::Create(m_pGraphicDev, m_pTransCom->m_vPosition);
	//Engine::CGameObject* pGameObject = CBlood::CreateScale(m_pGraphicDev, m_pTransCom->m_vPosition, 1.f);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		
		//NewObjectList.push_back(pGameObject);
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Wang", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Wang", pGameObject);
		//m_pGameLogicLayer->Ready_Object(L"Blood", pGameObject2);
	}
}

void CBoss::Create_Swirl()
{
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"Swirl");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CSwirl::Create(m_pGraphicDev, m_pTransCom->m_vPosition, 0);
	Engine::CGameObject* pGameObject2 = CSwirl::Create(m_pGraphicDev, m_pTransCom->m_vPosition, 1);

	if(NULL == pGameObject)	return;	
	if(NULL == pGameObject2)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		NewObjectList.push_back(pGameObject2);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Swirl", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Swirl", pGameObject);
		m_pGameLogicLayer->Ready_Object(L"Swirl", pGameObject2);
	}
}

void CBoss::Create_SwordCircle(_vec3 vPos)
{	
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"SwordCircle");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();


	Engine::CGameObject* pGameObject = CSwordCircle::Create(m_pGraphicDev, vPos);
	
	if(NULL == pGameObject)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		

		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"SwordCircle", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"SwordCircle", pGameObject);
	}
}


