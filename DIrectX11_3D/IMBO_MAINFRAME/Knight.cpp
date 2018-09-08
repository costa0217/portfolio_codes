#include "stdafx.h"
#include "Knight.h"

bool CKnight::Begin()
{
	CGameObject::Begin();
	return false;
}

void CKnight::Animate(float fTimeElapsed)
{
	m_fTime = fTimeElapsed;

	CGameObject::MappingRimLight(fTimeElapsed);

	if (true == m_bSprit) {
		if (false == m_bDamaged)
			KeyInput(fTimeElapsed); //KeyInput(fTimeElapsed);
	}
	else	GetServerData(fTimeElapsed);

	//점프
	if (true == m_bJump)	Jumping(fTimeElapsed);

	// 애니메이션 업데이트함수
	if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());


	UpdateSkill();
	ActionMoveProc();

	//모든 컴포넌트를 돌면서 Update실행
	for (auto i : m_mapComponents) {
		i.second->Update(fTimeElapsed);
	}

	DEBUGER->RegistCoordinateSys(GetWorldMtx());

	SetWeapon();
	DEBUGER->AddText(30, 100, 600, YT_Color(255.f, 255.f, 255.f), L"animation %d", m_nAnimNum);
}

bool CKnight::End()
{
	if (m_pWeapon) {
		delete m_pWeapon;
		m_pWeapon = nullptr;
	}

	if (m_pLeftWeapon) {
		delete m_pLeftWeapon;
		m_pLeftWeapon = nullptr;
	}
	if (m_pRightWeapon) {
		delete m_pRightWeapon;
		m_pRightWeapon = nullptr;
	}
	return true;
}

void CKnight::KeyInput(float fDeltaTime)
{
	if (m_pAnimater->GetCurAnimationIndex() == KNIGHT_ANIM_DIE || m_pAnimater->GetCurAnimationIndex() == KNIGHT_ANIM_DEADBODY) {
		m_nAnimNum = KNIGHT_ANIM_DIE;
		if (m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_nAnimNum = KNIGHT_ANIM_DEADBODY;
			m_pAnimater->SetCurAnimationIndex(KNIGHT_ANIM_DEADBODY);
		}
#ifdef NO_SERVER

#else
		m_fTranslateTime += fDeltaTime;
		if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
			m_fTranslateTime = 0;
			PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex());
		}
#endif	
		m_bCollision = true;
		return;
	}
	if (m_bStay) {
		m_fTranslateTime += fDeltaTime;
		if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
			m_fTranslateTime = 0;
			PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex());
		}
		return;
	}
	DWORD dwDirection = 0;
	m_xmvShift = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);

#ifdef NO_SERVER

#else

#endif
	if (GetAsyncKeyState(VK_SHIFT)){m_fSpeed = 50.f;}
	else{	m_fSpeed = 10.f;}

	if (m_bSkill)	m_pCamera->AttackStartZoomInOut(true);
	else			m_pCamera->AttackStartZoomInOut(false);
	// 스킬 및 공격
	if (false == m_bJump && false == m_bSkill)
	{
		if (INPUTMGR->MouseLeftDown()) {					// 기본공격 ----------------------
			m_bSkill = true;
			m_nAnimNum = KNIGHT_ANIM_ATTACK;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			
			//기본공격 충돌체 생성
			CSoundManager::Play_3Dsound("knight_attack1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			ResetCollisionValue(XMFLOAT3(0, 0, 3), 0.f, 0.8f, 2.f);
			INPUTMGR->SetMouseLeft(false);
			RENDERER->SetRadialBlurTime(true, 0.3f);
		}
		else if (INPUTMGR->KeyDown(VK_1)) {				// 스킬 1 ------------------------
			m_fSkill1EndTime = m_fSkillTime + 10.f;// skill1는 10초
			m_bSkill = true;
			m_nAnimNum = KNIGHT_ANIM_SKILL1_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			m_pCamera->CameraStartVibration(1.5f, 10.f);

			m_pWeaponTrail->PlayOnKnightSkill1();
			CSoundManager::Play_3Dsound("knight_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk1_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
		}
		else if (INPUTMGR->KeyDown(VK_2)) {				// 스킬 2 ------------------------
			m_fSkill2EndTime = m_fSkillTime + 5.f;// skill2는 5초
			m_bSkill = true;
			m_nAnimNum = KNIGHT_ANIM_SKILL2_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			CSoundManager::Play_3Dsound("knight_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk2_Shield", this);
		}
		else if (INPUTMGR->KeyDown(VK_3)) {				// 스킬 3 ------------------------
			m_bSkill = true;
			m_nAnimNum = KNIGHT_ANIM_SKILL3_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			CSoundManager::Play_3Dsound("knight_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk3_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
		}
		else if (INPUTMGR->KeyDown(VK_4)) {				// 스킬 4 ------------------------
			m_fSkill4EndTime = m_fSkillTime + 2.f;// skill4는 2초
			m_bSkill = true;
			m_nAnimNum = KNIGHT_ANIM_SKILL4_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			CSoundManager::Play_3Dsound("knight_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk4_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
		}
	}

	// 스킬시 이동 점프X
	if (true == m_bSkill) {
#ifdef NO_SERVER

#else
		m_fTranslateTime += fDeltaTime;
		if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
			m_fTranslateTime = 0;
			PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_nAnimNum);
		}
#endif
		//60fps로 업데이트, 네트워크 갱신

		return;
	}

	// 마우스 우클릭회전
	if (true == INPUTMGR->MouseRightUp() && abs(m_pCamera->m_cxDelta + m_pCamera->m_cyDelta) > 1.f) {
		m_bIdle = false;
	}
	if (false == m_bIdle) {
		m_fAngleY = m_pCamera->m_fCurrentAngle[ANGLE_Y] + XM_PI;
		m_fAngleX = XM_PI * 2.f - m_pCamera->m_fCurrentAngle[ANGLE_X];

		m_xmatrixRotate = XMMatrixRotationX(m_fAngleX) *  XMMatrixRotationY(m_fAngleY);
		SetRotation(XMMatrixRotationY(m_fAngleY));
	}

	// 이동
	if (INPUTMGR->OnlyKeyBoardDown(VK_W))		dwDirection |= DIR_FORWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_S))		dwDirection |= DIR_BACKWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_A))		dwDirection |= DIR_LEFT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_D))		dwDirection |= DIR_RIGHT;
	// 점프
	if (INPUTMGR->KeyBoardDown(VK_SPACE_))		m_bJump = true;

	if (true == INPUTMGR->MouseRightDown()) {
		m_bIdle = true;
	}

	SetupAnimation(dwDirection);
	//float fSpeed = 20.f;
	if (dwDirection) {
		float fSpdX = 1.f;
		if (dwDirection & DIR_RIGHT)		m_xmvShift += GetRight();
		if (dwDirection & DIR_LEFT)			m_xmvShift -= GetRight();
		if (dwDirection & DIR_FORWARD) { m_xmvShift += GetLook(); fSpdX = 1.f; }
		if (dwDirection & DIR_BACKWARD) { m_xmvShift -= GetLook(); fSpdX = 0.8f; }

		Move(XMVector3Normalize(m_xmvShift), (m_fSpeed * fSpdX) * fDeltaTime);

		m_bIdle = false;

		//walk effect
		if (!m_bJump) {
			m_fWalkEffectTime += fDeltaTime;
			if (m_fWalkEffectTime > 0.15f) {
				CEffectMgr::GetInstance()->Play_Effect(L"walk_dust", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 0.5f, m_xmf3Position.z, 1.f),
					XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_fWalkEffectTime = 0.f;
			}
		}
	}
	else {
		if (false == m_bJump) {
			if (KNIGHT_ANIM_JUMP_END != m_nAnimNum) {
				m_nAnimNum = ANIM_IDLE;
				m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			}
		}
	}
	////점프
	//if (true == m_bJump)	Jumping(fDeltaTime);


#ifdef NO_SERVER
	return;
#endif 
	//60fps로 업데이트, 네트워크 갱신
	m_fTranslateTime += fDeltaTime;
	if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
		m_fTranslateTime = 0;
		PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_nAnimNum);
	}
}

void CKnight::PushServerData(float x, float y, float z, float fAngleY, int nAnimNum){
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	NETWORKMGR->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, x, y, z, fAngleY, m_pAnimater->GetCurAnimationIndex()));
}

void CKnight::GetServerData(float fTimeElapsed)
{

#ifdef NO_SERVER
	return;
#endif
	//////
	PLAYR_FREQUENCY_DATA data = NETWORKMGR->GetPlayerFrequencyData(m_SLOT_ID);
	m_xmf3Position.x = data.fPosX;
	m_xmf3Position.y = data.fPosY;
	m_xmf3Position.z = data.fPosZ;

	m_fAngleY = data.fAngleY;
	//DWORD dwDirection = data.dwDirection;
	if (m_nAnimNum != data.iAnimNum)
		m_nAnimNum = data.iAnimNum;

	//////

	//if (m_bJump == true && data.bJump == false) {
	//	m_nAnimNum = ANIM_JUMP_END;
	//	m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	//}
	//m_bJump = data.bJump;

	SetPositionServer(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.0f));
	SetRotation(XMMatrixRotationY(m_fAngleY));

	if (m_pAnimater->SetCurAnimationIndex(m_nAnimNum)) {
		switch (m_nAnimNum) {
		case KNIGHT_ANIM_ATTACK:
			CSoundManager::Play_3Dsound("knight_attack1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			break;
		case KNIGHT_ANIM_ATTACK2:
			CSoundManager::Play_3Dsound("knight_attack2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			break;
		case KNIGHT_ANIM_ATTACK3:
			CSoundManager::Play_3Dsound("knight_attack3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			break;
		case KNIGHT_ANIM_SKILL1_FIRE:
			CSoundManager::Play_3Dsound("knight_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk1_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

			m_pWeaponTrail->PlayOnKnightSkill1();
			break;
		case KNIGHT_ANIM_SKILL2_FIRE:
			CSoundManager::Play_3Dsound("knight_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk2_Shield", this);
			break;
		case KNIGHT_ANIM_SKILL3_FIRE:
			CSoundManager::Play_3Dsound("knight_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk3_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case KNIGHT_ANIM_SKILL4_FIRE:
			CSoundManager::Play_3Dsound("knight_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk4_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case KNIGHT_ANIM_HIT_F:
			CSoundManager::Play_3Dsound("knight_hurt", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"TestBlood", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case KNIGHT_ANIM_DIE:
			CSoundManager::Play_3Dsound("knight_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			break;
		default:
			break;
		}
	}

	// 공격
	//if (m_bSkill == false && m_bJump == false && bAttack == true && m_nAnimNum != ANIM_ATTACK) {
	//	CEffectMgr::GetInstance()->Play_Effect(L"Test2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
	//		XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f));
	//	m_nAnimNum = ANIM_ATTACK;
	//	m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

	//	size_t iArraySize = m_mapSkill["Arrow"].size();
	//	for (size_t i = 0; i < iArraySize; ++i) {
	//		if (false == m_mapSkill["Arrow"][i]->GetActive()) {
	//			m_mapSkill["Arrow"][i]->SetActive(true);
	//			m_mapSkill["Arrow"][i]->SetPosition(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 3.f, m_xmf3Position.z, 1.f));
	//			m_mapSkill["Arrow"][i]->Rotate(m_xmatrixRotate);
	//			break;
	//		}
	//	}
	//}

	//SetupAnimation(dwDirection);
	//if (dwDirection) {
	//	m_bIdle = false;
	//}
	//else {
	//	if (false == m_bJump) {
	//		if (ANIM_JUMP_END != m_nAnimNum) {
	//			m_nAnimNum = ANIM_IDLE;
	//			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	//		}
	//	}
	//}
}

void CKnight::SetupAnimation(DWORD dwDirection)
{
	if (false == m_bJump)
	{
		if (dwDirection & DIR_FORWARD)		if (m_nAnimNum != KNIGHT_ANIM_RUN_F) m_nAnimNum = KNIGHT_ANIM_RUN_F;
		if (dwDirection & DIR_BACKWARD)		if (m_nAnimNum != KNIGHT_ANIM_RUN_B) m_nAnimNum = KNIGHT_ANIM_RUN_B;
		if (dwDirection & DIR_LEFT)			if (m_nAnimNum != KNIGHT_ANIM_RUN_L) m_nAnimNum = KNIGHT_ANIM_RUN_L;
		if (dwDirection & DIR_RIGHT)		if (m_nAnimNum != KNIGHT_ANIM_RUN_R) m_nAnimNum = KNIGHT_ANIM_RUN_R;

		if (dwDirection & DIR_FORWARD && dwDirection & DIR_LEFT) {
			if (m_nAnimNum != KNIGHT_ANIM_RUN_FL) m_nAnimNum = KNIGHT_ANIM_RUN_FL;
		}
		else if (dwDirection & DIR_FORWARD && dwDirection & DIR_RIGHT) {
			if (m_nAnimNum != KNIGHT_ANIM_RUN_FR) m_nAnimNum = KNIGHT_ANIM_RUN_FR;
		}
		else if (dwDirection & DIR_BACKWARD && dwDirection & DIR_LEFT) {
			if (m_nAnimNum != KNIGHT_ANIM_RUN_BL) m_nAnimNum = KNIGHT_ANIM_RUN_BL;
		}
		else if (dwDirection & DIR_BACKWARD && dwDirection & DIR_RIGHT) {
			if (m_nAnimNum != KNIGHT_ANIM_RUN_BR) m_nAnimNum = KNIGHT_ANIM_RUN_BR;
		}

		if (0 != dwDirection)
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}
	else {
		if (m_nAnimNum != KNIGHT_ANIM_JUMP_START
			&& m_nAnimNum != KNIGHT_ANIM_JUMP_LOOP)
		{
			m_nAnimNum = KNIGHT_ANIM_JUMP_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}

		if (m_nAnimNum == KNIGHT_ANIM_JUMP_START
			&& true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
		{
			m_nAnimNum = KNIGHT_ANIM_JUMP_LOOP;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
	}
}

void CKnight::Jumping(float fDeltaTime)
{
	m_fJumpTime += fDeltaTime;
	float fJumpValue = 1.3f * m_fJumpTime;
	float fJumpPower = 0.6f;

	m_xmf4x4World._42 += fJumpPower - fJumpValue;
	m_xmf3Position.y += fJumpPower - fJumpValue;

	if (m_xmf4x4World._42 < GetTerrainHeight())
	{
		m_fJumpTime = 0.f;
		m_bJump = false;

		m_xmf4x4World._42 = GetTerrainHeight();
		m_xmf3Position.y = GetTerrainHeight();

		m_nAnimNum = KNIGHT_ANIM_JUMP_END;
		m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}
} 

void CKnight::SetWeapon()
{
	if (m_pLeftWeapon)
	{
		XMMATRIX xmmtxFrame = this->GetAnimater()->GetCurAnimationInfo()->GetCurFrameMtx(this->GetAnimater()->GetLHand());
		XMMATRIX xmmtxFinal = xmmtxFrame * this->GetAnimater()->GetMeshOffsetMtx() * this->GetWorldMtx();

		XMMATRIX xmmtxRotX = XMMatrixRotationX(90.f);
		XMMATRIX xmmtxRotZ = XMMatrixRotationZ(0.f);
		XMMATRIX xmmtxScale = XMMatrixScaling(15.f, 15.f, 15.f);
		XMMATRIX xmmtxWeaponWorld = xmmtxScale* xmmtxRotX * xmmtxRotZ * xmmtxFinal;
		m_pLeftWeapon->SetWorldMtx(xmmtxWeaponWorld);

		//m_pWeaponTrail->Update


		BoundingBox* pWBox = m_pLeftWeapon->GetBBox();
		XMFLOAT4 xmStart, xmEnd;
		XMStoreFloat4(&xmStart, XMVector3TransformCoord(
			XMVectorSet(pWBox->Center.x, pWBox->Center.y - 1.f, pWBox->Center.z, 1.f), xmmtxWeaponWorld));
		XMStoreFloat4(&xmEnd, XMVector3TransformCoord(
			XMVectorSet(pWBox->Center.x, pWBox->Center.y + 2.f , pWBox->Center.z, 1.f), xmmtxWeaponWorld));
		m_pWeaponTrail->SetStartPos(XMLoadFloat4(&xmStart), XMLoadFloat4(&xmEnd));
		m_pWeaponTrail->Update(m_fTime);
		m_pWeaponTrail->SetRenderSwitch(m_bSkill);
		//void GetMainBoundingBox(BoundingBox& out);
	}

	if (m_pRightWeapon)
	{
		XMMATRIX xmmtxFrame = this->GetAnimater()->GetCurAnimationInfo()->GetCurFrameMtx(this->GetAnimater()->GetRHand());
		XMMATRIX xmmtxFinal = xmmtxFrame * this->GetAnimater()->GetMeshOffsetMtx() * this->GetWorldMtx();

		XMMATRIX xmmtxRotX = XMMatrixRotationX(0.f);
		XMMATRIX xmmtxRotZ = XMMatrixRotationZ(0.f);
		XMMATRIX xmmtxScale = XMMatrixScaling(15.f, 15.f, 15.f);

		m_pRightWeapon->SetWorldMtx(xmmtxScale* xmmtxRotX * xmmtxRotZ * xmmtxFinal);
	}
}

void CKnight::UpdateSkill()
{
	if (KNIGHT_ANIM_HIT_F == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_bDamaged = false;
			m_nAnimNum = KNIGHT_ANIM_IDLE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			m_bSkill = false;
		}
	}
	if (KNIGHT_ANIM_ATTACK2 == m_nAnimNum){
	
		if (false == m_bAttak && INPUTMGR->MouseLeftDown())
			m_bAttak = true;
	}
	if (KNIGHT_ANIM_ATTACK == m_nAnimNum)
	{
		if (false == m_bAttak && INPUTMGR->MouseLeftDown())
			m_bAttak = true;
	}
	
	// 점프 끝나면 IDLE로
	if (KNIGHT_ANIM_JUMP_END == m_nAnimNum
		|| m_bSkill == true) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			if (ANIM_HIT_F == m_nAnimNum) m_bDamaged = false;

			if (true == m_bAttak && KNIGHT_ANIM_ATTACK == m_nAnimNum)
			{
				m_nAnimNum = KNIGHT_ANIM_ATTACK2;
				//기본공격 충돌체 생성
				CSoundManager::Play_3Dsound("knight_attack2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				ResetCollisionValue(XMFLOAT3(0, 0, 3), 0.f, 0.8f, 2.f);
				RENDERER->SetRadialBlurTime(true, 0.3f);
			}
			else if (true == m_bAttak && KNIGHT_ANIM_ATTACK2 == m_nAnimNum)
			{
				m_nAnimNum = KNIGHT_ANIM_ATTACK3;
				//기본공격 충돌체 생성
				CSoundManager::Play_3Dsound("knight_attack3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				ResetCollisionValue(XMFLOAT3(0, 0, 3), 0.f, 0.8f, 2.f);
				RENDERER->SetRadialBlurTime(true, 0.4f);
			}

			else
			{
				m_nAnimNum = KNIGHT_ANIM_IDLE;
				m_bSkill = false;
			}
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			m_bAttak = false;
		}
	}
}

CKnight::CKnight(string name, tag t, bool bSprit, CGameObject * pWeapon, INT slot_id)
	: CGameObject(name, t)
	, m_bSprit(bSprit)
	, m_pWeapon(pWeapon)
	, m_SLOT_ID(slot_id)
{
	m_xmf4RimColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

	ResetHPValues(1000, 1000);
	m_fSpeed = 10.f;
	m_pLeftWeapon = new CGameObject("OSW", TAG_DYNAMIC_OBJECT);
	m_pLeftWeapon->Begin();

	//m_pRightWeapon = new CGameObject("OSW", TAG_DYNAMIC_OBJECT);
	//m_pRightWeapon->Begin();

	m_pWeaponTrail = new CTrail(XMVectorSet(0.8f, 0.5f, 0.4f, 1.f), 1, 0.f);
	m_pWeaponTrail->Initialize();
	m_pWeaponTrail->SetTexName(CString("Trail02"));
	m_pWeaponTrail->SetParent(this);
}

CKnight::~CKnight()
{
	if (m_pWeaponTrail)
		delete m_pWeaponTrail;
}

void CKnight::RegistToContainer()
{
	CGameObject::RegistToContainer();
	if (m_pWeapon) m_pWeapon->RegistToContainer();
	if (m_pLeftWeapon) m_pLeftWeapon->RegistToContainer();
	if (m_pRightWeapon) m_pRightWeapon->RegistToContainer();
}


void CKnight::TransferCollisioinData(int target_slot_id, int skillnum) {
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(), NETWORKMGR->GetSLOT_ID(), target_slot_id, 1, skillnum));
}

void CKnight::PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime)
{
	//나이트 스킬 변수
	m_fSkillTime += fDeltaTime;

	m_fCollisionTime += fDeltaTime;
	m_fAnimTime += fDeltaTime;
	if (m_fCollisionTime > 2.f) {
		m_fCollisionTime = 0.f;
		m_bCollision = false;//2초에 한번씩 다시 맞게 한다.
	}

	for (auto pObj : mlpObject[UTAG_NPC]) {
		if (true == IsCollision(pObj))
		{
			if (INPUTMGR->KeyDown(VK_F)) {
				m_pAnimater->SetCurAnimationIndex(ANIM_IDLE);
				m_bStay = true;
			}
			break;
		}
	}
	m_iCurAttack = m_iAttack;
	if (m_fSkill1EndTime > m_fSkillTime) {//skill1이 활성화 중이면 대미지 +
		m_iCurAttack *= 1.5f;
	}
	for (auto pArrow : mlpObject[utag::UTAG_BOSS2]) {
		//내가쏜 화살만 데미지를 입음
		if (false == pArrow->GetActive()) continue;
		if (m_bCollision) break;//내가 맞은 상태면
		if (true == IsCollision(pArrow))
		{
#ifdef NO_SERVER
			GetDemaged(100.f);
			SetRimLight();
#else
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
			NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(), 5, NETWORKMGR->GetSLOT_ID(), 6, 9));
			SetRimLight();
			pArrow->DisappearSkill();
#endif
			m_bCollision = true;

			break;
		}
	}
	for (auto pBoss : mlpObject[utag::UTAG_BOSS1]) {
	//for (auto pBoss : mlpObject[utag::UTAG_NPC]) {
		switch (m_nAnimNum) {
		case KNIGHT_ANIM_ATTACK:
			if (SkillCollision(pBoss)) {//char
#ifdef NO_SERVER
				pBoss->GetDemaged(m_iCurAttack);
#else
				TransferCollisioinData(5, 1);
				pBoss->SetRimLight();
#endif
				m_bCollision = true;
			}
			break;
		case KNIGHT_ANIM_ATTACK2:
			if (SkillCollision(pBoss)) {//boss
#ifdef NO_SERVER
				pBoss->GetDemaged(m_iCurAttack);
#else
				TransferCollisioinData(5, 2);
				pBoss->SetRimLight();
#endif
				m_bCollision = true;
			}
			break;
		case KNIGHT_ANIM_ATTACK3:
			if (SkillCollision(pBoss)) {
#ifdef NO_SERVER
				pBoss->GetDemaged(m_iCurAttack);
#else
				TransferCollisioinData(5, 3);
				pBoss->SetRimLight();
#endif
				m_bCollision = true;
			}
			break;
		default:
			break;
		}
	}
}

bool CKnight::GetDemaged(int iDemage) {
	if (m_pAnimater->GetCurAnimationIndex() == KNIGHT_ANIM_DIE || m_pAnimater->GetCurAnimationIndex() == KNIGHT_ANIM_DEADBODY) {
		m_nAnimNum = m_pAnimater->GetCurAnimationIndex();
		m_bDamaged = false;
		return false;//죽고있으면 충돌처리 하지 않음
	}

	if (m_fSkillTime < m_fSkill4EndTime) {// skill 4
		return false;//무적
	}
	if (m_fSkillTime < m_fSkill2EndTime) {// skill 2
		iDemage *= 0.5f;//대미지 절반으로 감소
	}

	m_bDamaged = true;
	/*CEffectMgr::GetInstance()->Play_Effect(L"TestBlood", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
		XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));*/

	m_nAnimNum = KNIGHT_ANIM_HIT_F;
	CSoundManager::Play_3Dsound("knight_hurt", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
	m_pAnimater->SetCurAnimationIndex(m_nAnimNum);



#ifdef NO_SERVER
	CGameObject::GetDemaged(iDemage);//내 hp 날리고!
#else

#endif

	if (m_iCurHP <= 0) {
		m_nAnimNum = KNIGHT_ANIM_DIE;
		CSoundManager::Play_3Dsound("knight_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
		m_pAnimater->SetCurAnimationIndex(KNIGHT_ANIM_DIE);
	}
#ifdef NO_SERVER
#else

	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	NETWORKMGR->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex()));
#endif


	return true;
}

void CKnight::GetSkilled(int nSkill)
{
	int slot_id = m_SLOT_ID;
}
