#include "stdafx.h"
#include "LesserGiant.h"
#include "SCOriTown.h"

CLesserGiant::CLesserGiant(string name, tag t, CGameObject* pTarget)
	: CGameObject(name, t)
{
	ResetHPValues(2000.f, 2000.f);//레서 자이언트는 hp가 무려 2천이에오
	m_pTempPlayer = pTarget;
	m_f3Diraction = XMFLOAT3(0.f, 0.f, 0.f);
}

CLesserGiant::~CLesserGiant()
{
}

bool CLesserGiant::GetDemaged(int iDemege){
	if (m_pAnimater->GetCurAnimationIndex() == BOSS1_ANI_DIE) return false;
	//뭔가 이팩트가 있으면 좋겠음 ㅠ

#ifdef NO_SERVER
	CGameObject::GetDemaged(iDemege);//내 hp 날리고!
#else

#endif
	if (m_iCurHP == 0) {
		if (m_pAnimater->SetCurAnimationIndex(BOSS1_ANI_DYING)) {
			CSoundManager::Play_3Dsound("boss1_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
		}
	}
	//BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	//NETWORKMGR->WritePacket(PT_BOSS_FREQUENCY_MOVE_CS, Packet, WRITE_PT_BOSS_FREQUENCY_MOVE_CS(Packet, m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_nAnimNum));
	return true;
}

void CLesserGiant::GetSkilled(int nSkill)
{
	int slot_id = 5;
}

void CLesserGiant::Animate(float fTimeElapsed)
{
	CGameObject::MappingRimLight(fTimeElapsed);

	if(false == m_bAttack && false == m_bSkill)
		m_fAccSkillTime += fTimeElapsed;
#ifdef NO_SERVER
	UpdatePattern(fTimeElapsed);
	Move(XMVector3Normalize(XMLoadFloat3(&m_f3Diraction)), (m_fSpeed)* fTimeElapsed);

	if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());
	ActionMoveProc();

	//모든 컴포넌트를 돌면서 Update실행
	for (auto i : m_mapComponents) {
		i.second->Update(fTimeElapsed);
	}

#else
	if (NETWORKMGR->GetSLOT_ID() == 0) {
		UpdatePattern(fTimeElapsed);
		Move(XMVector3Normalize(XMLoadFloat3(&m_f3Diraction)), (m_fSpeed)* fTimeElapsed);

	}


	else {
		BOSS_FREQUENCY_DATA data = NETWORKMGR->GetBossInfo();
		DEBUGER->AddGameText(25, 10, 350, YT_Color(0, 0, 200), L"%f %f %f %f %f", data.fPosX, data.fPosY, data.fPosZ, data.fAngleY, data.iAnimNum);

		SetPositionServer(XMVectorSet(data.fPosX, data.fPosY, data.fPosZ, 1.0f));
		SetRotation(XMMatrixRotationY(data.fAngleY));

		m_nAnimNum = data.iAnimNum;
		if (m_pAnimater->SetCurAnimationIndex(data.iAnimNum)) {
			switch (data.iAnimNum) {
			case BOSS1_ANI_SKILL1:
				CSoundManager::Play_3Dsound("boss1_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				ResetCollisionValue(XMFLOAT3(0, 1, 5), 1, 1.7, 5);
				break;
			case BOSS1_ANI_SKILL2:
				CSoundManager::Play_3Dsound("boss1_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				ResetCollisionValue(XMFLOAT3(0, 1, 5), 4.1, 4.6, 5);
				break;
			case BOSS1_ANI_SKILL3:
				CSoundManager::Play_3Dsound("boss1_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				ResetCollisionValue(XMFLOAT3(0, 1, 5), 3.4, 4.5, 5);
				break;
			case BOSS1_ANI_SKILL4:
				CSoundManager::Play_3Dsound("boss1_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				ResetCollisionValue(XMFLOAT3(0, 0, 0), 2, 7, 7);
				break;
			case BOSS1_ANI_DYING:
				CSoundManager::Play_3Dsound("boss1_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				break;
			case BOSS1_ANI_DIE:
				CSoundManager::Stop_bgm("bgm_firsttown_battle");
				break;
			default:
				break;
			}
		}
	}
	if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());
	ActionMoveProc();

	//모든 컴포넌트를 돌면서 Update실행
	for (auto i : m_mapComponents) {
		i.second->Update(fTimeElapsed);
	}

	if (NETWORKMGR->GetSLOT_ID() == 0) {


		BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
		NETWORKMGR->WritePacket(PT_BOSS_FREQUENCY_MOVE_CS, Packet, WRITE_PT_BOSS_FREQUENCY_MOVE_CS(Packet, m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex()));
	}


	

#endif
	DEBUGER->RegistCoordinateSys(GetWorldMtx());
}

void CLesserGiant::RegistToContainer()
{
	CGameObject::RegistToContainer();
}

void CLesserGiant::TransferCollisioinData(int target_slot_id, int skillnum) {
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(), 5, target_slot_id, 6, skillnum));


}
void CLesserGiant::PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime)
{

	if (m_pAnimater->GetCurAnimationIndex() == BOSS1_ANI_DYING || m_pAnimater->GetCurAnimationIndex() == BOSS1_ANI_DIE) {
		if (m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_pAnimater->SetCurAnimationIndex(BOSS1_ANI_DIE);
		}
		m_bCollision = true;
		return;
	}

	for (auto pArrow : mlpObject[utag::UTAG_ARROW]) {
		//내가쏜 화살만 데미지를 입음
		if (false == pArrow->GetActive()) continue;
		if (true == IsCollision(pArrow))
		{
#ifdef NO_SERVER
			GetDemaged(100.f);
			SetRimLight();
			pArrow->DisappearSkill();
#else
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
			NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(), NETWORKMGR->GetSLOT_ID(), 5, NETWORKMGR->GetServerPlayerInfos()[NETWORKMGR->GetSLOT_ID()].CHARACTER, 9));
			SetRimLight();
			pArrow->DisappearSkill();
#endif
			
		
			break;
		}
	}
	for (auto pArrow : mlpObject[utag::UTAG_OTHERPLAYER_ARROW]) {

		if (false == pArrow->GetActive()) continue;
		if (true == IsCollision(pArrow))
		{
			pArrow->DisappearSkill();
			SetRimLight();
			break;
		}
	}
	if (false == m_bCollision) {
		for (auto pPlayer : mlpObject[utag::UTAG_PLAYER]) {
			//pPlayer->Demaged();
			switch (m_nAnimNum) {
			case BOSS1_ANI_SKILL1:
				if (SkillCollision(pPlayer)) {
#ifdef NO_SERVER
					pPlayer->GetDemaged(m_iAttack);//100
#else
					TransferCollisioinData(pPlayer->GetSlotID(), 1);
					pPlayer->SetRimLight();
#endif
					m_bCollision = true;
				}
				break;
			case BOSS1_ANI_SKILL2:
				if (SkillCollision(pPlayer)) {
#ifdef NO_SERVER
					pPlayer->GetDemaged(m_iAttack * 2);//200
#else
					TransferCollisioinData(pPlayer->GetSlotID(), 2);
					pPlayer->SetRimLight();
#endif
					m_bCollision = true;
				}
				break;
			case BOSS1_ANI_SKILL3:
				if (SkillCollision(pPlayer)) {
#ifdef NO_SERVER
					pPlayer->GetDemaged(m_iAttack * 2);//200
#else
					TransferCollisioinData(pPlayer->GetSlotID(), 3);
					pPlayer->SetRimLight();
#endif
					m_bCollision = true;
				}
				break;
			case BOSS1_ANI_SKILL4:
				if (SkillCollision(pPlayer)) {
#ifdef NO_SERVER
					pPlayer->GetDemaged(m_iAttack * 1.5);//150
#else
					TransferCollisioinData(pPlayer->GetSlotID(), 4);
					pPlayer->SetRimLight();
#endif
					m_bCollision = true;
				}
				break;
			default:
				break;
			}
		}
	}
	m_fCollisionTime += fDeltaTime; 
	m_fAnimTime += fDeltaTime;
	if (m_fCollisionTime > 2.f) {
		m_fCollisionTime = 0.f;
		m_bCollision = false;//2초에 한번씩 다시 맞게 한다.
	}
}

void CLesserGiant::UpdatePattern(float fTimeElapsed)
{
	//set target
	CGameObject* pTarget = nullptr;
	int nPawn = NETWORKMGR->GetServerPlayerInfos().size();
	CSCOriTown* pScene = (CSCOriTown*)SCENEMGR->GetPresentScene();
	for (int i = 0; i < nPawn; ++i) {
		if (pScene->m_ppPawn[i]->GetAnimater()->GetCurAnimationIndex() != ANIM_DEADBODY) {
			//죽은게 아니라면 보이는 가장 첫번째 녀석을 쫓는다.
			pTarget = pScene->m_ppPawn[i];
		}
	}
	m_pTempPlayer = pTarget;
	if (nullptr == m_pTempPlayer) {
		//타겟이 없으면 아이들을 무한반복한다.
		m_pAnimater->SetCurAnimationIndex(BOSS1_ANI_IDLE);
		m_fSpeed = 0;//움직이지도 않는다.
		return;
	}
	//set target

	if (m_bFirstAction) return;//첫번째 액션중이면 return
	m_f3Diraction = XMFLOAT3(0.f, 0.f, 0.f);
	if (m_pAnimater->GetCurAnimationIndex() == BOSS1_ANI_DYING || m_pAnimater->GetCurAnimationIndex() == BOSS1_ANI_DIE) {
		if (m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_pAnimater->SetCurAnimationIndex(BOSS1_ANI_DIE);
		}
		m_bCollision = true;
		return;
	}

	float fDistance = 0.f;
	m_fSpeed = 5.f;
	if (nullptr != m_pTempPlayer)
	{
		XMFLOAT3 xmf3PlayerPos, xmf3LasserPos;
		XMStoreFloat3(&xmf3PlayerPos, m_pTempPlayer->GetPosition());
		XMStoreFloat3(&xmf3LasserPos, XMLoadFloat3(&m_xmf3Position));
		xmf3PlayerPos.y = 0.f;		xmf3LasserPos.y = 0.f;

		XMStoreFloat3(&m_f3Diraction, XMLoadFloat3(&xmf3PlayerPos) - XMLoadFloat3(&xmf3LasserPos));
		XMFLOAT3 xmf3Distance;
		XMStoreFloat3(&xmf3Distance, XMVector3Length(XMLoadFloat3(&m_f3Diraction)));
		XMStoreFloat3(&m_f3Diraction, XMVector4Normalize(XMLoadFloat3(&m_f3Diraction)));

		XMFLOAT3 xmf3DotValue;
		XMStoreFloat3(&xmf3DotValue, XMVector3Dot(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMLoadFloat3(&m_f3Diraction)));
		float fDirAngle = acosf(xmf3DotValue.x);
		m_fAngleY = fDirAngle;
		if (m_f3Diraction.x < 0.f) fDirAngle = XM_PI * 2.f - fDirAngle;

		if (true == m_bSkill)
		{
			if( BOSS1_ANI_SKILL2 == m_nAnimNum)
				m_f3Diraction = XMFLOAT3(0.f, 0.f, 0.f);
			else if (BOSS1_ANI_SKILL3 == m_nAnimNum)
			{
				m_fSpeed = 0.f;
				m_fSk2Time += fTimeElapsed;
				if (m_fSk2Time < 3.3f &&m_fSk2Time > 2.3f)
				{
					XMStoreFloat3(&m_f3Diraction, XMLoadFloat3(&m_xmf3Sk2Dir));
					m_fSpeed = m_fSk2Speed * 0.9f;
				}
				else if(m_fSk2Time >= 3.3f)
				{
					m_fSpeed = 0.f;
				}
			}
			else if (BOSS1_ANI_SKILL4 == m_nAnimNum)
			{
				SetRotation(DirectX::XMMatrixRotationY(fDirAngle));
				m_fSpeed = 0.f;
				if (m_fAnimTime < 7.0f &&m_fAnimTime > 1.f)
				{
					//XMStoreFloat3(&m_f3Diraction, XMLoadFloat3(&m_xmf3Sk2Dir));
					m_fSpeed = m_fSk2Speed * 0.2f;
				}
				//else if (m_fAnimTime >= 7.3f)
				//{
				//	m_fSpeed = 0.f;
				//}
			}

			if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
			{
				m_nAnimNum = BOSS1_ANI_WALK;
				m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
				ResetCollisionValue(XMFLOAT3(0, 0, 0), 0, 0, 0);
				m_bSkill = false;
				m_fSk2Time = 0.f;
			}
			return;
		}
		
		if (m_fAccSkillTime > 2.f)
		{
			m_fAccSkillTime = 0.f;
			m_bSkill = true;
			SetRotation(DirectX::XMMatrixRotationY(fDirAngle));

			switch (m_nPatternNum) {
			case 0:
				m_nAnimNum = BOSS1_ANI_SKILL2;
				if (m_pAnimater->SetCurAnimationIndex(m_nAnimNum)) {
					CSoundManager::Play_3Dsound("boss1_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
					ResetCollisionValue(XMFLOAT3(0, 1, 5), 4.1, 4.6, 5);
				}
				break;
			case 1:
				m_nAnimNum = BOSS1_ANI_SKILL3;
				if (m_pAnimater->SetCurAnimationIndex(m_nAnimNum)) {
					CSoundManager::Play_3Dsound("boss1_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
					ResetCollisionValue(XMFLOAT3(0, 1, 5), 3.4, 4.5, 5);
				}
				
				m_fSk2Speed = xmf3Distance.x;
				XMStoreFloat3(&m_xmf3Sk2Dir, XMLoadFloat3(&m_f3Diraction));
				break;

			case 2:
				m_nAnimNum = BOSS1_ANI_SKILL4;

				m_fSk2Speed = xmf3Distance.x;
				//XMStoreFloat3(&m_xmf3Sk2Dir, XMLoadFloat3(&m_f3Diraction));
				if (m_pAnimater->SetCurAnimationIndex(m_nAnimNum)) {
					CSoundManager::Play_3Dsound("boss1_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
					ResetCollisionValue(XMFLOAT3(0, 0, 0), 2, 7, 7);
				}
				break;
			}			
			++m_nPatternNum;
			if (m_nPatternNum == 3) m_nPatternNum = 0;
		}
		
		// 거리가 10 이하면 스킬1 시전
		if (xmf3Distance.x < 7.f){
			if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()){		// 스킬1 할때마다 위치 찾기.
				SetRotation(DirectX::XMMatrixRotationY(fDirAngle));
				ResetCollisionValue(XMFLOAT3(0, 1, 5), 1, 1.7, 5);
			}
			m_f3Diraction = XMFLOAT3(0.f, 0.f, 0.f);
			m_nAnimNum = BOSS1_ANI_SKILL1;
			if (m_pAnimater->SetCurAnimationIndex(m_nAnimNum)) {
				CSoundManager::Play_3Dsound("boss1_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				ResetCollisionValue(XMFLOAT3(0, 1, 5), 1, 1.7, 5);
			}
			m_bAttack = true;
		}
		else {
			if (true == m_bAttack)
			{
				m_f3Diraction = XMFLOAT3(0.f, 0.f, 0.f);
				if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
				{
					m_nAnimNum = BOSS1_ANI_WALK;
					m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
					m_bAttack = false;
				}
				return;
			}
			m_fAngleY = fDirAngle;
			SetRotation(DirectX::XMMatrixRotationY(fDirAngle));
		}
	}

	//if (true == m_bAttack && true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
	//	m_nAnimNum = BOSS1_ANI_WALK;
	//	m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	//	m_bAttack = false;
	//}
}
