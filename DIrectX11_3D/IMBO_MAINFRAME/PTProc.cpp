#include "stdafx.h"
#include "NetworkMgr.h"
#include "Ranger.h"

VOID CNetworkManager::PROC_PT_ENTER_SPACE(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//test
}

VOID CNetworkManager::PROC_PT_ENTER_SERVER_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//NetworkID�ޱ�
	READ_PACKET(PT_ENTER_SERVER_SUC);

	NETWORK_ID = Data.ID;
	//������ ��
	//���⼭ �� �α��� ��ư Ȱ��ȭ ������ ����
	m_bConnect = true;
}
VOID CNetworkManager::PROC_PT_LOGIN_SERVER_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//�α��� ���� �÷���
	m_bWaitData = false;
	m_bLogin = true;
}
VOID CNetworkManager::PROC_PT_LOGIN_SERVER_FAIL(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	//�α��� ���� �÷���
	m_bWaitData = false;
	m_bLogin = false;

}

//////////////////////////�༺����
VOID CNetworkManager::PROC_PT_ENTER_EARTH_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {

	READ_PACKET(PT_ENTER_EARTH_SUC);

	//�α��� ���� �÷���
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
	//�α��� ���� �÷���
	m_bWaitData = false;
	m_bEnterEarth = false;

}
//////////////////////////�༺����

VOID CNetworkManager::PROC_PT_ENTER_SERVER_S(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ENTER_SERVER_S);


	////�������� ���� ������ ���� ��ġ ����
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

	//�� ���̵� �ƴ϶��
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

	//�������� ���� id�� ��ħ �� id ��� �� ���� ����
	//if (m_pMyPlayer->GetObjectID() == Data.ID) {


		////recvDirection�� �ٱ� = �÷��̾��� �������� ��ȭ
		//m_pMyPlayer->SetdwRecvDirection(Data.DIRECTION);
		//m_pMyPlayer->SetPlayerPosition(XMLoadFloat3(&XMFLOAT3(Data.X, Data.Y, Data.Z)));

		XMFLOAT3 xmfPos;

		//XMStoreFloat3(&xmfPos, m_pMyPlayer->GetPlayerPosition());

		std::cout << _T("PT_KEY_INPUT_ALL_SC - my id - ") << Data.ID << std::endl;
		std::cout << "dir : " << Data.DIRECTION;
		std::cout << "pos : " << Data.X << ", " << Data.Y << ", " << Data.Z << std::endl;

		return;
	//}
	//�ƴϸ�
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

	//�������� ���� id�� ��ħ �� id ��� �� ���� ����
	//if (m_pMyPlayer->GetObjectID() == Data.ID) {

		////recvDirection�� �ٱ� = �÷��̾��� �������� ��ȭ
		//if (M_RBUTTON & Data.MOUSEBUTTON)
		//	m_pMyPlayer->Rotate(Data.CYDELTA, 0.0f, -Data.CXDELTA);
		//else
		//	m_pMyPlayer->Rotate(Data.CYDELTA, Data.CXDELTA, 0.0f);
		//
		return;
	//}
	//�ƴϸ�
	//{
	//	m_pOtherPlayerManager->UpdateOtherPlayer(Data.ID, Data.CXDELTA, Data.CYDELTA, Data.MOUSEBUTTON);
	//
	//}
}

VOID CNetworkManager::PROC_PT_FIRE_BULLET_ALL_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_FIRE_BULLET_ALL_SC);

	//�������� ���� id�� ��ħ �� id ��� �� ���� ����
	//if (m_pMyPlayer->GetObjectID() == Data.ID) {
		cout << "�Ѿ� �߽�!" << endl;
	//	BULLET_MGR->FireBullet(m_pMyPlayer, Data.BULLETID);
	//	return;
	//}
	//�ƴϸ�
	//{
	//	//�ٸ� �÷��̾� �߽�
	//	m_pOtherPlayerManager->OtherPlayerFireBullet(Data.ID, Data.BULLETID);
	//
	//}
}


VOID CNetworkManager::PROC_PT_PUT_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_PUT_OBJ_SC);

	//�� ���̵� �ƴ϶��

	//m_pServerObjectManager->AddServerObject(Data.ID, Data.X, Data.Y, Data.Z);
	XMFLOAT3 pos(Data.X, Data.Y, Data.Z);
	XMFLOAT3 dir(Data.MOVINGDIR_X, Data.MOVINGDIR_Y, Data.MOVINGDIR_Z);


	//m_pServerObjectManager->AddServerObject(Data.ID, Data.TAG, pos, dir, Data.SPD, Data.ACCSPD);


	std::cout << "PT_PUT_OBJECT_SC msg : " << Data.ID << std::endl;
}

VOID CNetworkManager::PROC_PT_REMOVE_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_REMOVE_OBJ_SC);

	//m_pServerObjectManager->RemoveServerObject(Data.ID, Data.TAG);

	//�Ѿ� ���� ����..

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

	//�� ���̵� �ƴ϶��

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

	//�� ���̵� �ƴ϶��

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


////�ֱ��� ����ȭ
//VOID CNetworkManager::PROC_PT_FREQUENCY_MOVE_SC(CScene* pScene, DWORD dwProtocol, BYTE *Packet, DWORD dwPacketLength)
//{
//	// ��ó�� �Լ��� ����ȭ
//	READ_PACKET(PT_FREQUENCY_MOVE_SC);
//	// ���� �ڵ�
//	//BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };
//	//S_PT_REG_USER Data;
//	//READ_PT_REG_USER(pPacket, Data);
//	
//	m_vServerPlayerInfo[Data.SLOT_ID].FREQUENCY_DATA.dwDirection = Data.DWDIR;
//	m_vServerPlayerInfo[Data.SLOT_ID].FREQUENCY_DATA.fAngleY = Data.ANGLEY;
//
//	//��ġ ����ȭ
//	//m_pOtherPlayerManager->SetOtherPlayerPosition(Data.ID, Data.X, Data.Y, Data.Z);
//}

