#include "stdafx.h"
#include "SCAldenard.h"

#include "ElfSkillArrow.h"
#include "LesserGiant.h"
#include "HpBar.h"
#include "ImageUI.h"

#include "Ranger.h"
#include "Knight.h"
#include "Dementor.h"
#include "Sister.h"
#include "Wizard.h"
#include "Bard.h"




bool CSCAldenard::Begin()
{
	CSoundManager::Play_bgm("bgm_aldenard");
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();
	ReadMapData();
	LoadSkillObjects();
	CreateUI();

	int nPawn = NETWORKMGR->GetServerPlayerInfos().size();//pawn의 전체 수

	m_ppPawn = new CGameObject*[nPawn];
	for (int i = 0; i < nPawn; ++i) {
		bool bSoul = (NETWORKMGR->GetSLOT_ID() == i);//slot_id랑 i랑 같으면 컨트롤 가능 객체
		INT CHARACTER = NETWORKMGR->GetServerPlayerInfo(i).CHARACTER;
		//pawn 제작
		CGameObject* pWeapon = nullptr;
		switch (CHARACTER) {
		case 0:
			//궁수
			pWeapon = new CGameObject("Bow1", TAG_DYNAMIC_OBJECT);
			pWeapon->Begin();
			m_ppPawn[i] = new CRanger("Elf01F", TAG_DYNAMIC_OBJECT, bSoul, pWeapon, i);
			m_ppPawn[i]->Begin();
			m_ppPawn[i]->SetPosition(XMVectorSet(10 * i + 250, 0, 250, 0));//[10, 10]부터 [40, 10]까지 배치
			m_ppPawn[i]->SetScale(XMVectorSet(1, 1, 1, 1));
			break;
		case 1:
			//기사
			pWeapon = new CGameObject("OSW", TAG_DYNAMIC_OBJECT);
			pWeapon->Begin();
			m_ppPawn[i] = new CKnight("Hum01M", TAG_DYNAMIC_OBJECT, bSoul, pWeapon, i);
			m_ppPawn[i]->Begin();
			m_ppPawn[i]->SetPosition(XMVectorSet(10 * i + 250, 0, 250, 0));//[10, 10]부터 [40, 10]까지 배치
			m_ppPawn[i]->SetScale(XMVectorSet(1, 1, 1, 1));
			break;
		case 2:
			//공간술사
			m_ppPawn[i] = new CDementor("Hum03M", TAG_DYNAMIC_OBJECT, bSoul, nullptr, i);
			m_ppPawn[i]->Begin();
			m_ppPawn[i]->SetPosition(XMVectorSet(10 * i + 250, 0, 250, 0));//[10, 10]부터 [40, 10]까지 배치
			m_ppPawn[i]->SetScale(XMVectorSet(1, 1, 1, 1));
			break;
		case 3:
			pWeapon = new CGameObject("THM", TAG_DYNAMIC_OBJECT);
			pWeapon->Begin();
			m_ppPawn[i] = new CSister("Lup01M", TAG_DYNAMIC_OBJECT, bSoul, pWeapon, i);
			m_ppPawn[i]->Begin();
			m_ppPawn[i]->SetPosition(XMVectorSet(10 * i + 250, 0, 250, 0));//[10, 10]부터 [40, 10]까지 배치
			m_ppPawn[i]->SetScale(XMVectorSet(1, 1, 1, 1));
			break;
		case 4:
			m_ppPawn[i] = new CWizard("Hum02F", TAG_DYNAMIC_OBJECT, bSoul, nullptr, i);
			m_ppPawn[i]->Begin();
			m_ppPawn[i]->SetPosition(XMVectorSet(10 * i + 250, 0, 250, 0));//[10, 10]부터 [40, 10]까지 배치
			m_ppPawn[i]->SetScale(XMVectorSet(1, 1, 1, 1));
			break;
		case 5:
			//법사
			m_ppPawn[i] = new CBard("Hum04F", TAG_DYNAMIC_OBJECT, bSoul, nullptr, i);
			m_ppPawn[i]->Begin();
			m_ppPawn[i]->SetPosition(XMVectorSet(10 * i + 250, 0, 250, 0));//[10, 10]부터 [40, 10]까지 배치
			m_ppPawn[i]->SetScale(XMVectorSet(1, 1, 1, 1));
			break;
		}

		if (bSoul) {//player에게 camera 붙혀주기
			m_ppPawn[i]->SetTerrainContainer(UPDATER->GetTerrainContainer());
			m_ppPawn[i]->SetUTag(utag::UTAG_PLAYER);
			m_ppPawn[i]->SetPosition(XMVectorSet(10 * i + 250, 0, 250, 0));//[10, 10]부터 [40, 10]까지 배치
			UPDATER->GetSpaceContainer()->AddObject(m_ppPawn[i]);
			m_ppPawn[i]->GetAnimater()->SetCurAnimationIndex(0);
			CAMMGR->SetTarget(CAM_FREE, m_ppPawn[i]);
			CAMMGR->GetCamera(CAM_FREE)->SetMode(MODE_FIX);
			//들어오자마자 action move start
			AldenardFly();
		}
		else {
			//다른 player는 충돌처리를 허지 않으심
			m_ppPawn[i]->SetUTag(utag::UTAG_OTHERPLAYER);
			UPDATER->GetSpaceContainer()->AddObject(m_ppPawn[i]);
			m_ppPawn[i]->GetAnimater()->SetCurAnimationIndex(0);
		}

		//navi object
		if (false == CNaviObjectManager::GetNaviVertices().empty())
			m_ppPawn[i]->SetNaviMeshIndex();
	}


	//보스 제작
	/*CGameObject*	pBoss = new CLesserGiant("Boss01L", TAG_DYNAMIC_OBJECT, m_ppPawn[0]);
	pBoss->SetUTag(utag::UTAG_BOSS1);
	pBoss->Begin();
	pBoss->SetTerrainContainer(UPDATER->GetTerrainContainer());
	pBoss->SetPosition(XMVectorSet(200, 0, 250, 0));
	pBoss->SetNaviMeshIndex();
	pBoss->SetScale(XMVectorSet(1, 1, 1, 1));
	UPDATER->GetSpaceContainer()->AddObject(pBoss);
	pBoss->GetAnimater()->SetCurAnimationIndex(0);*/
	ResetCollisionValue(XMFLOAT3(179, 20, 290), 20);
#ifdef NO_SERVER
	return CScene::Begin();
#endif
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	NETWORKMGR->WritePacket(PT_ALDENARD_READY_CS, Packet, WRITE_PT_FTOWN_READY_CS(Packet, NETWORKMGR->GetROOM_ID()));

	while (false == m_bGameStart) {
		NetworkProc();
		Sleep(1);
	}
	return CScene::Begin();
}

void CSCAldenard::AldenardFly() {
	//m_bFinalProc = true;//날면 마지막 단계임
	int slot_id = NETWORKMGR->GetSLOT_ID();
	m_ppPawn[slot_id]->ActionMoveEnd();

	char action_move_file_name[128];
	int action_move_id = slot_id + 1;
	if (action_move_id> 3) action_move_id = rand() % 3 + 1;
	sprintf(action_move_file_name, "Aldenard_Fly%d", action_move_id);

	m_ppPawn[slot_id]->ActionMoveStart(action_move_file_name);
}
bool CSCAldenard::End()
{
	CSoundManager::Stop_bgm("bgm_aldenard");
	int nPawn = NETWORKMGR->GetServerPlayerInfos().size();
	for (int i = 0; i < nPawn; ++i)
	{
		if (m_ppPawn[i] == nullptr)
		{
			m_ppPawn[i]->End();
			delete m_ppPawn[i];
			m_ppPawn[i] = nullptr;
		}		
	}

	return CScene::End();
}

void CSCAldenard::Animate(float fTimeElapsed)
{
	XMFLOAT3 xmf3Pos;
	int slot_id = NETWORKMGR->GetSLOT_ID();
	XMStoreFloat3(&xmf3Pos, m_ppPawn[slot_id]->GetPosition());
	DEBUGER->AddGameText(25, 100, 100, YT_Color(200, 200, 0), L"%d %d %d", (int)xmf3Pos.x , (int)xmf3Pos.y, (int)xmf3Pos.z);
	m_fGateSoundLoop += fTimeElapsed;
	m_fBirdSoundLoop += fTimeElapsed;
	if (m_fGateSoundLoop > 5.f) {
		m_fGateSoundLoop = 0.f;
		XMFLOAT3 xmf3Pos = XMFLOAT3(192, 30, 304);
		CSoundManager::Play_3Dsound("bgm_gate_loop", 1, &xmf3Pos, 1.f, 10.f, 200.f);
	}

	CScene::Animate(fTimeElapsed);


	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}
	//int slot_id = NETWORKMGR->GetSLOT_ID();
//flag인 부분 충돌 처리
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	if (FlagCollision(m_ppPawn[slot_id])) {
		//flag인 부분과 충돌했다면!
		m_ppPawn[slot_id]->SetbStay(true);//나 stay!
		m_ppPawn[slot_id]->GetAnimater()->SetCurAnimationIndex(ANIM_IDLE);
		NETWORKMGR->WritePacket(PT_SARASEN_START_CS, Packet, WRITE_PT_SARASEN_START_CS(Packet, NETWORKMGR->GetROOM_ID(), NETWORKMGR->GetSLOT_ID()));
		NETWORKMGR->GetServerPlayerInfos()[NETWORKMGR->GetSLOT_ID()].READY = true;
	}

	if (INPUTMGR->KeyBoardDown(VK_Y))
	{
		char action_move_file_name[128];
		int action_move_id = slot_id + 1;
		if (action_move_id> 3) action_move_id = rand() % 3 + 1;
		sprintf(action_move_file_name, "Aldenard_Fly%d", action_move_id);
		float fSpeed = CPositionInfoManager::GetActionSpeed(action_move_file_name);
		CPositionInfoManager::SetActoionSpeed(action_move_file_name, fSpeed + 1.f);
	}
	else if (INPUTMGR->KeyBoardDown(VK_F))
	{
		AldenardFly();
	}
	else if (INPUTMGR->KeyBoardDown(VK_T))
	{
		SCENEMGR->ChangeScene(SCN_BOSS);
	}

	for (int i = 0; i < 20; ++i)
		NetworkProc();
}

void CSCAldenard::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CSCAldenard::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID) {
	case WM_KEYUP:
		switch (wParam) {
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;
		case VK_P:
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CSCAldenard::ProcessInput(float fTimeElapsed)
{
	if (INPUTMGR->KeyDown(VK_P)) {
		INPUTMGR->SetDebugMode(static_cast<bool>((INPUTMGR->GetDebugMode() + 1) % 2));
	}
	m_pCamera->ProcessInput(fTimeElapsed);
}

CGameObject * CSCAldenard::PickObjectPointedByCursor(int xClient, int yClient)
{
	if (!m_pCamera) return(NULL);

	//Get screen pos -> Camera pos
	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, m_pCamera->GetProjectionMtx());
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	//음 이건 화면을 찍은 점의 카메라 좌표계의 녀석이고
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / xmf4x4Projection._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / xmf4x4Projection._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition;
	xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmMtxViewInverse;
	xmMtxViewInverse = XMMatrixInverse(nullptr, m_pCamera->GetViewMtx());
	//picking pos에 camera inverse를 곱했으니 이건 picking pos의 world pos!
	xmvPickPosition = XMVector3TransformCoord(xmvPickPosition, xmMtxViewInverse);
	XMVECTOR xmvRayDir = xmvPickPosition - m_pCamera->GetPosition();

	CGameObject* pNearestObject = NULL;
	float fHitDistance = FLT_MAX;
	float fNearDistance = FLT_MAX;
	pNearestObject = UPDATER->PickObject(m_pCamera->GetPosition(), XMVector3Normalize(xmvRayDir), fHitDistance);

	fNearDistance = fHitDistance;

	return(pNearestObject);
}

void CSCAldenard::NetworkProc()
{
#ifdef NO_SERVER
	return;
#endif
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD dwProtocol = 0;
	DWORD dwPacketLength = 0;

	if (NETWORKMGR->GetClientSession()->ReadPacket(dwProtocol, Packet, dwPacketLength)) {
		switch (dwProtocol) {
		case PT_FREQUENCY_MOVE_SC:
			PROC_PT_FREQUENCY_MOVE_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_MOUSE_LEFT_ATTACK_SC:
			PROC_PT_MOUSE_LEFT_ATTACK_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ALDENARD_READY_SC:
			PROC_PT_ALDENARD_READY_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_SARASEN_START_SC:
			PROC_PT_SARASEN_START_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_SARASEN_START_COMP_SC:
			PROC_PT_SARASEN_START_COMP_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_RANGE_SKILL_INFO_SC:
			PROC_PT_RANGE_SKILL_INFO_SC(dwProtocol, Packet, dwPacketLength);
			break;
		}
	}
}

VOID CSCAldenard::PROC_PT_RANGE_SKILL_INFO_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_RANGE_SKILL_INFO_SC);

	//Data.SLOT_ID
	//Data.Xyz

	NETWORKMGR->GetServerPlayerInfos()[Data.SLOT_ID].CLICK_DATA.fX = Data.X;
	NETWORKMGR->GetServerPlayerInfos()[Data.SLOT_ID].CLICK_DATA.fY = Data.Y;
	NETWORKMGR->GetServerPlayerInfos()[Data.SLOT_ID].CLICK_DATA.fZ = Data.Z;
	return VOID();
}

VOID CSCAldenard::PROC_PT_ALDENARD_READY_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength)
{
	m_bGameStart = true;
	return VOID();
}

VOID CSCAldenard::PROC_PT_FREQUENCY_MOVE_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_FREQUENCY_MOVE_SC);

	DEBUGER->AddText(25, 10, 400 + Data.SLOT_ID * 50, YT_Color(), L"recv ID : %d", Data.SLOT_ID);
	DEBUGER->AddText(25, 10, 450 + Data.SLOT_ID * 50, YT_Color(), L"%f %f %f", Data.POSX, Data.POSY, Data.POSZ);
	DEBUGER->AddText(25, 10, 450 + Data.SLOT_ID * 50, YT_Color(), L"%f", Data.ANGLEY);

	PLAYR_FREQUENCY_DATA data;
	data.fPosX = Data.POSX;
	data.fPosY = Data.POSY;
	data.fPosZ = Data.POSZ;

	data.fAngleY = Data.ANGLEY;
	data.iAnimNum = Data.ANIMNUM;
	//CPawn* pPawn = (CPawn*)m_ppPawn[Data.SLOT_ID];
	//pPawn->NetworkInput(data.dwDirection, data.fAngleY);
	//network queue에 입력하구 대기한다.
	//NETWORKMGR->GetServerPlayerInfos()[Data.SLOT_ID].m_qFREQUENCY_DATA.push(data);
	NETWORKMGR->GetServerPlayerInfos()[Data.SLOT_ID].FREQUENCY_DATA = data;


	return VOID();
}

VOID CSCAldenard::PROC_PT_MOUSE_LEFT_ATTACK_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_MOUSE_LEFT_ATTACK_SC);

	NETWORKMGR->GetServerPlayerInfos()[Data.SLOT_ID].ATTACK = Data.ATTACK;

	return VOID();
}

VOID CSCAldenard::PROC_PT_SARASEN_START_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_SARASEN_START_SC);

	NETWORKMGR->GetServerPlayerInfos()[Data.SLOT_ID].READY = Data.READY;

	return VOID();
}

VOID CSCAldenard::PROC_PT_SARASEN_START_COMP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {

	for (int i = 0; i < NETWORKMGR->GetServerPlayerInfos().size(); ++i)
		NETWORKMGR->GetServerPlayerInfos()[i].READY = false;

	//
	//
	// 사라센으로 출발엥에에에에ㅔ엥
	//
	//
	SCENEMGR->ChangeScene(SCN_BOSS);
	return VOID();
}


void CSCAldenard::ReadMapData()
{
	IMPORTER->Begin("../../Assets/SceneResource/Aldenard/Aldenard.scn");
	//output path
	wstring wsOutputPath = IMPORTER->ReadWstring();
	//scene name
	wstring wsSceneName = IMPORTER->ReadWstring();
	m_sName.assign(wsSceneName.cbegin(), wsSceneName.cend());

	UPDATER->LoadSpaceInfo();
	UPDATER->LoadTerrainInfo(wsOutputPath, wsSceneName);
	UPDATER->LoadObjectsInfo();
	//effect info
	RENDERER->LoadEffectInfo(wsOutputPath, wsSceneName);

	IMPORTER->End();
}

void CSCAldenard::LoadSkillObjects()
{

}

void CSCAldenard::CreateUI()
{
	//RCSELLER->TestingRCAdd();
	CUIObject* pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.1f)), XMLoadFloat2(&XMFLOAT2(45.f, 65.f)), "Boss_Icon", 9.f);
	m_vecUI.push_back(pUI);

	pUI = CHpBar::Create(XMLoadFloat2(&XMFLOAT2(312.f, 745.f)), XMLoadFloat2(&XMFLOAT2(132.f, 8.f)));
	m_vecUI.push_back(pUI);

	pUI = CHpBar::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.12f, WINSIZEY * 0.25f)), XMLoadFloat2(&XMFLOAT2(60.f, 5.f)));
	m_vecUI.push_back(pUI);

	pUI = CHpBar::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.12f, WINSIZEY * 0.3f)), XMLoadFloat2(&XMFLOAT2(60.f, 5.f)));
	m_vecUI.push_back(pUI);

	pUI = CHpBar::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.12f, WINSIZEY * 0.35f)), XMLoadFloat2(&XMFLOAT2(60.f, 5.f)));
	m_vecUI.push_back(pUI);


	string sCharSelect;
	string sSkill1;
	string sSkill2;
	string sSkill3;
	string sSkill4;
	int nChracter = NETWORKMGR->GetServerPlayerInfo(NETWORKMGR->GetSLOT_ID()).CHARACTER;
	switch (nChracter) {
	case 0:
		sCharSelect = "Char_Select_0";
		sSkill1 = "ranger_skicon1";
		sSkill2 = "ranger_skicon2";
		sSkill3 = "ranger_skicon3";
		sSkill4 = "ranger_skicon4";

		break;
	case 1:
		sCharSelect = "Char_Select_1";
		sSkill1 = "knight_skicon1";
		sSkill2 = "knight_skicon2";
		sSkill3 = "knight_skicon3";
		sSkill4 = "knight_skicon4";
		break;
	case 2:
		sCharSelect = "Char_Select_2";
		sSkill1 = "dementor_skicon1";
		sSkill2 = "dementor_skicon2";
		sSkill3 = "dementor_skicon3";
		sSkill4 = "dementor_skicon4";
		break;
	case 3:
		sCharSelect = "Char_Select_3";
		sSkill1 = "sister_skicon1";
		sSkill2 = "sister_skicon2";
		sSkill3 = "sister_skicon3";
		sSkill4 = "sister_skicon4";
		break;
	case 4:
		sCharSelect = "Char_Select_4";
		sSkill1 = "wizard_skicon1";
		sSkill2 = "wizard_skicon2";
		sSkill3 = "wizard_skicon3";
		sSkill4 = "wizard_skicon4";
		break;
	case 5:
		sCharSelect = "Char_Select_5";
		sSkill1 = "bard_skicon1";
		sSkill2 = "bard_skicon2";
		sSkill3 = "bard_skicon3";
		sSkill4 = "bard_skicon4";
		break;
	default:
		break;
	}

	//player icon
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(140.f, 787.f)), XMLoadFloat2(&XMFLOAT2(40.f, 50.f)), sCharSelect, 10.f);
	m_vecUI.push_back(pUI);

	//skill back
	string sSkillBack = "SkillBack_tr";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(202.f, 786.f)), XMLoadFloat2(&XMFLOAT2(22.f, 30.f)), sSkillBack, 9.f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(246.f, 786.f)), XMLoadFloat2(&XMFLOAT2(22.f, 30.f)), sSkillBack, 9.f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(290.f, 786.f)), XMLoadFloat2(&XMFLOAT2(22.f, 30.f)), sSkillBack, 9.f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(334.f, 786.f)), XMLoadFloat2(&XMFLOAT2(22.f, 30.f)), sSkillBack, 9.f);
	m_vecUI.push_back(pUI);

	//skill icon
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(202.f, 786.f)), XMLoadFloat2(&XMFLOAT2(20.f, 28.f)), sSkill1, 9.5f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(246.f, 786.f)), XMLoadFloat2(&XMFLOAT2(20.f, 28.f)), sSkill2, 9.5f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(290.f, 786.f)), XMLoadFloat2(&XMFLOAT2(20.f, 28.f)), sSkill3, 9.5f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(334.f, 786.f)), XMLoadFloat2(&XMFLOAT2(20.f, 28.f)), sSkill4, 9.5f);
	m_vecUI.push_back(pUI);

}
CSCAldenard::CSCAldenard(SCENE_ID eID, CDirectXFramework* pFrameWork): CScene(eID){
	m_pFrameWork = pFrameWork;
}
CSCAldenard::~CSCAldenard(){
}