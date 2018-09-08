#include "stdafx.h"
#include "NetworkMgr.h"
#include "Ranger.h"

VOID CNetworkManager::PROC_PT_ENTER_SPACE(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//test
}

VOID CNetworkManager::PROC_PT_ENTER_SERVER_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//NetworkID받기
	READ_PACKET(PT_ENTER_SERVER_SUC);

	NETWORK_ID = Data.ID;
	//접속이 됨
	//여기서 뭐 로그인 버튼 활성화 같은거 해줌
	m_bConnect = true;
}
VOID CNetworkManager::PROC_PT_LOGIN_SERVER_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//로그인 성공 플래그
	m_bWaitData = false;
	m_bLogin = true;
}
VOID CNetworkManager::PROC_PT_LOGIN_SERVER_FAIL(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//로그인 실패 플래그
	m_bWaitData = false;
	m_bLogin = false;

}

//////////////////////////행성진입
VOID CNetworkManager::PROC_PT_ENTER_EARTH_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {

	READ_PACKET(PT_ENTER_EARTH_SUC);

	//로그인 성공 플래그
	//m_pMyPlayer->SetObjectID(Data.MYID);
	//m_pMyPlayer->SetTeam(Data.TEAM);


	XMFLOAT3 xmfPos;
	xmfPos.x = Data.X;
	xmfPos.y = Data.Y;
	xmfPos.z = Data.Z;

	//m_pMyPlayer->SetPlayerPosition(XMLoadFloat3(&xmfPos));

	m_bWaitData = false;
	m_bEnterEarth = true;
}
VOID CNetworkManager::PROC_PT_ENTER_EARTH_FAIL(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//로그인 실패 플래그
	m_bWaitData = false;
	m_bEnterEarth = false;

}
//////////////////////////행성진입

VOID CNetworkManager::PROC_PT_ENTER_SERVER_S(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ENTER_SERVER_S);


	////서버에서 받은 정보를 토대로 위치 조정
	//m_pMyPlayer->SetPlayerPosition(Data.X, Data.Y, Data.Z);
	//m_pMyPlayer->SetObjectID(Data.MYID);

	XMFLOAT3 xmfPos;
	xmfPos.x = Data.X;
	xmfPos.y = Data.Y;
	xmfPos.z = Data.Z;


	//m_pMyPlayer->SetPlayerPosition(XMLoadFloat3(&xmfPos));

	std::cout << "PT_ENTER_SERVER_S msg : " << Data.MYID << std::endl;

}

VOID CNetworkManager::PROC_PT_ENTER_SERVER_ALL(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ENTER_SERVER_ALL);

	//내 아이디가 아니라면
	//if (m_pMyPlayer->GetObjectID() != Data.ID) {
	//	m_pOtherPlayerManager->AddOtherPlayer(Data.ID, Data.X, Data.Y, Data.Z);
	//}

	std::cout << "PT_ENTER_SERVER_ALL msg : " << Data.ID << std::endl;
}

VOID CNetworkManager::PROC_PT_LEAVE_SERVER_ALL(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_LEAVE_SERVER_ALL);


	//m_pOtherPlayerManager->RemoveOtherPlayer(Data.ID);

	std::cout << "PT_LEAVE_SERVER_ALL msg : " << Data.ID << std::endl;
}


VOID CNetworkManager::PROC_PT_KEY_INPUT_ALL_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_KEY_INPUT_ALL_SC);

	//서버에서 보낸 id가 마침 내 id 라면 내 정보 갱신
	//if (m_pMyPlayer->GetObjectID() == Data.ID) {


		////recvDirection을 바굼 = 플레이어의 움직임의 변화
		//m_pMyPlayer->SetdwRecvDirection(Data.DIRECTION);
		//m_pMyPlayer->SetPlayerPosition(XMLoadFloat3(&XMFLOAT3(Data.X, Data.Y, Data.Z)));

		XMFLOAT3 xmfPos;

		//XMStoreFloat3(&xmfPos, m_pMyPlayer->GetPlayerPosition());

		std::cout << _T("PT_KEY_INPUT_ALL_SC - my id - ") << Data.ID << std::endl;
		std::cout << "dir : " << Data.DIRECTION;
		std::cout << "pos : " << Data.X << ", " << Data.Y << ", " << Data.Z << std::endl;

		return;
	//}
	//아니면
	//{
	//
	//	m_pOtherPlayerManager->UpdateOtherPlayer(Data.ID, Data.DIRECTION, Data.X, Data.Y, Data.Z);
	//
	//	std::cout << _T("PT_KEY_INPUT_ALL_SC - other id - ") << Data.ID << std::endl;
	//	std::cout << "dir : " << Data.DIRECTION;
	//	std::cout << "pos : " << Data.X << ", " << Data.Y << ", " << Data.Z << std::endl;
	//
	//	return;
	//}

}

VOID CNetworkManager::PROC_PT_MOUSE_INPUT_ALL_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_MOUSE_INPUT_ALL_SC);

	//서버에서 보낸 id가 마침 내 id 라면 내 정보 갱신
	//if (m_pMyPlayer->GetObjectID() == Data.ID) {

		////recvDirection을 바굼 = 플레이어의 움직임의 변화
		//if (M_RBUTTON & Data.MOUSEBUTTON)
		//	m_pMyPlayer->Rotate(Data.CYDELTA, 0.0f, -Data.CXDELTA);
		//else
		//	m_pMyPlayer->Rotate(Data.CYDELTA, Data.CXDELTA, 0.0f);
		//
		return;
	//}
	//아니면
	//{
	//	m_pOtherPlayerManager->UpdateOtherPlayer(Data.ID, Data.CXDELTA, Data.CYDELTA, Data.MOUSEBUTTON);
	//
	//}
}

VOID CNetworkManager::PROC_PT_FIRE_BULLET_ALL_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_FIRE_BULLET_ALL_SC);

	//서버에서 보낸 id가 마침 내 id 라면 내 정보 갱신
	//if (m_pMyPlayer->GetObjectID() == Data.ID) {
		cout << "총알 발싸!" << endl;
	//	BULLET_MGR->FireBullet(m_pMyPlayer, Data.BULLETID);
	//	return;
	//}
	//아니면
	//{
	//	//다른 플레이어 발싸
	//	m_pOtherPlayerManager->OtherPlayerFireBullet(Data.ID, Data.BULLETID);
	//
	//}
}


VOID CNetworkManager::PROC_PT_PUT_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_PUT_OBJ_SC);

	//내 아이디가 아니라면

	//m_pServerObjectManager->AddServerObject(Data.ID, Data.X, Data.Y, Data.Z);
	XMFLOAT3 pos(Data.X, Data.Y, Data.Z);
	XMFLOAT3 dir(Data.MOVINGDIR_X, Data.MOVINGDIR_Y, Data.MOVINGDIR_Z);


	//m_pServerObjectManager->AddServerObject(Data.ID, Data.TAG, pos, dir, Data.SPD, Data.ACCSPD);


	std::cout << "PT_PUT_OBJECT_SC msg : " << Data.ID << std::endl;
}

VOID CNetworkManager::PROC_PT_REMOVE_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_REMOVE_OBJ_SC);

	//m_pServerObjectManager->RemoveServerObject(Data.ID, Data.TAG);

	//총알 삭제 로직..

	std::cout << "PT_REMOVE_OBJECT_SC msg : " << Data.ID << std::endl;
}
VOID CNetworkManager::PROC_PT_CHANGE_STATE_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_CHANGE_STATE_OBJ_SC);

	XMFLOAT3 pos(Data.X, Data.Y, Data.Z);
	XMFLOAT3 dir(Data.MOVINGDIR_X, Data.MOVINGDIR_Y, Data.MOVINGDIR_Z);


	//m_pServerObjectManager->SetServerObject(Data.ID, Data.TAG, pos, dir, Data.SPD, Data.ACCSPD);

	//m_pServerObjectManager->SetServerObjectPosition(Data.ID, Data.X, Data.Y, Data.Z);
	std::cout << "PT_CHANGE_STATE_OBJECT_SC msg : " << Data.ID << std::endl;
}


VOID CNetworkManager::PROC_PT_PUT_OBJS_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_PUT_OBJS_SC);

	//내 아이디가 아니라면

	//m_pServerObjectManager->AddServerObject(Data.ID, Data.X, Data.Y, Data.Z);
	for (int i = 0; i < Data.COUNT; i++) {
		XMFLOAT3 pos(Data.X[i], Data.Y[i], Data.Z[i]);
		XMFLOAT3 dir(Data.MOVINGDIR_X[i], Data.MOVINGDIR_Y[i], Data.MOVINGDIR_Z[i]);


		//m_pServerObjectManager->AddServerObject(Data.ID[i], Data.TAG[i], pos, dir, Data.SPD[i], Data.ACCSPD[i]);
		
	}


	std::cout << "PT_PUT_OBJECTS_SC msg : " << Data.COUNT << std::endl;
}


VOID CNetworkManager::PROC_PT_REMOVE_OBJS_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_REMOVE_OBJS_SC);

	//내 아이디가 아니라면

	//m_pServerObjectManager->AddServerObject(Data.ID, Data.X, Data.Y, Data.Z);
	for (int i = 0; i < Data.COUNT; i++) {


		//m_pServerObjectManager->RemoveServerObject(Data.ID[i], Data.TAG[i]);


	}


	std::cout << "PT_REMOVE_OBJECTS_SC msg : " << Data.COUNT << std::endl;
}



//player PUT REMOVE CHANGESTATE
VOID CNetworkManager::PROC_PT_PUT_PLAYER_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_PUT_PLAYER_SC);

	//m_pOtherPlayerManager->AddOtherPlayer(Data.ID, Data.DIRECTION,
	//
	//	Data.RIGHTX, Data.RIGHTY, Data.RIGHTZ,
	//	Data.UPX, Data.UPY, Data.UPZ,
	//	Data.LOOKX, Data.LOOKY, Data.LOOKZ,
	//	Data.POSX, Data.POSY, Data.POSZ);


	std::cout << "PT_PUT_PLAYER_SC msg : " << Data.ID << std::endl;

	std::cout << "r " << Data.RIGHTX << Data.RIGHTY << Data.RIGHTZ << std::endl;
	std::cout << "u " << Data.UPX << Data.UPY << Data.UPZ << std::endl;
	std::cout << "l " << Data.LOOKX << Data.LOOKY << Data.LOOKZ << std::endl;
}

VOID CNetworkManager::PROC_PT_REMOVE_PLAYER_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_REMOVE_PLAYER_SC);

	//m_pOtherPlayerManager->RemoveOtherPlayer(Data.ID);

	std::cout << "PT_REMOVE_PLAYER_SC msg : " << Data.ID << std::endl;
}
VOID CNetworkManager::PROC_PT_CHANGE_STATE_PLAYER_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_CHANGE_STATE_PLAYER_SC);
	//
	//XMFLOAT3 pos(Data.X, Data.Y, Data.Z);
	//XMFLOAT3 dir(Data.MOVINGDIR_X, Data.MOVINGDIR_Y, Data.MOVINGDIR_Z);
	//
	//
	//m_pServerObjectManager->SetServerObject(Data.ID, pos, dir, Data.SPD, Data.ACCSPD);

	//m_pServerObjectManager->SetServerObjectPosition(Data.ID, Data.X, Data.Y, Data.Z);
	std::cout << "PT_CHANGE_STATE_PLAYER_SC msg : " << Data.ID << std::endl;
}


////주기적 동기화
//VOID CNetworkManager::PROC_PT_FREQUENCY_MOVE_SC(CScene* pScene, DWORD dwProtocol, BYTE *Packet, DWORD dwPacketLength)
//{
//	// 전처리 함수로 간략화
//	READ_PACKET(PT_FREQUENCY_MOVE_SC);
//	// 실제 코드
//	//BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };
//	//S_PT_REG_USER Data;
//	//READ_PT_REG_USER(pPacket, Data);
//	
//	m_vServerPlayerInfo[Data.SLOT_ID].FREQUENCY_DATA.dwDirection = Data.DWDIR;
//	m_vServerPlayerInfo[Data.SLOT_ID].FREQUENCY_DATA.fAngleY = Data.ANGLEY;
//
//	//위치 동기화
//	//m_pOtherPlayerManager->SetOtherPlayerPosition(Data.ID, Data.X, Data.Y, Data.Z);
//}

