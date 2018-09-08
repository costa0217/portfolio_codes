
#include "stdafx.h"
#include "NetworkMgr.h"

void clienterror()
{
	exit(-1);
}

#define USE_LOCAL_IP

bool CNetworkManager::Begin() {
#ifdef NO_SERVER
	return false;
#endif
	m_vServerPlayerInfo.clear();

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	m_pClientSession = new CLabClientSession;

	m_fMousePacketTime = 0;
	m_dwprevMouseButton = 0;
	m_fKeyPacketTime = 0;


#ifdef USE_LOCAL_IP
	//로컬 아이피 모드 적용
	if (!m_pClientSession->BeginTcp("127.0.0.1", DEFAULT_PORT)) {
		m_pClientSession->End();
		return FALSE;
	}

#else
	if (!m_pClientSession->BeginTcp("192.168.0.110", DEFAULT_PORT)) {
		m_pClientSession->End();
		return FALSE;
	}
#endif

	m_bWaitData = false;
	m_bLogin = false;

	//WSADATA   wsadata;
	//WSAStartup(MAKEWORD(2, 2), &wsadata);
	//
	//ClientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	return true;
}

bool CNetworkManager::End() {
#ifdef NO_SERVER
	return false;
#endif
	m_vServerPlayerInfo.clear();

	WSACleanup();
	//클라이언트 세션 정상 종료시 TRUE
	//아니면 FALSE
	if (!m_pClientSession->End())
		return FALSE;

	return true;
}

BOOL CNetworkManager::WritePacket(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	if (m_pClientSession->WritePacket(dwProtocol, Packet, dwPacketLength))
		return TRUE;

	return FALSE;
}

BOOL CNetworkManager::Login() {
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	WCHAR ID[10] = { L"TEST" };
	WCHAR PW[10] = { L"TEST" };

	m_bWaitData = true;
	m_pClientSession->WritePacket(PT_LOGIN_CS, Packet, WRITE_PT_LOGIN_CS(Packet, ID, PW));
	//요청 보냄 

	//요청이 도착할 떄까지 공회전을 해야함..
	while (m_bWaitData) {
		PTProc(nullptr);
	}

	//로그인 요청 완료시 결과가 나옴
	return m_bLogin;
}

BOOL CNetworkManager::EnterEarth() {
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	m_bWaitData = true;
	m_pClientSession->WritePacket(PT_ENTER_EARTH_CS, Packet, WRITE_PT_ENTER_EARTH_CS(Packet));
	//요청 보냄 

	//요청이 도착할 떄까지 공회전을 해야함..
	while (m_bWaitData) {
		PTProc(nullptr);
	}

	//로그인 요청 완료시 결과가 나옴
	return m_bEnterEarth;
}

void CNetworkManager::PTProc(CScene* pScene) {
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD dwProtocol = 0;
	DWORD dwPacketLength = 0;

	//LATER. 0414
	//이걸 ServerController가 처리하게 바꾸자!
	if (m_pClientSession->ReadPacket(dwProtocol, Packet, dwPacketLength)) {
		switch (dwProtocol) {
		case PT_ENTER_SERVER_SUC:
			PROC_PT_ENTER_SERVER_SUC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ENTER_SERVER_S:
			PROC_PT_ENTER_SERVER_S(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_LOGIN_SERVER_SUC:
			PROC_PT_LOGIN_SERVER_SUC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_LOGIN_SERVER_FAIL:
			PROC_PT_LOGIN_SERVER_FAIL(dwProtocol, Packet, dwPacketLength);
			break;
			//행성진입
		case PT_ENTER_EARTH_SUC:
			PROC_PT_ENTER_EARTH_SUC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ENTER_EARTH_FAIL:
			PROC_PT_ENTER_EARTH_FAIL(dwProtocol, Packet, dwPacketLength);
			break;

		case PT_ENTER_SERVER_ALL:
			PROC_PT_ENTER_SERVER_ALL(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_KEY_INPUT_ALL_SC:
			PROC_PT_KEY_INPUT_ALL_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_MOUSE_INPUT_ALL_SC:
			PROC_PT_MOUSE_INPUT_ALL_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_LEAVE_SERVER_ALL:
			PROC_PT_LEAVE_SERVER_ALL(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_FIRE_BULLET_ALL_SC:
			PROC_PT_FIRE_BULLET_ALL_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_PUT_OBJ_SC:
			PROC_PT_PUT_OBJ_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_REMOVE_OBJ_SC:
			PROC_PT_REMOVE_OBJ_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_CHANGE_STATE_OBJ_SC:
			PROC_PT_CHANGE_STATE_OBJ_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;

		case PT_PUT_OBJS_SC:
			PROC_PT_PUT_OBJS_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_REMOVE_OBJS_SC:
			PROC_PT_REMOVE_OBJS_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_PUT_PLAYER_SC:
			PROC_PT_PUT_PLAYER_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_REMOVE_PLAYER_SC:
			PROC_PT_REMOVE_PLAYER_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;
		case PT_CHANGE_STATE_PLAYER_SC:
			PROC_PT_CHANGE_STATE_PLAYER_SC(pScene, dwProtocol, Packet, dwPacketLength);
			break;


		default:
			break;

		}
	}
}


//키보드 입력전송
BOOL CNetworkManager::KeyInputProc(DWORD dwDirection) {
	//
	//	//player의 이전 dwDirection과 지금 만들어진 dwDirection이 다르다면 
	//	if (dwDirection != m_pMyPlayer->GetdwSendDirection()) {
	//		//현재 dwDirection을 set해준다.
	//		m_pMyPlayer->SetdwSendDirection(dwDirection);
	//	
	//		BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	//		XMFLOAT3 pos;
	//		XMStoreFloat3(&pos, m_pMyPlayer->GetPosition());
	//	
	//		//서버에 통보 야! 나 dwDirection 바꿨어! Later 0404
	//		if (!m_pClientSession->WritePacket(PT_KEY_INPUT_CS, Packet, WRITE_PT_KEY_INPUT_CS(Packet, dwDirection, pos.x, pos.y, pos.z)))
	//			return FALSE;
	//	}
	//	if (0 != dwDirection) {
	//		//지금 버튼이 눌려있다면
	//		//현제 속도에 비례해서 많이 패킷을 보냄
	//		//0.03 = 30fps
	//		//0.015 = 약 60fps
	//		if (m_fKeyPacketTime > 0.1) {
	//			//1초에 10번씩
	//			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	//	
	//			XMFLOAT3 xmfPos;
	//			XMStoreFloat3(&xmfPos, m_pMyPlayer->GetPlayerPosition());
	//			//위치 동기화
	//			if (!m_pClientSession->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, PLAYER, m_pMyPlayer->GetObjectID(),
	//				xmfPos.x, xmfPos.y, xmfPos.z)))
	//				return FALSE;
	//	
	//			//std::cout << "send PT_FREQUENCY_MOVE_CS " << std::endl;
	//			m_fKeyPacketTime = 0;
	//		}
	//	}
	//	else {
	//		//안눌려 있다면 Idle
	//		if (m_fKeyPacketTime > 0.5) {
	//			//1초에 2번씩
	//			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	//	
	//			XMFLOAT3 xmfPos;
	//			XMStoreFloat3(&xmfPos, m_pMyPlayer->GetPlayerPosition());
	//			//위치 동기화
	//			if (!m_pClientSession->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, PLAYER, m_pMyPlayer->GetObjectID(),
	//				xmfPos.x, xmfPos.y, xmfPos.z)))
	//				return FALSE;
	//	
	//	
	//			//std::cout << "send PT_FREQUENCY_MOVE_CS " << std::endl;
	//			m_fKeyPacketTime = 0;
	//	
	//	
	//		}
	//	}


	return TRUE;
}

//마우스 입력전송
BOOL CNetworkManager::MouseMoveProc(float cxDelta, float cyDelta, DWORD dwMouseButton) {

	//if ((cxDelta != 0.0f) || (cyDelta != 0.0f))
	//{
	//	if (cxDelta || cyDelta)
	//	{
	//		//0.03 = 30fps
	//		//0.015 = 약 60fps
	//		if (m_fMousePacketTime > 0.015) {
	//			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	//
	//			//recvDirection을 바굼 = 플레이어의 움직임의 변화
	//			if (M_RBUTTON & dwMouseButton)
	//				m_pMyPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
	//			else
	//				m_pMyPlayer->Rotate(cyDelta, cxDelta, 0.0f);
	//
	//
	//			if (!m_pClientSession->WritePacket(PT_MOUSE_INPUT_CS, Packet, WRITE_PT_MOUSE_INPUT_CS(Packet, cxDelta, cyDelta, dwMouseButton)))
	//				return FALSE;
	//
	//			m_fMousePacketTime = 0;
	//
	//
	//		}
	//
	//		//서버 test 하기 위한 마우스 인풋 통일 + 서버에서 승인받은 정보로 Rotate해줄 꺼다
	//	}
	//	//if (dwDirection) m_pPlayer->Move(dwDirection, fPlayerSpeed * m_GameTimer.GetTimeElapsed(), true);
	//	//테스트를 위해 이곳의 Move는 지운다. 서버의 응답으로만 플레이어가 움직이게 한다.
	//}

	return TRUE;

}

BOOL CNetworkManager::MouseButtonProc(DWORD dwMouseButton) {

	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD dwProtocol = 0;
	DWORD dwPacketLength = 0;

	//이전에 보낸 입력과 다른 입력이 들어왔다면 
	if (dwMouseButton != m_dwprevMouseButton) {
		m_dwprevMouseButton = dwMouseButton;
		if (!m_pClientSession->WritePacket(PT_MOUSE_BUTTON_INPUT_CS, Packet, WRITE_PT_MOUSE_BUTTON_INPUT_CS(Packet, dwMouseButton)))
			return FALSE;
	}

	return TRUE;
}

void CNetworkManager::Update(float fTimeElapsed) {
	m_fMousePacketTime += fTimeElapsed;
	m_fKeyPacketTime += fTimeElapsed;

	if (m_bConnect) {
		//DEBUGER->AddText(50, 500, 500,YT_Color(), L"Connect");
	}
}


void CNetworkManager::AddServerPlayerInfo(INT CHARACTER) {
	ServerPlayerInfo Data = ServerPlayerInfo{ CHARACTER };
	m_vServerPlayerInfo.push_back(Data);
}
ServerPlayerInfo CNetworkManager::GetServerPlayerInfo(INT SLOT_ID)
{
	if (m_vServerPlayerInfo.size() < SLOT_ID) return ServerPlayerInfo{ -1 };

	return m_vServerPlayerInfo[SLOT_ID];
}
PLAYR_FREQUENCY_DATA CNetworkManager::GetPlayerFrequencyData(INT SLOT_ID) {
	PLAYR_FREQUENCY_DATA FREQUENCY_DATA;
	if (m_vServerPlayerInfo.size() < SLOT_ID) return FREQUENCY_DATA;
	//if (m_vServerPlayerInfo[SLOT_ID].m_qFREQUENCY_DATA.empty()) return FREQUENCY_DATA;

	//있으면 임시 저장한 queue중에 맨 위에꺼 꺼내서 준다.
	//FREQUENCY_DATA = m_vServerPlayerInfo[SLOT_ID].m_qFREQUENCY_DATA.front();
	//m_vServerPlayerInfo[SLOT_ID].m_qFREQUENCY_DATA.pop();
	FREQUENCY_DATA = m_vServerPlayerInfo[SLOT_ID].FREQUENCY_DATA;
	return FREQUENCY_DATA;
}
BOOL CNetworkManager::GetAttack(INT SLOT_ID) {
	if (m_vServerPlayerInfo[SLOT_ID].ATTACK == true)
	{
		m_vServerPlayerInfo[SLOT_ID].ATTACK = false;
		return true;
	}
	return false;
}
CNetworkManager::CNetworkManager() : CSingleTonBase<CNetworkManager>("networkmanagersingleton") {

}

CNetworkManager::~CNetworkManager() {

}
